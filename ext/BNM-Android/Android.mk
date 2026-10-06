LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := BNM

LOCAL_CPPFLAGS += -std=c++20

LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/include \
    $(LOCAL_PATH)/external/include \
    $(LOCAL_PATH)/external \
    $(LOCAL_PATH)/external/utf8 \
    $(LOCAL_PATH)/src/private \
    $(LOCAL_PATH)/../Dobby/include

LOCAL_SRC_FILES := \
    src/Class.cpp \
    src/ClassesManagement.cpp \
    src/Coroutine.cpp \
    src/Delegates.cpp \
    src/Defaults.cpp \
    src/EventBase.cpp \
    src/Exceptions.cpp \
    src/FieldBase.cpp \
    src/Hooks.cpp \
    src/Image.cpp \
    src/Internals.cpp \
    src/Loading.cpp \
    src/MethodBase.cpp \
    src/MonoStructures.cpp \
    src/PropertyBase.cpp \
    src/UnityStructures.cpp \
    src/Utils.cpp

include $(BUILD_STATIC_LIBRARY)
