// https://github.com/cgytrus/ScreenSelector/tree/main
#pragma once

#include <windows.h>
#include <memory>
#include <cocos2d.h>
using namespace cocos2d;

enum class UpdateMode { None, OnChange, Force };
enum class FullscreenMode { Windowed, Exclusive, Borderless };

class FullscreenManager : public cocos2d::CCObject {
private:
	FullscreenManager() {}

	static int _screen;
	static FullscreenMode _fullscreenMode;

	static std::vector<HMONITOR> _monitors;

	void setBorderlessFullscreenPreCallback();
	void setBorderlessFullscreenCallback();

public:
	static const char* vanillaWindowedGameVar;

	static void setFullscreenMode(FullscreenMode, UpdateMode = UpdateMode::OnChange);
	static void setBorderlessFullscreen();

	static HMONITOR getMonitor();

	static inline HWND getWindowHwnd(GLFWwindow* window) { return *(HWND*)((uintptr_t)window + 0x22c); }
	static inline HWND getWindowHwnd(CCEGLView* view) { return getWindowHwnd(view->getWindow()); }
	static inline CCSize getWindowedSize(CCEGLView* view) { return *(CCSize*)((uintptr_t)view + 0xa0); }
};
