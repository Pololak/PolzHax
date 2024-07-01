#include "PauseLayer.h"
#include <MinHook.h>
#include <cocos2d.h>
#include "Windows.h"

bool noclip = false;
bool zerodelay = false;

DWORD cocosbase = (DWORD)GetModuleHandleA("libcocos2d.dll");
const HINSTANCE cocos = GetModuleHandleA("libcocos2d.dll");

CCSprite* toggle_texture(CCSprite* toggleOn, CCSprite* toggleOff) {
    return (noclip) ? toggleOn : toggleOff;
}

void PauseLayer::noclip_button(CCObject*) {
    noclip = !noclip;
    if (noclip) {
        WriteProcessMemory(
            GetCurrentProcess(),
            reinterpret_cast<void*>(gd::base + 0xF04E9),
            "\xe9\xf0\x02\x00\x00\x90",
            6,
            NULL
        );
    }
    else {
        WriteProcessMemory(
            GetCurrentProcess(),
            reinterpret_cast<void*>(gd::base + 0xF04E9),
            "\x0f\x85\xef\x02\x00\x00",
            6,
            NULL
        );
    }
}


bool __fastcall PauseLayer::hook(CCLayer* self) {
    bool result = PauseLayer::init(self);
    auto director = CCDirector::sharedDirector();
    auto size = director->getWinSize();

    auto toggleOn = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
    auto toggleOff = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
    toggleOff->setOpacity(200);

    auto noclip_button = gd::CCMenuItemToggler::create(
        toggle_texture(toggleOn, toggleOff),
        toggle_texture(toggleOff, toggleOn),
        self,
        menu_selector(PauseLayer::noclip_button));

    auto menu2 = CCMenu::create();

    auto SettingsSprite = CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png");
    auto SettingsButton = gd::CCMenuItemSpriteExtra::create(SettingsSprite, nullptr, self, menu_selector(gd::MenuLayer::onOptions));

    menu2->setPosition({ size.width - 150, size.height - 100 });
    menu2->setScale(0.6f);

    menu2->addChild(SettingsButton);

    self->addChild(menu2);

   

    auto noclabel = CCLabelBMFont::create("Noclip", "bigFont.fnt");
    noclip_button->setScale(0.8F);
    noclip_button->setPositionX(10);
    noclabel->setScale(0.5F);
    noclabel->setPositionX(60);
    noclabel->setPositionY(1);

    auto menu = CCMenu::create();
    menu->setPosition({ 25, size.height - 50 });
    menu->addChild(noclip_button);
    menu->addChild(noclabel);
    
  

    self->addChild(menu);
    return result;
}