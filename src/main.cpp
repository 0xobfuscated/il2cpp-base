#include <jni.h>
#include <pthread.h>
#include <unistd.h>
#include <dlfcn.h>
#include <cstdio>
#include <cstring>
#include <cinttypes>
#include <elf.h>

#include "BNM/Loading.hpp"
#include "game/DebugController.h"
#include "gui/ImGuiSetup.h"
#include "gui/gui.h"
#include "utils/Tools.h"
#include "utils/log.h"

static JavaVM* JVM = nullptr;
static bool BNMLoaded  = false;


void OnBNMLoaded() {
    LOGI("[OnBNMLoaded] BNM fully loaded");
    game::DebugController::Init();
    game::DebugController::GetInstance();
}

void* MainThread(void*) {
    LOGI("[*] MainThread started");
    
    uintptr_t base;
    while ((base = Tools::GetBaseAddress("libil2cpp.so")) == 0) {
        usleep(100000);
    }
    LOGI("[*] libil2cpp.so base: %p", (void *)base);

    int PointerSize = sizeof(void *);
    LOGI("[*] pointer size: %d", PointerSize);

    BNM::Loading::AllowLateInitHook();
    BNM::Loading::AddOnLoadedEvent(OnBNMLoaded);

    usleep(500000);

    if (!BNMLoaded) {
        LOGI("Attempting BNM loading via Dlfcn handle");
        void* handle = dlopen("libil2cpp.so", RTLD_LAZY);
        if (handle) {
            if (BNM::Loading::TryLoadByDlfcnHandle(handle)) {
                LOGI("[*] BNM loaded successfully.");
                BNMLoaded = true;
            } else {
                LOGE("[*] BNM loading failed via Dlfcn handle");
            }
        } else {
            LOGE("[*] Failed to dlopen libil2cpp.so");
        }
    }

    if (JVM == nullptr)
        JVM = Tools::GetJavaVM();

    if (JVM != nullptr) {
        ImGuiSetup::SetupHooks(JVM);
    } else {
        LOGE("[*] JavaVM unavailable!!!");
        ImGuiSetup::SetupHooks(nullptr);
    }

    LOGI("[*] BNM initialized.");
    return nullptr;
}

__attribute__((constructor))
void InitMain() {
    pthread_t mainThread;
    pthread_create(&mainThread, nullptr, MainThread, nullptr);
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    JVM = vm;
    JNIEnv* env = nullptr;
    if (vm->GetEnv((void**)&env, JNI_VERSION_1_6) == JNI_OK) {
        LOGI("[*] JNI_OnLoad called");
        BNM::Loading::AllowLateInitHook();
        BNM::Loading::AddOnLoadedEvent(OnBNMLoaded);
        if (BNM::Loading::TryLoadByJNI(env)) {
            BNMLoaded = true;
        }
    }
    return JNI_VERSION_1_6;
}
