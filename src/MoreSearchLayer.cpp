#include "MoreSearchLayer.hpp"
#include "Setting.hpp"

void PolzMoreSearchLayer::onExtraLong(CCObject*) {
    setting().onExtraLong = !setting().onExtraLong;
}

bool PolzMoreSearchLayer::init() {
    if (!MoreSearchLayer::init()) return false;

    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    this->createToggleButton("Extra-Long", menu_selector(PolzMoreSearchLayer::onExtraLong), !setting().onExtraLong, this->m_buttonMenu, ccp(winSize.width / 2.f + 60.f, winSize.height / 2.f - 80.f));

    return true;
}