#ifndef GAME_DEBUG_CONTROLLER_H
#define GAME_DEBUG_CONTROLLER_H

#include "BNM/Class.hpp"

namespace game {
    namespace DebugController {
        void Init();
        BNM::IL2CPP::Il2CppObject* GetInstance();
        bool IsActive();

        void AddWood(int amount);
        void AddMoney(int amount);
        void AddForgeExp(int amount);
        void UnlockEverything();
        void ToggleSpeedUp();
    }
}

#endif // GAME_DEBUG_CONTROLLER_H
