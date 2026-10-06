LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := imgui

LOCAL_CPPFLAGS += -std=c++17 -DIMGUI_IMPL_OPENGL_ES3

LOCAL_C_INCLUDES := \
    $(LOCAL_PATH) \
    $(LOCAL_PATH)/backends

LOCAL_SRC_FILES := \
    imgui.cpp \
    imgui_demo.cpp \
    imgui_draw.cpp \
    imgui_tables.cpp \
    imgui_widgets.cpp \
    backends/imgui_impl_android.cpp \
    backends/imgui_impl_opengl3.cpp

include $(BUILD_STATIC_LIBRARY)
