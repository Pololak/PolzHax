#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <cocos2d.h>
#include "imgui-hook.hpp"

using namespace cocos2d;

void _stub() {}
std::function<void()> g_drawFunc = _stub;
std::function<void()> g_initFunc = _stub;
std::function<void()> g_toggleCallback = _stub;

void ImGuiHook::setRenderFunction(std::function<void()> func) {
    g_drawFunc = func;
}

void ImGuiHook::setInitFunction(std::function<void()> func) {
    g_initFunc = func;
}

void ImGuiHook::setToggleCallback(std::function<void()> func) {
    g_toggleCallback = func;
}

bool g_inited = false;

HWND windowToHWND(GLFWwindow* window) {
    return WindowFromDC(*reinterpret_cast<HDC*>(reinterpret_cast<uintptr_t>(window) + 0x244));
}

void (__thiscall* CCEGLView_swapBuffers)(CCEGLView*);
void __fastcall CCEGLView_swapBuffers_H(CCEGLView* self) {
    auto window = self->getWindow();

    if (!g_inited) {
        g_inited = true;
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::GetIO();
        auto hwnd = windowToHWND(window);
        ImGui_ImplWin32_Init(hwnd);
        ImGui_ImplOpenGL3_Init();
        g_initFunc();
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    g_drawFunc();

    ImGui::EndFrame();
    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
    // glFlush();

    CCEGLView_swapBuffers(self);
}

// why is this an extern
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void (__thiscall* CCEGLView_pollEvents)(CCEGLView*);
void __fastcall CCEGLView_pollEvents_H(CCEGLView* self) {
    auto& io = ImGui::GetIO();

    bool blockInput = false;
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);

        if (io.WantCaptureMouse) {
            switch (msg.message) {
                case WM_LBUTTONDBLCLK:
                case WM_LBUTTONDOWN:
                case WM_LBUTTONUP:
                case WM_MBUTTONDBLCLK:
                case WM_MBUTTONDOWN:
                case WM_MBUTTONUP:
                case WM_MOUSEACTIVATE:
                case WM_MOUSEHOVER:
                case WM_MOUSEHWHEEL:
                case WM_MOUSELEAVE:
                case WM_MOUSEMOVE:
                case WM_MOUSEWHEEL:
                case WM_NCLBUTTONDBLCLK:
                case WM_NCLBUTTONDOWN:
                case WM_NCLBUTTONUP:
                case WM_NCMBUTTONDBLCLK:
                case WM_NCMBUTTONDOWN:
                case WM_NCMBUTTONUP:
                case WM_NCMOUSEHOVER:
                case WM_NCMOUSELEAVE:
                case WM_NCMOUSEMOVE:
                case WM_NCRBUTTONDBLCLK:
                case WM_NCRBUTTONDOWN:
                case WM_NCRBUTTONUP:
                case WM_NCXBUTTONDBLCLK:
                case WM_NCXBUTTONDOWN:
                case WM_NCXBUTTONUP:
                case WM_RBUTTONDBLCLK:
                case WM_RBUTTONDOWN:
                case WM_RBUTTONUP:
                case WM_XBUTTONDBLCLK:
                case WM_XBUTTONDOWN:
                case WM_XBUTTONUP:
                    blockInput = true;
            }
        }

        if (io.WantCaptureKeyboard) {
            switch (msg.message) {
                case WM_HOTKEY:
                case WM_KEYDOWN:
                case WM_KEYUP:
                case WM_SYSKEYDOWN:
                case WM_SYSKEYUP:
                    blockInput = true;
            }
        } else if (msg.message == WM_KEYDOWN && (msg.wParam == VK_F1 || msg.wParam == VK_OEM_3 ||msg.wParam == VK_TAB)) {
            // std::cout << "key is " << std::hex << static_cast<unsigned>(msg.wParam) << std::endl;
            g_toggleCallback();
        }

        if (!blockInput)
            DispatchMessage(&msg);

        ImGui_ImplWin32_WndProcHandler(msg.hwnd, msg.message, msg.wParam, msg.lParam);
    }

    CCEGLView_pollEvents(self);
}

void ImGuiHook::poll(CCEGLView* self) {
    CCEGLView_pollEvents_H(self);
}

void (__thiscall* CCEGLView_toggleFullScreen)(cocos2d::CCEGLView*, bool);
void __fastcall CCEGLView_toggleFullScreen_H(cocos2d::CCEGLView* self, void*, bool toggle) {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CCEGLView_toggleFullScreen(self, toggle);

    g_inited = false;
    // ImGui::CreateContext();
    //ImGui::GetIO();
    // auto hwnd = windowToHWND(self->getWindow());
    // ImGui_ImplWin32_Init(hwnd);
    // ImGui_ImplOpenGL3_Init();
}

void (__thiscall* AppDelegate_applicationWillEnterForeground)(void*);
void __fastcall AppDelegate_applicationWillEnterForeground_H(void* self) {
    AppDelegate_applicationWillEnterForeground(self);
    ImGui::GetIO().ClearInputKeys();
}

void ImGuiHook::setupHooks(std::function<void(void*, void*, void**)> hookFunc) {
    auto cocosBase = GetModuleHandleA("libcocos2d.dll");
    hookFunc(
        reinterpret_cast<void*>(GetProcAddress(cocosBase, "?swapBuffers@CCEGLView@cocos2d@@UAEXXZ")),
        reinterpret_cast<void*>(&CCEGLView_swapBuffers_H),
        reinterpret_cast<void**>(&CCEGLView_swapBuffers)
    );
    hookFunc(
        reinterpret_cast<void*>(GetProcAddress(cocosBase, "?pollEvents@CCEGLView@cocos2d@@QAEXXZ")),
        reinterpret_cast<void*>(&CCEGLView_pollEvents_H),
        reinterpret_cast<void**>(&CCEGLView_pollEvents)
    );
    hookFunc(
        reinterpret_cast<void*>(GetProcAddress(cocosBase, "?toggleFullScreen@CCEGLView@cocos2d@@QAEX_N@Z")),
        reinterpret_cast<void*>(&CCEGLView_toggleFullScreen_H),
        reinterpret_cast<void**>(&CCEGLView_toggleFullScreen)
    );
    hookFunc(
        reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(GetModuleHandleA(0)) + 0x28f00),
        reinterpret_cast<void*>(&AppDelegate_applicationWillEnterForeground_H),
        reinterpret_cast<void**>(&AppDelegate_applicationWillEnterForeground)
    );
}