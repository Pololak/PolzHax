#include "MoreSearchLayer.hpp"
#include "Setting.hpp"

void MoreSearchLayer::Callback::onExtraLong(CCObject*) {
	setting().onExtraLong = !setting().onExtraLong;
}

bool __fastcall MoreSearchLayer::initH(gd::MoreSearchLayer* self) {
	if (!MoreSearchLayer::init(self)) return false;

	auto director = CCDirector::sharedDirector();
	auto winSize = director->getWinSize();

	self->createToggleButton("Extra-Long", menu_selector(MoreSearchLayer::Callback::onExtraLong), !setting().onExtraLong, self->m_buttonMenu, ccp(winSize.width / 2.f - 140.f, winSize.height / 2.f - 80.f));

	return true;
}

void MoreSearchLayer::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xa3620), MoreSearchLayer::initH, reinterpret_cast<void**>(&MoreSearchLayer::init));
}