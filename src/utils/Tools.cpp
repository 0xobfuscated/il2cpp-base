#include "Tools.h"

#include <dlfcn.h>
#include <inttypes.h>
#include <jni.h>
#include <libgen.h>
#include <stdio.h>
#include <string.h>
#include <elf.h>

#include "log.h"

void* ManualDlsym(uintptr_t mmapBase, const char* symName) {
    if (!mmapBase) return nullptr;

    auto* ehdr = reinterpret_cast<Elf64_Ehdr*>(mmapBase);
    if (memcmp(ehdr->e_ident, ELFMAG, SELFMAG) != 0) return nullptr;

    auto* phdr = reinterpret_cast<Elf64_Phdr*>(mmapBase + ehdr->e_phoff);

    uintptr_t libBase = mmapBase;
    Elf64_Dyn* dyn = nullptr;
    bool baseSet = false;

    for (int i = 0; i < ehdr->e_phnum; ++i) {
        if (!baseSet && phdr[i].p_type == PT_LOAD) {
            libBase = mmapBase - phdr[i].p_vaddr;
            baseSet = true;
        }
        if (phdr[i].p_type == PT_DYNAMIC)
            dyn = reinterpret_cast<Elf64_Dyn*>(libBase + phdr[i].p_vaddr);
    }
    if (!dyn) return nullptr;

    Elf64_Sym* symtab = nullptr;
    const char* strtab = nullptr;
    uint32_t* hashtab = nullptr;
    uint32_t* gnuHash = nullptr;

    for (Elf64_Dyn* d = dyn; d->d_tag != DT_NULL; ++d) {
        switch (d->d_tag) {
            case DT_SYMTAB:   symtab  = reinterpret_cast<Elf64_Sym*> (libBase + d->d_un.d_ptr); break;
            case DT_STRTAB:   strtab  = reinterpret_cast<const char*>(libBase + d->d_un.d_ptr); break;
            case DT_HASH:     hashtab = reinterpret_cast<uint32_t*>  (libBase + d->d_un.d_ptr); break;
            case DT_GNU_HASH: gnuHash = reinterpret_cast<uint32_t*>  (libBase + d->d_un.d_ptr); break;
            default: break;
        }
    }
    if (!symtab || !strtab) return nullptr;

    size_t symCount = 0;
    if (hashtab) {
        symCount = hashtab[1];
    } else if (gnuHash) {
        uint32_t nbuckets = gnuHash[0];
        uint32_t bloomSize = gnuHash[2];
        const uint32_t* buckets = &gnuHash[4 + bloomSize * 2];
        uint32_t maxIdx = 0;
        for (uint32_t i = 0; i < nbuckets; ++i)
            if (buckets[i] > maxIdx) maxIdx = buckets[i];
        symCount = maxIdx ? maxIdx + 100 : 50000;
    } else {
        symCount = 50000;
    }

    for (size_t i = 0; i < symCount; ++i) {
        if (symtab[i].st_value == 0) continue;
        if (strcmp(strtab + symtab[i].st_name, symName) == 0)
            return reinterpret_cast<void*>(libBase + symtab[i].st_value);
    }
    return nullptr;
}

uintptr_t Tools::GetBaseAddress(const char* name) {
    FILE* maps = fopen("/proc/self/maps", "r");
    if (!maps) return 0;

    uintptr_t result = 0;
    char line[512];
    while (fgets(line, sizeof(line), maps)) {
        char tmpName[256];
        uintptr_t tmpBase;
        if (sscanf(line, "%" PRIXPTR "-%*" PRIXPTR " %*s %*s %*s %*s %s", &tmpBase, tmpName) > 0) {
            if (strcmp(name, basename(tmpName)) == 0) {
                result = tmpBase;
                break;
            }
        }
    }
    fclose(maps);
    return result;
}

