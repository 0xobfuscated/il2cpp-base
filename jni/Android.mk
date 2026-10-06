LOCAL_PATH := $(call my-dir)
PROJECT_ROOT := $(abspath $(LOCAL_PATH)/..)

include $(PROJECT_ROOT)/ext/BNM-Android/Android.mk
include $(PROJECT_ROOT)/ext/Dobby/Android.mk
include $(PROJECT_ROOT)/ext/imgui/Android.mk

include $(CLEAR_VARS)

LOCAL_MODULE := libreversed

LOCAL_CPPFLAGS += -std=c++20

LOCAL_C_INCLUDES := \
    $(PROJECT_ROOT)/src \
    $(PROJECT_ROOT)/ext/BNM-Android/include \
    $(PROJECT_ROOT)/ext/BNM-Android/external \
    $(PROJECT_ROOT)/ext/Dobby/include \
    $(PROJECT_ROOT)/ext/imgui \
    $(PROJECT_ROOT)/ext/imgui/backends

LOCAL_SRC_FILES := \
    $(PROJECT_ROOT)/src/main.cpp \
    $(PROJECT_ROOT)/src/game/DebugController.cpp \
    $(PROJECT_ROOT)/src/gui/gui.cpp \
    $(PROJECT_ROOT)/src/gui/ImGuiSetup.cpp \
    $(PROJECT_ROOT)/src/gui/Theme.cpp \
    $(PROJECT_ROOT)/src/utils/Tools.cpp

LOCAL_STATIC_LIBRARIES := BNM dobby imgui

LOCAL_LDLIBS := -lGLESv3 -lEGL -llog -landroid

include $(BUILD_SHARED_LIBRARY)
