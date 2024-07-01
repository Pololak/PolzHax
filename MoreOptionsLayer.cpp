#include "MoreOptionsLayer.h"

bool __fastcall MoreOptionsLayer::hook(CCLayer* self) {

	CCLayer* MoreOptionsLayerObject = self;

	bool result = MoreOptionsLayer::init(self);
	auto labelm = CCLabelBMFont::create("Hello World", "bigFont.fnt");
	auto director = CCDirector::sharedDirector();
	auto size = director->getWinSize();
	auto menu = CCMenu::create();

	self->addChild(labelm);

	return result;
}