#include "EndLevelLayer.h"

void __fastcall EndLevelLayer::customSetup_H(gd::GJDropDownLayer* self) {

	auto director = CCDirector::sharedDirector();
	auto layerMain = (cocos2d::CCLayer*)self->getChildren()->objectAtIndex(0);

	auto safeLabel = CCLabelBMFont::create("Safe Mode", "bigFont.fnt");
	safeLabel->setPosition({ director->getScreenLeft() / 2, director->getScreenTop() / 2 });
	layerMain->addChild(safeLabel);


	EndLevelLayer::customSetup(self);
}

void EndLevelLayer::mem_init() {
	MH_CreateHook(
		reinterpret_cast<void*>(gd::base + 0x50430),
		EndLevelLayer::customSetup,
		reinterpret_cast<void**>(EndLevelLayer::customSetup_H));
}