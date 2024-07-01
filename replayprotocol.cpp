#include "replayprotocol.h"

bool ReplayProtocol::init() {
	static CCLayerColor* cclcol;
	if (!(cclcol->initWithColor(ccc4(0, 0, 0, 105)))) return false;
	cocos2d::extension::CCScale9Sprite* bg = cocos2d::extension::CCScale9Sprite::create("GJ_square01.png");
	auto director = CCDirector::sharedDirector();
	bg->setContentSize({ 360,180 });
	bg->setPosition((director->getScreenRight()) / 2, (director->getScreenTop()) / 2);
	bg->setZOrder(10);
	this->addChild(bg);

	return true;
}

void ReplayProtocol::keyBackClicked() {

}