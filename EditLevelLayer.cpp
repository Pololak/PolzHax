#include "EditLevelLayer.h"

bool __fastcall EditLevelLayer::EditLevelLayer_init_H(gd::EditLevelLayer* self, void*, gd::GJGameLevel* gjlvl) {

    if (!EditLevelLayer::EditLevelLayer_init(self, gjlvl)) return false;

    auto director = CCDirector::sharedDirector();
    auto size = director->getWinSize();
    auto menu = CCMenu::create();

    //auto zhigasprite = CCSprite::createWithSpriteFrameName("GJ_garageBtn_001.png");
    //auto zhiga = gd::CCMenuItemSpriteExtra::create(zhigasprite, self, nullptr, 0);

    auto WaveButton = CCSprite::createWithSpriteFrameName("gj_dartBtn_off_001.png");

    self->addChild(WaveButton);

    //menu->setPosition(size.width / 40, size.height / 2);
    //menu->addChild(zhiga);
    //self->addChild(zhiga);

    return true;
}