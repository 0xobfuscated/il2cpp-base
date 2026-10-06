#ifndef LOG_H
#define LOG_H

#include <android/log.h>

#define ENABLE_LOGGING 1

#if ENABLE_LOGGING
#define LOG_TAG "reversed"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

#else
#define LOGE(...)
#define LOGW(...)
#define LOGI(...)
#define LOGD(...)
#endif

#endif
