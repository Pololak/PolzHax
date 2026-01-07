#include "FullscreenManager.h"
#include <gd.h>

const char* FullscreenManager::vanillaWindowedGameVar = "0025";

FullscreenMode FullscreenManager::_fullscreenMode = FullscreenMode::Windowed;
std::vector<HMONITOR> FullscreenManager::_monitors;

int FullscreenManager::_screen = -1;

void FullscreenManager::setFullscreenMode(FullscreenMode mode, UpdateMode update) {
	if (_fullscreenMode == mode && update != UpdateMode::Force) return;

	auto gm = gd::GameManager::sharedState();
	auto view = cocos2d::CCEGLView::sharedOpenGLView();
	if (!gm || !view) return;

	HWND windowHwnd = getWindowHwnd(view);

    switch (mode) {
    case FullscreenMode::Borderless: {
        CCCallFunc* callFunc = CCCallFunc::create(nullptr, callfunc_selector(FullscreenManager::setBorderlessFullscreenPreCallback));
        CCDelayTime* delayTime = CCDelayTime::create(0.f);
        CCSequence* sequence = CCSequence::create(delayTime, callFunc, delayTime);
        gm->getActionManager()->addAction((CCAction*)sequence, (CCNode*)gm, false);
        break;
    }
    }

    _fullscreenMode = mode;
}

void windowedModesShared(HWND windowHwnd, CCEGLView* view, int width, int height) {
    HMONITOR monitor = FullscreenManager::getMonitor();
    MONITORINFO monitorInfo;
    monitorInfo.cbSize = sizeof(MONITORINFO);
    if (!GetMonitorInfo(monitor, &monitorInfo)) {
        // TODO: use FLAlertLayer instead
        MessageBox(windowHwnd, "Failed getting monitor info!", "Screen Selector Error", MB_OK | MB_ICONERROR);
    }

    RECT monitorRect = monitorInfo.rcMonitor;

    if (width <= 0) width = monitorRect.right - monitorRect.left;
    if (height <= 0) height = monitorRect.bottom - monitorRect.top;
    SetWindowPos(windowHwnd, HWND_TOP, monitorRect.left, monitorRect.top, width, height, SWP_NOREDRAW | SWP_NOACTIVATE | SWP_FRAMECHANGED);
    view->resizeWindow(width, height);
}

void FullscreenManager::setBorderlessFullscreenPreCallback() {
    auto gm = gd::GameManager::sharedState();
    CCCallFunc* callFunc = CCCallFunc::create(nullptr, callfunc_selector(FullscreenManager::setBorderlessFullscreenCallback));
    CCDelayTime* delayTime = CCDelayTime::create(0.f);
    CCSequence* sequence = CCSequence::create(delayTime, callFunc, delayTime);
    gm->getActionManager()->addAction((CCAction*)sequence, (CCNode*)gm, false);
}
void FullscreenManager::setBorderlessFullscreenCallback() { FullscreenManager::setBorderlessFullscreen(); }
void FullscreenManager::setBorderlessFullscreen() {
    auto gm = gd::GameManager::sharedState();
    auto view = CCEGLView::sharedOpenGLView();
    if (!gm || !view) return;
    bool currentWindowed = gm->getGameVariable(vanillaWindowedGameVar);
    if (!currentWindowed) return;

    HWND windowHwnd = getWindowHwnd(view);

    LONG windowStyle = GetWindowLong(windowHwnd, GWL_STYLE);
    SetWindowLong(windowHwnd, GWL_STYLE, windowStyle & ~(WS_CAPTION | WS_SIZEBOX | WS_SYSMENU));

    windowedModesShared(windowHwnd, view, 0, 0);
}
HMONITOR FullscreenManager::getMonitor() {
    return _monitors.at(_screen);
}