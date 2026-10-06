#include "gui.h"
#include "Theme.h"
#include "../game/DebugController.h"

namespace UI {
    void Init() {
        Theme::LoadFonts();
        Theme::ApplyStyle();
    }

    void Render() {
        ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(900, 700), ImGuiCond_Always);

        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoResize;

        ImGui::Begin("Castle Busters", nullptr, windowFlags);

        ImGui::Spacing();
        ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "Game State:");
        ImGui::SameLine();
        if (game::DebugController::IsActive()) {
            ImGui::TextColored(ImVec4(0.4f, 1.0f, 0.4f, 1.0f), "CONNECTED");
        } else {
            ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "DISCONNECTED");
        }
        ImGui::SameLine(ImGui::GetWindowWidth() - 80);
        ImGui::TextDisabled("v0.0.1");
        ImGui::Spacing();
        ImGui::Separator();

        if (ImGui::CollapsingHeader("Resources")) {
            ImGui::Spacing();

            ImGui::Columns(2, nullptr, false);

            ImGui::Text("Wood");
            ImGui::Spacing();

            if (ImGui::Button("Add 100", ImVec2(200, 70))) {
                game::DebugController::AddWood(100);
            }
            if (ImGui::Button("Add 500", ImVec2(200, 70))) {
                game::DebugController::AddWood(500);
            }
            if (ImGui::Button("Add 1K", ImVec2(200, 70))) {
                game::DebugController::AddWood(1000);
            }
            if (ImGui::Button("Add 10K##wood", ImVec2(200, 70))) {
                game::DebugController::AddWood(10000);
            }

            ImGui::NextColumn();

            ImGui::Text("Coins");
            ImGui::Spacing();

            if (ImGui::Button("Add 10K##coins", ImVec2(200, 70))) {
                game::DebugController::AddMoney(10000);
            }
            if (ImGui::Button("Add 100K", ImVec2(200, 70))) {
                game::DebugController::AddMoney(100000);
            }

            ImGui::Columns(1);
        }

        ImGui::Spacing();
        ImGui::Separator();

        if (ImGui::CollapsingHeader("Experience")) {
            ImGui::Spacing();

            if (ImGui::Button("Add 100 Forge Exp", ImVec2(400, 70))) {
                game::DebugController::AddForgeExp(100);
            }
        }

        ImGui::Spacing();
        ImGui::Separator();

        if (ImGui::CollapsingHeader("General")) {
            ImGui::Spacing();

            if (ImGui::Button("Unlock Everything", ImVec2(400, 70))) {
                game::DebugController::UnlockEverything();
            }
            ImGui::SameLine();
            if (ImGui::Button("Toggle SpeedUp", ImVec2(400, 70))) {
                game::DebugController::ToggleSpeedUp();
            }
        }

        ImGui::End();
    }
}
