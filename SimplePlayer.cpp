#include "SimplePlayer.hpp"
#include "Icons.hpp"

void SimplePlayer::newExtraFrame(gd::SimplePlayer* simplePlayer, char const* extra) {
	CCSprite* extraSpr = static_cast<CCSprite*>(simplePlayer->m_firstLayer->getChildByTag(69));
	auto spriteFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
	if (!extraSpr) return;

	if (spriteFrameCache->spriteFrameByName(extra)) {
		extraSpr->setPosition(simplePlayer->m_secondLayer->getPosition());
		extraSpr->setDisplayFrame(spriteFrameCache->spriteFrameByName(extra));
		extraSpr->setVisible(true);
	}
	else {
		extraSpr->setVisible(false);
	}
}

bool __fastcall SimplePlayer::initH(gd::SimplePlayer* self, void*, int frameID) {
	if (!SimplePlayer::init(self, frameID)) return false;

	int limit = Icons::getCount("player", "001");
	if (frameID > limit) frameID = limit;

	CCSprite* extraSprite = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("player_%02d_glow_001.png", frameID)->getCString());
	extraSprite->setTag(69);
	extraSprite->setVisible(false);
	extraSprite->setZOrder(99);
	extraSprite->setPosition(self->m_secondLayer->getPosition());
	self->m_firstLayer->addChild(extraSprite);

	SimplePlayer::newExtraFrame(self, CCString::createWithFormat("player_%02d_extra_001.png", frameID)->getCString());

	return true;
}

void __fastcall SimplePlayer::updatePlayerFrameH(gd::SimplePlayer* self, void*, int frameID, gd::IconType iconType) {
	SimplePlayer::updatePlayerFrame(self, frameID, iconType);

	const char* prefix = "player";
	switch (iconType) {
	case gd::IconType::Ship:
		prefix = "ship"; break;
	case gd::IconType::Ball:
		prefix = "player_ball"; break;
	case gd::IconType::UFO:
		prefix = "bird"; break;
	case gd::IconType::Wave:
		prefix = "dart"; break;
	default: break;
	}

	int limit = Icons::getCount(prefix, "001");
	if (frameID > limit) frameID = limit;
	SimplePlayer::newExtraFrame(self, CCString::createWithFormat(std::string(prefix + std::string("_%02d_extra_001.png")).c_str(), frameID)->getCString());
}

void SimplePlayer::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x80810), SimplePlayer::initH, reinterpret_cast<void**>(&SimplePlayer::init));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x80c70), SimplePlayer::updatePlayerFrameH, reinterpret_cast<void**>(&SimplePlayer::updatePlayerFrame));
}