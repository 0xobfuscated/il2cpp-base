#include "ImGuiSetup.h"
#include "gui.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "dobby.h"
#include "../utils/Tools.h"
#include "../utils/log.h"
#include <EGL/egl.h>
#include <GLES3/gl3.h>

namespace ImGuiSetup {
    static bool initialized = false;
    static bool inHook = false;
    static JavaVM* jvm = nullptr;

    typedef EGLBoolean (*eglSwapBuffers_t)(EGLDisplay, EGLSurface);
    static eglSwapBuffers_t orig_eglSwapBuffers = nullptr;

    static jboolean nativeOnTouch(JNIEnv* env, jclass clazz, jint action, jfloat x, jfloat y) {
        if (!initialized) return JNI_FALSE;

        ImGuiIO& io = ImGui::GetIO();
        io.AddMouseSourceEvent(ImGuiMouseSource_TouchScreen);
        io.AddMousePosEvent(x, y);

        switch (action & 0xff) {
            case 0: // ACTION_DOWN
            case 5: // ACTION_POINTER_DOWN
                io.AddMouseButtonEvent(0, true);
                break;
            case 1: // ACTION_UP
            case 6: // ACTION_POINTER_UP
            case 3: // ACTION_CANCEL
                io.AddMouseButtonEvent(0, false);
                break;
            case 2: // ACTION_MOVE
                break;
            default:
                break;
        }

        return io.WantCaptureMouse ? JNI_TRUE : JNI_FALSE;
    }

    static void registerNativeTouchMethod() {
        JNIEnv* env = nullptr;
        if (jvm->GetEnv((void**)&env, JNI_VERSION_1_6) != JNI_OK &&
            jvm->AttachCurrentThread(&env, nullptr) != JNI_OK) {
            LOGE("[*] Failed to attach thread for touch registration");
            return;
        }

        jclass cls = Tools::FindClass(env, "com/unity3d/player/UnityPlayerActivity");
        if (!cls) {
            LOGE("[*] Failed to find UnityPlayerActivity class");
            return;
        }

        JNINativeMethod method = {"nativeOnTouch", "(IFF)Z", (void*)nativeOnTouch};
        if (env->RegisterNatives(cls, &method, 1) == 0) {
            LOGI("[*] Registered nativeOnTouch");
        } else {
            LOGE("[*] Failed to register nativeOnTouch");
            env->ExceptionClear();
        }
        env->DeleteLocalRef(cls);
    }

    static EGLBoolean eglSwapBuffers_hk(EGLDisplay dpy, EGLSurface surface) {
        if (inHook) return orig_eglSwapBuffers(dpy, surface);
        inHook = true;

        if (!initialized) {
            ImGui::CreateContext();
            UI::Init();
            ImGui_ImplOpenGL3_Init("#version 300 es");
            initialized = true;
            LOGI("[*] ImGui initialized");
        }

        ImGuiIO& io = ImGui::GetIO();
        EGLint width = 0, height = 0;
        eglQuerySurface(dpy, surface, EGL_WIDTH, &width);
        eglQuerySurface(dpy, surface, EGL_HEIGHT, &height);
        if (width > 0 && height > 0) {
            io.DisplaySize = ImVec2((float)width, (float)height);
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();
        UI::Render();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        EGLBoolean result = orig_eglSwapBuffers(dpy, surface);
        inHook = false;
        return result;
    }

    void SetupHooks(JavaVM* vm) {
        jvm = vm;

        void* eglSwapBuffers = DobbySymbolResolver("libEGL.so", "eglSwapBuffers");
        if (eglSwapBuffers) {
            DobbyHook(eglSwapBuffers, (void*)eglSwapBuffers_hk, (void**)&orig_eglSwapBuffers);
            LOGI("[*] Hooked eglSwapBuffers");
        } else {
            LOGE("[*] Failed to hook eglSwapBuffers");
        }

        registerNativeTouchMethod();
    }

    void Shutdown() {
        if (!initialized) return;
        ImGui_ImplOpenGL3_Shutdown();
        ImGui::DestroyContext();
        initialized = false;
    }

    bool IsInitialized() {
        return initialized;
    }
}
