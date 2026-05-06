#pragma once
#include <imgui.h>
#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_opengl3.h>
#include <functional>
#include <Geode/Geode.hpp>

namespace ImGuiHook {
    void setRenderFunction(std::function<void()> func);
    void setToggleCallback(std::function<void()> func);
    void setInitFunction(std::function<void()> func);

    void poll(cocos2d::CCEGLView* self);
}
