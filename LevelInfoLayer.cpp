#include "LevelInfoLayer.hpp"
#include "LevelShare.hpp"
#include "nfd.h"
#include <fstream>
#include "Setting.hpp"
#include "utils.hpp"
#include "PlayLayer.hpp"

gd::CustomSongWidget* m_songWidget;

void LevelInfoLayer::Callback::onExportLevel(CCObject*) {
	if (!this->m_level->m_levelString.size()) {
		gd::FLAlertLayer::create("Error", "Level string is empty!", "OK")->show();
		return;
	}

	nfdchar_t* path = nullptr;
	if (NFD_SaveDialog("gmd", std::string(CCFileUtils::sharedFileUtils()->getWritablePath2() + this->m_level->m_levelName).c_str(), &path) == NFD_OKAY) {
		std::ofstream file(path);
		dumpLevel(this->m_level, file);
		free(path);
		gd::FLAlertLayer::create("Success", "The level has been exported.", "OK")->show();
	}
}

void LevelInfoLayer::Callback::onMoveToTop(CCObject*) {
	auto layer = gd::FLAlertLayer::create(this, "Move To Top", "Move this level to the top of the levels list?", "NO", "YES", 300.f);
	layer->setTag(10);
	layer->show();
}

void LevelInfoLayer::Callback::onFavorite(CCObject*) {
	auto strings = split(setting().m_favoritedLevelsIDs, ',');
	for (const auto& str : strings) {
		const int i = std::stoi(str);
		if (this->m_level->m_levelID == i) {
			
		}
	}
}

void LevelInfoLayer::Callback::onPlayReplay(CCObject*) {
	if (!this->shouldDownloadLevel()) {
		gd::GameSoundManager::sharedState()->stopBackgroundMusic();
		gd::GameSoundManager::sharedState()->playSound("playSound_01.ogg");

		gd::GameManager::sharedState()->m_lastScene = gd::LastGameScene::LevelInfoLayerOrEditLevelLayer;

		CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(.5f, gd::PlayLayer::scene(this->m_level)));

		auto playLayer = gd::GameManager::sharedState()->getPlayLayer();
		if (playLayer) {
			playLayer->m_playbackMode = true;
			PlayLayer::setupReplay(playLayer, this->m_level->m_recordString);
		}
	}
}

