#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <Geode/Geode.hpp>
#include <Geode/modify/CCEGLView.hpp>
#include <Geode/modify/AppDelegate.hpp>

#include "imgui-hook.hpp"
#include "Menu.hpp"
#include "Setting.hpp"
#include "PauseLayer.hpp"

using namespace geode::prelude;

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

// why is this an extern
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

class $modify(ImGuiCCEGLView, CCEGLView) {
    void swapBuffers() {
        auto window = this->getWindow();

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

        CCEGLView::swapBuffers();
    }

    void pollEvents() {
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
            }
            else if (msg.message == WM_KEYDOWN && (msg.wParam == VK_F1 || msg.wParam == VK_OEM_3 || msg.wParam == VK_TAB || msg.wParam == setting().m_openMenuKey)) {
                g_toggleCallback();
                if (!GameManager::sharedState()->getGameVariable("0024") && GameManager::sharedState()->m_playLayer && (PolzPauseLayer::get() == nullptr)) {
                    CCEGLView::sharedOpenGLView()->showCursor(setting().show);
                }
            }

            if (!blockInput)
                DispatchMessage(&msg);

            ImGui_ImplWin32_WndProcHandler(msg.hwnd, msg.message, msg.wParam, msg.lParam);
        }

        CCEGLView::pollEvents();
    }

    void toggleFullScreen(bool toggle) {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();

        CCEGLView::toggleFullScreen(toggle);

        g_inited = false;

        PolzHax::updateFPSBypass();
    }
};

class $modify(AppDelegate) {
    void applicationWillEnterForeground() {
        AppDelegate::applicationWillEnterForeground();
        ImGui::GetIO().ClearInputKeys();
    }
};

void ImGuiHook::poll(CCEGLView* self) {
    static_cast<ImGuiCCEGLView*>(self)->pollEvents();
}