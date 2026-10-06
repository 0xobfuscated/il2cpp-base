#pragma once
#include "BNM/Class.hpp"
#include "BNM/Property.hpp"
#include "BNM/Method.hpp"
#include "BNM/Utils.hpp"
#include "../utils/log.h"
#include <utility>

namespace BNMHelper {
    template<typename T>
    class ClassWrapper {
    public:
        ClassWrapper(const char* ns, const char* name) : namespaceName(ns), className(name) {}

        bool init() {
            if (!getClass().IsValid()) {
                LOGE("[*] Failed to find %s.%s", namespaceName, className);
                return false;
            }
            return true;
        }

        T* getInstance() {
            BNM::AttachIl2Cpp();
            if (instance) return instance;

            BNM::Class cls = getClass();
            if (!cls.IsValid()) return nullptr;

            auto prop = cls.GetProperty("Instance");
            if (prop.IsValid()) {
                instance = BNM::Property<T*>(prop).Get();
                if (instance) return instance;
            }

            auto method = cls.GetMethod("get_Instance", 0);
            if (method.IsValid())
                instance = BNM::Method<T*>(method).Call();

            return instance;
        }

        bool isActive() const {
            return instance != nullptr;
        }

        template<typename... Args>
        void call(const char* method, Args&&... args) {
            BNM::AttachIl2Cpp();
            T* target = getInstance();
            if (!target) {
                LOGE("[*] %s.%s unavailable: instance not found", className, method);
                return;
            }

            BNM::Class cls = getClass();
            if (!cls.IsValid()) return;

            BNM::Method<void> typedMethod(cls.GetMethod(method, sizeof...(Args)));
            if (!typedMethod.IsValid()) {
                LOGE("[*] %s.%s unavailable: method not found", className, method);
                return;
            }

            typedMethod[target].Call(std::forward<Args>(args)...);
        }

        template<typename R, typename... Args>
        R callReturn(const char* method, Args&&... args) {
            BNM::AttachIl2Cpp();
            T* target = getInstance();
            BNM::Class cls = getClass();
            if (!target || !cls.IsValid()) return R{};

            auto m = cls.GetMethod(method, sizeof...(Args));
            if (!m.IsValid()) return R{};

            return BNM::Method<R>(m)[target].Call(std::forward<Args>(args)...);
        }

    private:
        BNM::Class getClass() const {
            return BNM::Class(namespaceName, className);
        }

        const char* namespaceName;
        const char* className;
        T* instance = nullptr;
    };
}
