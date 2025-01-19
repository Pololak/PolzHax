#pragma once
#include <imgui.h>
#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_opengl3.h>
#include <functional>
#include <cocos2d.h>

namespace ImGuiHook {
    void setupHooks(std::function<void(void*, void*, void**)> hookFunc);

    void setRenderFunction(std::function<void()> func);
    void setToggleCallback(std::function<void()> func);
    void setInitFunction(std::function<void()> func);
    void poll(cocos2d::CCEGLView* self);
}
