#include "SimplePlayer.h"
#include "Icons.h"

gd::SimplePlayer* simplePlayer;

bool __fastcall SimplePlayer::init_H(gd::SimplePlayer* self, void* edx, int frameID) {
	simplePlayer = self;
	if (!SimplePlayer::init(self, frameID)) return false;

	int limit = Icons::getCount("player", "001");
	if (frameID > limit) frameID = limit;

	CCSprite* extraSprite = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("player_%02d_glow_001.png", frameID)->getCString());
	extraSprite->setTag(69);
	extraSprite->setVisible(false);
	extraSprite->setZOrder(99);
	extraSprite->setPosition(self->colorSpr()->getPosition());
	self->baseSpr()->addChild(extraSprite);

	SimplePlayer::newExtraFrame(CCString::createWithFormat("player_%02d_extra_001.png", frameID)->getCString());

	return true;
}

void SimplePlayer::newExtraFrame(char const* extra) {
	//std::cout << extra << std::endl;
	auto extraSpr = static_cast<CCSprite*>(simplePlayer->baseSpr()->getChildByTag(69));
	auto SFC = CCSpriteFrameCache::sharedSpriteFrameCache();
	if (!extraSpr) return;


	if (SFC->spriteFrameByName(extra)) {
		extraSpr->setPosition(simplePlayer->colorSpr()->getPosition());
		extraSpr->setDisplayFrame(SFC->spriteFrameByName(extra));
		extraSpr->setVisible(true);
		/*std::cout << "isExtra" << std::endl;*/
	}
	else {
		extraSpr->setVisible(false);
		/*std::cout << "noExtra" << std::endl;*/
	}
}

void __fastcall SimplePlayer::updatePlayerFrame_H(gd::SimplePlayer* self, void* edx, int frameID, gd::IconType iconType) {
	SimplePlayer::updatePlayerFrame(self, frameID, iconType);
	const char* prefix = "player";
	switch (iconType) {
	case gd::IconType::Ship: prefix = "ship"; break;
	case gd::IconType::Ball: prefix = "player_ball"; break;
	case gd::IconType::UFO: prefix = "bird"; break;
	case gd::IconType::Wave: prefix = "dart"; break;
	default: break;
	}
	int limit = Icons::getCount(prefix, "001");
	//std::cout << prefix << " " << limit << std::endl;
	if (frameID > limit) frameID = limit;
	SimplePlayer::newExtraFrame(CCString::createWithFormat("player_%02d_extra_001.png", frameID)->getCString());
}

void SimplePlayer::mem_init() {
	MH_CreateHook(
		reinterpret_cast<void*>(gd::base + 0x80810),
		SimplePlayer::init_H,
		reinterpret_cast<void**>(&SimplePlayer::init));
	MH_CreateHook(
		reinterpret_cast<void*>(gd::base + 0x80c70),
		SimplePlayer::updatePlayerFrame_H,
		reinterpret_cast<void**>(&SimplePlayer::updatePlayerFrame));
}