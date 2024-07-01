#include "ColorSelectPopup.h"

bool __fastcall ColorSelectPopup::hook(CCLayer* self) {
    bool result = ColorSelectPopup::init(self);
    auto director = CCDirector::sharedDirector();
    auto size = director->getWinSize();
    auto menu = CCMenu::create();

    auto labeld = CCLabelBMFont::create("0", "goldFont.fnt");

    self->addChild(labeld);

    return result;
}