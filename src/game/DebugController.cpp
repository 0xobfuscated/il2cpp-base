#include "DebugController.h"
#include "../utils/BNMHelper.h"

namespace game {
    namespace DebugController {
        static BNMHelper::ClassWrapper<BNM::IL2CPP::Il2CppObject> controller("CastleClashers.Core", "DebugController");

        void Init() {
            LOGI("[*] Initializing DebugController");
            bool inited = controller.init();
            if (!inited) {
                LOGE("[*] DebugController init failed");
                return;
            }
            LOGI("[*] DebugController initialized");
        }

        BNM::IL2CPP::Il2CppObject* GetInstance() {
            return controller.getInstance();
        }

        bool IsActive() {
            return controller.isActive();
        }

        void AddWood(int amount) {
            controller.call("AddWood", amount);
        }

        void AddMoney(int amount) {
            controller.call("AddMoney", amount);
        }

        void AddForgeExp(int amount) {
            controller.call("AddForgeExp", amount);
        }

        void UnlockEverything() {
            controller.call("UnlockEverything");
        }

        void ToggleSpeedUp() {
            controller.call("ToggleSpeedUp");
        }
    }
}
