#ifndef IMGUI_SETUP_H
#define IMGUI_SETUP_H

#include <jni.h>

namespace ImGuiSetup {
    void SetupHooks(JavaVM* vm);
    void Shutdown();
    bool IsInitialized();
}

#endif // IMGUI_SETUP_H
