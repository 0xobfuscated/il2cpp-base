#include "Theme.h"
#include <stdio.h>

namespace Theme {
    static ImFont* MainFont = nullptr;

    void LoadFonts() {
        ImGuiIO& io = ImGui::GetIO();
        
        ImFontConfig config;
        config.OversampleH = 2;
        config.OversampleV = 2;
        config.PixelSnapH = false;
        
        const char* fontPath = "/system/fonts/Roboto-Regular.ttf";
        FILE* f = fopen(fontPath, "rb");
        if (f) {
            fclose(f);
            MainFont = io.Fonts->AddFontFromFileTTF(fontPath, 28.0f, &config);
        } else {
            config.SizePixels = 28.0f;
            MainFont = io.Fonts->AddFontDefault(&config);
        }

        io.FontDefault = MainFont;
        io.FontGlobalScale = 1.0f;
    }

    void ApplyStyle() {
        ImGui::StyleColorsDark();

        ImGuiStyle& style = ImGui::GetStyle();

        style.WindowMenuButtonPosition = ImGuiDir_Left;
        style.WindowTitleAlign = ImVec2(0.0f, 0.5f);

        style.WindowRounding = 8.0f;
        style.FrameRounding = 6.0f;
        style.PopupRounding = 6.0f;
        style.ScrollbarRounding = 6.0f;
        style.GrabRounding = 6.0f;
        style.GrabMinSize = 12.0f;

        style.WindowPadding = ImVec2(10.0f, 15.0f);
        style.FramePadding = ImVec2(12.0f, 8.0f);
        style.ItemSpacing = ImVec2(10.0f, 8.0f);

        style.ScaleAllSizes(1.5f);
        
        ImVec4* colors = style.Colors;
        
        colors[ImGuiCol_WindowBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.98f);
        colors[ImGuiCol_ChildBg] = ImVec4(0.01f, 0.01f, 0.01f, 0.50f);
        colors[ImGuiCol_PopupBg] = ImVec4(0.04f, 0.04f, 0.04f, 0.98f);
        
        colors[ImGuiCol_Border] = ImVec4(0.08f, 0.08f, 0.08f, 0.50f);
        colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        
        colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
        colors[ImGuiCol_TextSelectedBg] = ImVec4(0.80f, 0.10f, 0.10f, 0.40f);
        
        colors[ImGuiCol_Header] = ImVec4(0.05f, 0.05f, 0.05f, 0.60f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.10f, 0.05f, 0.05f, 0.80f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.15f, 0.05f, 0.05f, 1.00f);
        
        colors[ImGuiCol_TitleBg] = ImVec4(0.03f, 0.03f, 0.03f, 1.00f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.05f, 0.03f, 0.03f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.02f, 0.02f, 0.02f, 0.75f);
        
        colors[ImGuiCol_MenuBarBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
        
        colors[ImGuiCol_Button] = ImVec4(0.80f, 0.10f, 0.10f, 0.85f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.90f, 0.15f, 0.15f, 1.00f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.70f, 0.08f, 0.08f, 1.00f);
        
        colors[ImGuiCol_FrameBg] = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.12f, 0.08f, 0.08f, 1.00f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.15f, 0.08f, 0.08f, 1.00f);
        
        colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.15f, 0.15f, 1.00f);
        colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.15f, 0.15f, 1.00f);
        
        colors[ImGuiCol_SliderGrab] = ImVec4(0.90f, 0.15f, 0.15f, 1.00f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(1.00f, 0.20f, 0.20f, 1.00f);
        
        colors[ImGuiCol_Tab] = ImVec4(0.08f, 0.08f, 0.08f, 0.80f);
        colors[ImGuiCol_TabHovered] = ImVec4(0.12f, 0.08f, 0.08f, 0.90f);
        colors[ImGuiCol_TabActive] = ImVec4(0.80f, 0.10f, 0.10f, 1.00f);
        colors[ImGuiCol_TabUnfocused] = ImVec4(0.06f, 0.06f, 0.06f, 0.60f);
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.10f, 0.06f, 0.06f, 0.80f);
        
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.50f);
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.10f, 0.08f, 0.08f, 0.50f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.15f, 0.10f, 0.10f, 0.80f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.20f, 0.12f, 0.12f, 1.00f);
        
        colors[ImGuiCol_ResizeGrip] = ImVec4(0.12f, 0.08f, 0.08f, 0.20f);
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.18f, 0.12f, 0.12f, 0.70f);
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.25f, 0.15f, 0.15f, 1.00f);
        
        colors[ImGuiCol_Separator] = ImVec4(0.10f, 0.08f, 0.08f, 0.50f);
        colors[ImGuiCol_SeparatorHovered] = ImVec4(0.18f, 0.12f, 0.12f, 0.70f);
        colors[ImGuiCol_SeparatorActive] = ImVec4(0.25f, 0.15f, 0.15f, 1.00f);
        
        colors[ImGuiCol_PlotLines] = ImVec4(0.90f, 0.15f, 0.15f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.20f, 0.20f, 1.00f);
        colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.15f, 0.15f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.20f, 0.20f, 1.00f);
        
        colors[ImGuiCol_DragDropTarget] = ImVec4(0.90f, 0.15f, 0.15f, 0.90f);
        
        colors[ImGuiCol_NavHighlight] = ImVec4(0.90f, 0.15f, 0.15f, 0.50f);
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 0.20f, 0.20f, 0.50f);
        colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.60f);
    }
}
