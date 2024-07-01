#include "LoadingLayer.h"

bool __fastcall LoadingLayer::hook(CCLayer* self) {
	CCLayer* LoadingLayerObject = self;

	bool result = LoadingLayer::init(self);
	auto director = CCDirector::sharedDirector();
	auto size = director->getWinSize();
	auto menu = CCMenu::create();

	auto picture = CCSprite::createWithSpriteFrameName("player_17_2_001.png");

	picture->setScale(5.f);
	picture->setPositionX(size.width / 2);
	picture->setPositionY(size.height / 16);

	self->addChild(picture);

	return result;
}