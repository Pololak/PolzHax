#include "CCEGLViewHook.hpp"
#include "Setting.hpp"
#include "imgui-hook.hpp"
#include "Menu.hpp"

void __fastcall CCEGLViewHook::setupWindowH(cocos2d::CCEGLView* self, void*, cocos2d::CCRect rect) {
    //int count;
    //auto monitor = reinterpret_cast<GLFWmonitor * *(__cdecl*)(int*)>(reinterpret_cast<uintptr_t>(GetModuleHandleA("libcocos2d.dll")) + 0x110b20)(&count)[0];
    //auto vidmode = reinterpret_cast<GLFWvidmode * (__cdecl*)(GLFWmonitor*)>(reinterpret_cast<uintptr_t>(GetModuleHandleA("libcocos2d.dll")) + 0x10c900)(monitor);

    //reinterpret_cast<void(__cdecl*)(int, int)>(reinterpret_cast<uintptr_t>(GetModuleHandleA("libcocos2d.dll")) + 0x10d2f0)(GLFW_RED_BITS, vidmode->redBits);
    //reinterpret_cast<void(__cdecl*)(int, int)>(reinterpret_cast<uintptr_t>(GetModuleHandleA("libcocos2d.dll")) + 0x10d2f0)(GLFW_GREEN_BITS, vidmode->greenBits);
    //reinterpret_cast<void(__cdecl*)(int, int)>(reinterpret_cast<uintptr_t>(GetModuleHandleA("libcocos2d.dll")) + 0x10d2f0)(GLFW_BLUE_BITS, vidmode->blueBits);
    //reinterpret_cast<void(__cdecl*)(int, int)>(reinterpret_cast<uintptr_t>(GetModuleHandleA("libcocos2d.dll")) + 0x10d2f0)(GLFW_REFRESH_RATE, vidmode->refreshRate);
    //// reinterpret_cast<void(__cdecl*)(int, int)>(reinterpret_cast<uintptr_t>(GetModuleHandleA("libcocos2d.dll")) + 0x10d2f0)(GLFW_FLOATING, setting().onBorderlessFullscreen ? 1 : 0);
    //reinterpret_cast<void(__cdecl*)(int, int)>(reinterpret_cast<uintptr_t>(GetModuleHandleA("libcocos2d.dll")) + 0x10d2f0)(GLFW_DECORATED, 0);

    CCEGLViewHook::setupWindow(self, rect);
}   

void __fastcall CCEGLViewHook::toggleFullScreenH(cocos2d::CCEGLView* self, void*, bool fullscreen) {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CCEGLViewHook::toggleFullScreen(self, fullscreen);

    ImGuiHook::setInited(false);
    PolzHax::updateFPSBypass();
}

inline void(__cdecl* centerWindowOrSmth)(GLFWwindow*);
void __cdecl centerWindowOrSmthH(GLFWwindow* window) {
    if (!window) return;
    RECT windowRect;
    GetWindowRect(WindowFromDC(*reinterpret_cast<HDC*>(reinterpret_cast<uintptr_t>(window) + 0x244)), &windowRect);
    int monitorCount;
    auto monitor = reinterpret_cast<GLFWmonitor * *(__cdecl*)(int*)>(reinterpret_cast<uintptr_t>(GetModuleHandleA("libcocos2d.dll")) + 0x110b20)(&monitorCount)[0];
    auto vidmode = reinterpret_cast<GLFWvidmode * (__cdecl*)(GLFWmonitor*)>(reinterpret_cast<uintptr_t>(GetModuleHandleA("libcocos2d.dll")) + 0x10c900)(monitor);
    int x = 0;
    int y = 0;
    reinterpret_cast<void(__cdecl*)(GLFWmonitor*, int*, int*)>(reinterpret_cast<uintptr_t>(GetModuleHandleA("libcocos2d.dll")) + 0x110aa0)(monitor, &x, &y);
    x += vidmode->width / 2 - (windowRect.right - windowRect.left) / 2;
    y += vidmode->height / 2 - (windowRect.bottom - windowRect.top) / 2;
    SetWindowPos(WindowFromDC(*reinterpret_cast<HDC*>(reinterpret_cast<uintptr_t>(window) + 0x244)), HWND_TOP, x, y, 0, 0, SWP_NOREDRAW | SWP_NOACTIVATE | SWP_FRAMECHANGED | SWP_NOSIZE);
}

void CCEGLViewHook::mem_init() {
    //MH_CreateHook(reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(GetModuleHandleA("libcocos2d.dll")) + 0xc1d10), CCEGLViewHook::setupWindowH, reinterpret_cast<void**>(&CCEGLViewHook::setupWindow));
    MH_CreateHook(reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(GetModuleHandleA("libcocos2d.dll")) + 0xc1ef0), CCEGLViewHook::toggleFullScreenH, reinterpret_cast<void**>(&CCEGLViewHook::toggleFullScreen));
    //MH_CreateHook(reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(GetModuleHandleA("libcocos2d.dll")) + 0x10db80), centerWindowOrSmthH, reinterpret_cast<void**>(&centerWindowOrSmth));
}