bool __fastcall LevelInfoLayer::initH(gd::LevelInfoLayer* self, void*, gd::GJGameLevel* level) {
	m_songWidget = nullptr;
	if (!LevelInfoLayer::init(self, level)) return false;

	if (m_songWidget) {
		std::cout << m_songWidget << std::endl;
		
		if (setting().onAutoSongDownload && !m_songWidget->m_isRobtopSong) {
			m_songWidget->onDownload(nullptr);
		}
	}

	auto director = CCDirector::sharedDirector();
	auto winSize = director->getWinSize();

	auto actionsMenu = static_cast<CCMenu*>(self->m_likeBtn->getParent());

	auto onExportLevelSpr = CCSprite::createWithSpriteFrameName("GJ_downloadBtn_001.png");
	if (!onExportLevelSpr->initWithFile("BE_Export_File.png")) {
		onExportLevelSpr->createWithSpriteFrameName("GJ_downloadBtn_001.png");
	}
	auto onExportLevel = gd::CCMenuItemSpriteExtra::create(onExportLevelSpr, self, menu_selector(LevelInfoLayer::Callback::onExportLevel));
	onExportLevel->setPosition(actionsMenu->convertToNodeSpace({ director->getScreenLeft() + 30.f, winSize.height / 2.f + 75.f }));
	actionsMenu->addChild(onExportLevel);

	//auto onGarageSpr = CCSprite::createWithSpriteFrameName("gj_iconBtn_on_001.png");
	//auto onGarage = gd::CCMenuItemSpriteExtra::create(onGarageSpr, self, menu_selector(LevelInfoLayer::Callback::onGarage));
	//onGarage->setPosition(100, 50);
	//actionsMenu->addChild(onGarage);

	auto onMoveToTopSpr = CCSprite::createWithSpriteFrameName("edit_upBtn_001.png");
	auto onMoveToTop = gd::CCMenuItemSpriteExtra::create(onMoveToTopSpr, self, menu_selector(LevelInfoLayer::Callback::onMoveToTop));
	onMoveToTop->setPosition(actionsMenu->convertToNodeSpace(ccp(director->getScreenRight() - 80.f, director->getScreenBottom() + 70.f)));
	actionsMenu->addChild(onMoveToTop);

	if (setting().onDeveloperMode) {
		auto labelBg = extension::CCScale9Sprite::create("square02_small.png");
		labelBg->setPosition(director->getScreenLeft() + 96.f, winSize.height / 2.f);
		labelBg->setOpacity(100);
		labelBg->setContentSize({ 80.f, 240.f });
		self->addChild(labelBg, -1);

		auto developerLabel = CCLabelBMFont::create("", "chatFont.fnt");
		developerLabel->setScale(.4f);
		developerLabel->setPosition(director->getScreenLeft() + 60.f, winSize.height / 2.f);
		developerLabel->setAnchorPoint({ 0.f, .5f });
		self->addChild(developerLabel, 15);

		std::stringstream ss;

		ss << "m_levelID: " << level->m_levelID << std::endl;
		ss << "m_userName: " << level->m_userName.c_str() << std::endl;
		ss << "m_recordString: " << level->m_recordString.c_str() << std::endl;
		ss << "m_uploadDate: " << level->m_uploadDate.c_str() << std::endl;
		ss << "m_updateDate: " << level->m_updateDate.c_str() << std::endl;
		ss << "m_userID: " << level->m_userID << std::endl;
		ss << "m_difficulty: " << level->m_difficulty << std::endl;
		ss << "m_levelRev: " << level->m_levelRev << std::endl;
		ss << "m_objectCount: " << level->m_objectCount << std::endl;
		ss << "m_order: " << level->m_order << std::endl;
		ss << "m_ratings: " << level->m_ratings << std::endl;
		ss << "m_ratingsSum: " << level->m_ratingsSum << std::endl;
		ss << "m_completes: " << level->m_completes << std::endl;
		ss << "m_hasBeenModified: " << std::boolalpha << level->m_hasBeenModified << std::endl;
		ss << "m_levelVersion: " << level->m_levelVersion << std::endl;
		ss << "m_gameVersion: " << level->m_gameVersion << std::endl;
		ss << "m_likes: " << level->m_likes << std::endl;
		ss << "m_dislikes: " << level->m_dislikes << std::endl;
		ss << "m_featured: " << level->m_featured << std::endl;
		ss << "m_password: " << level->m_password << std::endl;
		ss << "m_twoPlayerMode: " << std::boolalpha << level->m_twoPlayerMode << std::endl;
		ss << "m_starRatings: " << level->m_starRatings << std::endl;
		ss << "m_starRatingsSum: " << level->m_starRatingsSum << std::endl;
		ss << "m_maxStarRatings: " << level->m_maxStarRatings << std::endl;
		ss << "m_minStarRatings: " << level->m_minStarRatings << std::endl;
		ss << "m_demonVotes: " << level->m_demonVotes << std::endl;
		ss << "m_rateStars: " << level->m_rateStars << std::endl;
		ss << "m_rateFeature: " << std::boolalpha << level->m_rateFeature << std::endl;
		ss << "m_rateUser: " << level->m_rateUser.c_str() << std::endl;
		ss << "m_dontSave: " << std::boolalpha << level->m_dontSave << std::endl;
		ss << "m_isHidden: " << std::boolalpha << level->m_isHidden << std::endl;
		ss << "m_ID: " << level->m_ID << std::endl;
		ss << "m_tempName: " << level->m_tempName.c_str() << std::endl;
		ss << "m_capacity001: " << level->m_capacity001 << std::endl;
		ss << "m_capacity002: " << level->m_capacity002 << std::endl;
		ss << "m_capacity003: " << level->m_capacity003 << std::endl;
		ss << "m_capacity004: " << level->m_capacity004 << std::endl;

		developerLabel->setString(ss.str().c_str());

		auto onFavoriteOff = CCSprite::create("GJ_heartOff_001.png");
		auto onFavoriteOn = CCSprite::create("GJ_heartOn_001.png");

		auto onFavorite = gd::CCMenuItemToggler::create(onFavoriteOff, onFavoriteOn, self, menu_selector(LevelInfoLayer::Callback::onFavorite));
		onFavorite->setPosition(actionsMenu->convertToNodeSpace({ director->getScreenLeft() + 68.f, director->getScreenBottom() + 30.f }));
		actionsMenu->addChild(onFavorite);

		auto onPlayReplaySpr = CCSprite::createWithSpriteFrameName("GJ_playBtn2_001.png");
		onPlayReplaySpr->setScale(.35f);
		auto onPlayReplay = gd::CCMenuItemSpriteExtra::create(onPlayReplaySpr, self, menu_selector(LevelInfoLayer::Callback::onPlayReplay));
		onPlayReplay->setOpacity(100);
		onPlayReplay->setPositionX(60.f);
		self->m_playBtnMenu->addChild(onPlayReplay, -1);
	}

	return true;
}

