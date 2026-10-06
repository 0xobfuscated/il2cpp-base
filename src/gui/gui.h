#ifndef GUI_H
#define GUI_H

#include "imgui.h"
#include "imgui_impl_opengl3.h"

namespace UI {
    extern bool show_window;

    void Init();
    void Render();
}

#endif // GUI_H
