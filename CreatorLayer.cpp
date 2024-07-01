#include "CreatorLayer.h"

bool __fastcall CreatorLayer::hook(CCLayer* self) {
	CCLayer* CreatorLayerObject = self;

	bool result = CreatorLayer::init(self);

	auto director = CCDirector::sharedDirector();
	auto size = director->getWinSize();
	auto menu = CCMenu::create();

	auto swaston = CCSprite::createWithSpriteFrameName("player_ball_06_2_001.png");
	auto swastoncircle = CCSprite::createWithSpriteFrameName("d_ball_01_001.png");
	auto swastonbg = CCSprite::createWithSpriteFrameName("d_ball_07_001.png");

	auto btnspr = CCSprite::createWithSpriteFrameName("bgIcon_01_001.png");
	auto btnbtn = gd::CCMenuItemSpriteExtra::create(btnspr, nullptr, self, 0);

	//gd::MenuLayer* MGLayer = gd::MenuLayer::node();
	//CCDirector::sharedDirector()->getRunningScene()->addChild(MGLayer);

	//self->addChild(MGLayer);
	

	/*swaston->setPositionX(size.width / 2);
	swaston->setPositionY(size.height / 15);
	swaston->setColor(ccColor3B(0, 0, 0));
	swaston->setRotation(-20.0f);
	swaston->setFlipX(1);
	swaston->setZOrder(103);

	swastoncircle->setPositionX(size.width / 2);
	swastoncircle->setPositionY(size.height / 15);
	swastoncircle->setZOrder(102);
	swastoncircle->setScale(1.2f);
	
	swastonbg->setPositionX(size.width / 2);
	swastonbg->setPositionY(size.height / 15);
	swastonbg->setColor(ccColor3B(255, 0, 0));
	swastonbg->setZOrder(100);
	swastonbg->setScaleX(3.0f);
	swastonbg->setScaleY(1.5f);




	self->addChild(swaston);
	self->addChild(swastoncircle);
	self->addChild(swastonbg);*/

	return result;
}