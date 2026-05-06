#include <Geode/Geode.hpp>

#include <Geode/modify/AppDelegate.hpp>
#include <Geode/modify/CCKeyboardDispatcher.hpp>
#include <Geode/modify/GameManager.hpp>

// Utils
#include "patching.hpp"

// Menu
#include "Menu.hpp"
#include "Setting.hpp"

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

class $modify(GameManager) { // I need this because Zmx added custom icons lol.
    bool isIconUnlocked(int p0, IconType p1) {
        return setting().onIcons ? true : GameManager::isIconUnlocked(p0, p1);
    }

    bool isColorUnlocked(int p0, bool p1) {
        return setting().onIcons ? true : GameManager::isColorUnlocked(p0, p1);
    }
};

class $modify(CCKeyboardDispatcher) {
    bool dispatchKeyboardMSG(enumKeyCodes key, bool down) {
        bool ret = CCKeyboardDispatcher::dispatchKeyboardMSG(key, down);

        if (down) {

        }

        return ret;
    }
};

$on_mod(Loaded) {
    sequence_patch(geode::base::get() + 0x28bd5, { 0x6a, 0x00 }); // RGBA8888 format.
    sequence_patch(geode::base::get() + 0x3a49b, { 0xb8, 0x01, 0x00, 0x00, 0x00, 0x90, 0x90 }); // Play Music Button.
    //sequence_patch(geode::base::get() + 0x145128, { 0x42, 0x61, 0x72, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }); // Progress Bar -> Bar
    sequence_patch(geode::base::get() + 0x3a669, { 0x00, 0x00, 0x00, 0x43 }); // CustomSongWidget m_artistLabel->limitLabelWidth(120.f, ...)
    //sequence_patch(geode::base::get() + 0x3e9e1, { 0x00, 0x00, 0x8c, 0x42 }); // EditorPauseLayer keysButton y pos 70

    setupImGuiMenu();
}