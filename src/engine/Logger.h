#pragma once

#include "imgui/imgui.h"

struct Logger {
    ImGuiTextBuffer     Buf;
    ImGuiTextFilter     Filter;
    ImVector<int>       LineOffsets; // Index to lines offset. We maintain this with AddLog() calls.
    bool                AutoScroll;  // Keep scrolling if already at the bottom.

    Logger();

    void clear();
    void addLog(const char* fmt, ...) IM_FMTARGS(2);
    void draw(const char* title, bool* p_open = (bool*)0);
};