void __fastcall LevelInfoLayer::onCloneH(gd::LevelInfoLayer* self, void*, CCObject* sender) {
	LevelInfoLayer::onClone(self, sender);

	if (!self->shouldDownloadLevel()) {
		auto level = static_cast<gd::GJGameLevel*>(gd::LocalLevelManager::sharedState()->m_localLevels->objectAtIndex(0));
		level->m_songID = self->m_level->m_songID;
	}
}

void __fastcall LevelInfoLayer::FLAlert_ClickedH(gd::LevelInfoLayer* _self, void*, gd::FLAlertLayer* layer, bool btn2) {
	auto self = reinterpret_cast<gd::LevelInfoLayer*>(reinterpret_cast<uintptr_t>(_self) - 0x128);
	if ((layer->getTag() == 10) && btn2) {
		auto savedLevels = gd::GameLevelManager::sharedState()->m_savedLevelsDict;
		savedLevels->removeObjectForKey(std::to_string(self->m_level->m_levelID));
		savedLevels->setObject(self->m_level, std::to_string(self->m_level->m_levelID));
	}
	LevelInfoLayer::FLAlert_Clicked(_self, layer, btn2);
}

void __fastcall LevelInfoLayer::onLevelInfoH(gd::LevelInfoLayer* self, void*, CCObject* sender) {
	if (self->m_level->m_objectCount == 0) {
		return LevelInfoLayer::onLevelInfo(self, sender);
	}

	auto info = CCString::createWithFormat("<cy>%s</c>\n<cg>Total Attempts</c>: %i\n<cl>Total Jumps</c>: %i\n<cp>Normal</c>: %i%%\n<co>Practice</c>: %i%%\n<cy>Objects</c>: %i",
		self->m_level->m_levelName.c_str(),
		self->m_level->m_attempts,
		self->m_level->m_jumps,
		self->m_level->m_normalPercent,
		self->m_level->m_practicePercent,
		self->m_level->m_objectCount)->getCString();

	gd::FLAlertLayer::create("Level Info", info, "OK")->show();
}

void __fastcall LevelInfoLayer::levelUpdateFinishedH(gd::LevelInfoLayer* _self, void*, gd::GJGameLevel* level, gd::UpdateResponse response) {
	LevelInfoLayer::levelUpdateFinished(_self, level, response);
	auto self = reinterpret_cast<gd::LevelInfoLayer*>(reinterpret_cast<uintptr_t>(_self) - 0x11c);

	self->updateLabelValues();
}

void __fastcall LevelInfoLayer::songWidgetH() {
	__asm {
		mov m_songWidget, eax
	}
	LevelInfoLayer::songWidget();
}

void __fastcall LevelInfoLayer::destructorH(gd::LevelInfoLayer* self) {
	LevelInfoLayer::destructor(self);
	m_songWidget = nullptr;
}

void LevelInfoLayer::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x9bc10), LevelInfoLayer::initH, reinterpret_cast<void**>(&LevelInfoLayer::init));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x9e2c0), LevelInfoLayer::onCloneH, reinterpret_cast<void**>(&LevelInfoLayer::onClone));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x9f2d0), LevelInfoLayer::FLAlert_ClickedH, reinterpret_cast<void**>(&LevelInfoLayer::FLAlert_Clicked));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x9ed10), LevelInfoLayer::onLevelInfoH, reinterpret_cast<void**>(&LevelInfoLayer::onLevelInfo));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x9da70), LevelInfoLayer::levelUpdateFinishedH, reinterpret_cast<void**>(&LevelInfoLayer::levelUpdateFinished));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x9b940), LevelInfoLayer::destructorH, reinterpret_cast<void**>(&LevelInfoLayer::destructor));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x9cb06), LevelInfoLayer::songWidgetH, reinterpret_cast<void**>(&LevelInfoLayer::songWidget));
}