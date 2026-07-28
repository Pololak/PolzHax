#include "SimplePlayer.hpp"
#include "Icons.hpp"

void SimplePlayer::updateExtraSprite(gd::SimplePlayer* self, std::string frameName) {
	auto extraSprite = static_cast<CCSprite*>(self->m_firstLayer->getChildByTag(6556));
	auto spriteFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(frameName.c_str());
	if (spriteFrame) {
		extraSprite->setDisplayFrame(spriteFrame);
		extraSprite->setVisible(true);
		extraSprite->setPosition(self->m_firstLayer->getContentSize() / 2.f);
	}
	else {
		extraSprite->setVisible(false);
	}
}

bool __fastcall SimplePlayer::initH(gd::SimplePlayer* self, void*, int frameID) {
	if (!SimplePlayer::init(self, frameID)) return false;

	int limit = Icons::getCount("player", "001");
	if (frameID > limit) frameID = limit;

	auto extraSprite = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("player_%02d_001.png", frameID)->getCString());
	self->m_firstLayer->addChild(extraSprite, 2, 6556);
	extraSprite->setPosition(self->m_firstLayer->convertToNodeSpace(self->m_firstLayer->getContentSize()));

	auto extraFrameName = CCString::createWithFormat("player_%02d_extra_001.png", frameID);
	SimplePlayer::updateExtraSprite(self, extraFrameName->getCString());

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
	SimplePlayer::updateExtraSprite(self, CCString::createWithFormat(std::string(prefix + std::string("_%02d_extra_001.png")).c_str(), frameID)->getCString());
}

void SimplePlayer::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x80810), SimplePlayer::initH, reinterpret_cast<void**>(&SimplePlayer::init));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x80c70), SimplePlayer::updatePlayerFrameH, reinterpret_cast<void**>(&SimplePlayer::updatePlayerFrame));
}