#include "InfoLayer.hpp"

void InfoLayer::Callback::onRefreshComments(CCObject*) {
	auto glm = gd::GameLevelManager::sharedState();

	int timeLeft = glm->getTimeLeft("upd_comments", 5.f);
	if (timeLeft >= 1) {
		return;
	}

	glm->makeTimeStamp("upd_comments");
	glm->resetTimerForKey(this->m_commentsKey.c_str());
	this->loadPage(this->m_page);
}

bool __fastcall InfoLayer::initH(gd::InfoLayer* self, void*, gd::GJGameLevel* level) {
	if (!InfoLayer::init(self, level)) return false;

	auto onRefreshCommentsSpr = CCSprite::createWithSpriteFrameName("GJ_updateBtn_001.png");
	onRefreshCommentsSpr->setScale(.7f);
	auto onRefreshComments = gd::CCMenuItemSpriteExtra::create(onRefreshCommentsSpr, self, menu_selector(InfoLayer::Callback::onRefreshComments));
	onRefreshComments->setPosition(406.f, -134.f);
	self->m_buttonMenu->addChild(onRefreshComments);

	return true;
}

void __fastcall InfoLayer::onLevelInfoH(gd::InfoLayer* self, void*, CCObject* sender) {
	//std::stringstream ss;

	//ss << "<cl>Uploaded</c>: " << self->m_level->m_uploadDate << "\n";
	//ss << "<cl>Updated</c>: " << self->m_level->m_updateDate << "\n";
	//if (self->m_level->m_originalLevel != 0) {
	//	ss << "<cg>Original</c>: " << self->m_level->m_originalLevel << "\n";
	//}
	//ss << "<cy>Game Version</c>: " << self->m_level->m_gameVersion << "\n";
	//if (self->m_level->m_password != 0) {
	//	ss << "<co>Password</c>: " << self->m_level->m_password << "\n";
	//}
	//else {
	//	ss << "<co>Password</c>: Free Copy" << "\n";
	//}
	//ss << "<cg>Objects</c>: " << self->m_level->m_objectCount << "\n";

	//gd::FLAlertLayer::create("Level Info", ss.str().c_str(), "OK")->show();
}

void InfoLayer::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x84080), InfoLayer::initH, reinterpret_cast<void**>(&InfoLayer::init));
	//MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x859c0), InfoLayer::onLevelInfoH, reinterpret_cast<void**>(&InfoLayer::onLevelInfo));
}