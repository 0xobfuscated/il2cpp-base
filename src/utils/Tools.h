#pragma once

#include <jni.h>
#include <stdint.h>

void* ManualDlsym(uintptr_t mmapBase, const char* symName);

namespace Tools {
    uintptr_t GetBaseAddress(const char* name);

    JavaVM* GetJavaVM();
    JNIEnv* GetJNIEnv();
    jclass FindClass(JNIEnv* env, const char* className);
}
