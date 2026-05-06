#include <Geode/Geode.hpp>

#include <Geode/modify/AppDelegate.hpp>
#include <Geode/modify/CCKeyboardDispatcher.hpp>

#include "Setting.hpp"

#include <imgui-cocos.hpp>

using namespace geode::prelude;

class $modify(AppDelegate) {
    void applicationDidEnterBackground() {
        AppDelegate::applicationDidEnterBackground();
        if (setting().onAutoSave) {
            setting().save();
        }
    }

    void applicationWillEnterForeground() {
        AppDelegate::applicationWillEnterForeground();
        if (setting().onAutoSave) {
            setting().save();
        }
    }
};

class $modify(CCKeyboardDispatcher) {
    bool dispatchKeyboardMSG(enumKeyCodes key, bool down) {
        bool ret = CCKeyboardDispatcher::dispatchKeyboardMSG(key, down);

        if (down) {
            if (key == VK_TAB || key == VK_F1 || key == VK_OEM_3 || key == setting().m_openMenuKey) {
                ImGuiCocos::get().toggle();
                setting().show = ImGuiCocos::get().isVisible();
            }
        }

        return ret;
    }
};