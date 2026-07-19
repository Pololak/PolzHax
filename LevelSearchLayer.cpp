#include "LevelSearchLayer.hpp"
#include "Setting.hpp"
#include <fstream>
#include "json.hpp"
#include "DemonListLayer.hpp"

void LevelSearchLayer::Callback::onSearchPlayer(CCObject*) {
	if (!this->m_searchInput->getString().empty()) {
		std::cout << this->m_searchInput->getString() << std::endl;
		CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(.5f, gd::LevelBrowserLayer::scene(gd::GJSearchObject::create(gd::SearchType::UsersLevels, this->m_searchInput->getString()))));
	}
}

void LevelSearchLayer::Callback::onProcessHttpRequestCompleted(extension::CCHttpClient* client, extension::CCHttpResponse* response) {
	if (response->isSucceed()) {
		std::cout << response->getResponseCode() << std::endl;
		std::string ss;
		for (const auto& idk : *response->getResponseData()) {
			ss += idk;
		}
		std::cout << ss << std::endl;

		nlohmann::json j = nlohmann::json::parse(ss);
		if (j.size()) {
			std::string customSearchQuery;

			for (int i = 0; i < j.size(); i++) {
				auto parsedLevel = j[i];
				if (i > 0) {
					customSearchQuery += ",";
				}

				customSearchQuery += std::to_string(static_cast<int>(parsedLevel["level_id"]));
			}

			auto searchObject = gd::GJSearchObject::create(static_cast<gd::SearchType>(26));
			searchObject->m_searchQuery = customSearchQuery;

			CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(.5f, gd::LevelBrowserLayer::scene(searchObject)));
		}
	}
}

void LevelSearchLayer::Callback::onTestDemonlist(CCObject*) {
	std::cout << "flksadhjfkjsdahfkjhsdf" << std::endl;
	CCDirector::sharedDirector()->pushScene(CCTransitionFade::create(.5f, DemonListLayer::scene()));

	//extension::CCHttpRequest* request = new extension::CCHttpRequest();
	//request->setUrl("https://platinumdl.alwaysdata.net/api/v2/demons");
	//request->setRequestType(extension::CCHttpRequest::kHttpGet);
	//request->setResponseCallback(this, httpresponse_selector(Callback::onProcessHttpRequestCompleted));
	//extension::CCHttpClient::getInstance()->send(request);
	//request->release();

	//std::ifstream demonlistFile;
	//demonlistFile.open(CCFileUtils::sharedFileUtils()->getWritablePath2() + "demonlist.json");
	//if (demonlistFile.is_open()) {
	//	nlohmann::json j = nlohmann::json::parse(demonlistFile);
	//	if (j.size()) {
	//		std::string customSearchQuery;

	//		for (int i = 0; i < j.size(); i++) {
	//			auto parsedLevel = j[i];
	//			if (i > 0) {
	//				customSearchQuery += ",";
	//			}

	//			customSearchQuery += std::to_string(static_cast<int>(parsedLevel["levelID"]));
	//		}

	//		auto searchObject = gd::GJSearchObject::create(static_cast<gd::SearchType>(26));
	//		searchObject->m_searchQuery = customSearchQuery;

	//		CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(.5f, gd::LevelBrowserLayer::scene(searchObject)));
	//	}
	//}
	//demonlistFile.close();
}

bool __fastcall LevelSearchLayer::initH(gd::LevelSearchLayer* self) {
	if (!LevelSearchLayer::init(self)) return false;

	auto director = CCDirector::sharedDirector();
	auto winSize = director->getWinSize();

	auto menu = CCMenu::create();
	self->addChild(menu);

	auto onSearchPlayerSpr = CCSprite::create("GJ_button_01.png");
	onSearchPlayerSpr->setScale(.7f);
	auto onSearchPlayerSpr2 = gd::SimplePlayer::create(4);
	onSearchPlayerSpr2->updatePlayerFrame(4, gd::IconType::Cube);
	onSearchPlayerSpr2->setScale(.9f);
	onSearchPlayerSpr2->setPosition(onSearchPlayerSpr->getContentSize() / 2.f);
	onSearchPlayerSpr->addChild(onSearchPlayerSpr2);
	auto onSearchPlayer = gd::CCMenuItemSpriteExtra::create(onSearchPlayerSpr, self, menu_selector(LevelSearchLayer::Callback::onSearchPlayer));
	onSearchPlayer->setPosition(200.f, 120.f);
	menu->addChild(onSearchPlayer);

	if (setting().onDeveloperMode) {
		auto rightMenu = self->getChildByType<CCMenu*>(0);
		if (rightMenu) {
			auto onTestDemonlistSpr = CCSprite::createWithSpriteFrameName("diffIcon_06_btn_001.png");
			auto onTestDemonlist = gd::CCMenuItemSpriteExtra::create(onTestDemonlistSpr, self, menu_selector(LevelSearchLayer::Callback::onTestDemonlist));
			onTestDemonlist->setPosition(rightMenu->convertToNodeSpace({ director->getScreenRight() - 25.f, director->getScreenTop() - 125.f }));
			rightMenu->addChild(onTestDemonlist);
		}
	}

	return true;
}

void __fastcall LevelSearchLayer::clearFiltersH(gd::LevelSearchLayer* self) {
	LevelSearchLayer::clearFilters(self);

	setting().onExtraLong = false;
}

void LevelSearchLayer::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x9f830), LevelSearchLayer::initH, reinterpret_cast<void**>(&LevelSearchLayer::init));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xa1000), LevelSearchLayer::clearFiltersH, reinterpret_cast<void**>(&LevelSearchLayer::clearFilters));
}