JavaVM* Tools::GetJavaVM() {
    using GetVMsFn = jint(*)(JavaVM**, jsize, jsize*);

    auto getVMs = reinterpret_cast<GetVMsFn>(dlsym(RTLD_DEFAULT, "JNI_GetCreatedJavaVMs"));

    if (!getVMs) {
        LOGW("GetJavaVM: RTLD_DEFAULT failed, falling back to manual ELF walk");
        uintptr_t artBase = Tools::GetBaseAddress("libart.so");
        if (artBase){
            getVMs = reinterpret_cast<GetVMsFn>(ManualDlsym(artBase, "JNI_GetCreatedJavaVMs"));
        }
    }

    if (!getVMs) {
        LOGE("GetJavaVM: failed to resolve JNI_GetCreatedJavaVMs");
        return nullptr;
    }

    JavaVM* vm = nullptr;
    jsize count = 0;
    if (getVMs(&vm, 1, &count) != JNI_OK || count == 0 || !vm) {
        LOGE("GetJavaVM: JNI_GetCreatedJavaVMs returned no VM");
        return nullptr;
    }

    LOGI("GetJavaVM: acquired JavaVM @ %p", vm);
    return vm;
}

JNIEnv* Tools::GetJNIEnv() {
    JavaVM* vm = Tools::GetJavaVM();
    if (!vm) return nullptr;

    JNIEnv* env = nullptr;
    if (vm->GetEnv((void**)&env, JNI_VERSION_1_6) != JNI_OK) {
        if (vm->AttachCurrentThread(&env, nullptr) != JNI_OK)
            return nullptr;
    }
    return env;
}

static void ClearPendingJNIException(JNIEnv* env) {
    if (env && env->ExceptionCheck())
        env->ExceptionClear();
}

static jobject GetApplicationContext(JNIEnv* env) {
    if (!env) return nullptr;

    jclass activityThreadClass = env->FindClass("android/app/ActivityThread");
    if (!activityThreadClass) {
        ClearPendingJNIException(env);
        return nullptr;
    }

    jmethodID currentApplication = env->GetStaticMethodID(
        activityThreadClass, "currentApplication", "()Landroid/app/Application;");
    if (!currentApplication) {
        ClearPendingJNIException(env);
        env->DeleteLocalRef(activityThreadClass);
        return nullptr;
    }

    jobject application = env->CallStaticObjectMethod(activityThreadClass, currentApplication);
    ClearPendingJNIException(env);
    env->DeleteLocalRef(activityThreadClass);
    return application;
}

jclass Tools::FindClass(JNIEnv* env, const char* className) {
    if (!env || !className) return nullptr;

    jclass cls = env->FindClass(className);
    if (cls) return cls;
    ClearPendingJNIException(env);

    if (strcmp(className, "com/unity3d/player/UnityPlayerActivity") != 0)
        return nullptr;

    jobject app = GetApplicationContext(env);
    if (!app) return nullptr;

    jclass appClass = env->GetObjectClass(app);
    jmethodID getClassLoader = appClass ? env->GetMethodID(appClass, "getClassLoader", "()Ljava/lang/ClassLoader;") : nullptr;
    jobject classLoader = getClassLoader ? env->CallObjectMethod(app, getClassLoader) : nullptr;
    ClearPendingJNIException(env);

    jclass classLoaderClass = env->FindClass("java/lang/ClassLoader");
    jmethodID loadClass = classLoaderClass ? env->GetMethodID(classLoaderClass, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;") : nullptr;
    jstring dottedName = env->NewStringUTF("com.unity3d.player.UnityPlayerActivity");

    cls = (classLoader && loadClass && dottedName) ? (jclass)env->CallObjectMethod(classLoader, loadClass, dottedName) : nullptr;
    ClearPendingJNIException(env);

    if (dottedName) env->DeleteLocalRef(dottedName);
    if (classLoaderClass) env->DeleteLocalRef(classLoaderClass);
    if (classLoader) env->DeleteLocalRef(classLoader);
    if (appClass) env->DeleteLocalRef(appClass);
    env->DeleteLocalRef(app);

    return cls;
}
