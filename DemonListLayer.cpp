#include "DemonListLayer.hpp"
#include "json.hpp"
#include "utils.hpp"
#include <unordered_map>

DemonListLayer* m_demonListLayer;

class DemonList : public cocos2d::CCObject, gd::LevelManagerDelegate {
public:
	std::unordered_map<gd::GJGameLevel*, int> m_listLevels;
	std::unordered_map<int, int> m_idsPositions;

	static DemonList& get() {
		static DemonList ret;
		return ret;
	}

	void getLevels() {
		if (!m_listLevels.empty()) return;

		extension::CCHttpRequest* request = new extension::CCHttpRequest();
		request->setUrl("https://platinumdl.alwaysdata.net/api/v2/demons");
		request->setRequestType(extension::CCHttpRequest::kHttpGet);
		request->setResponseCallback(this, httpresponse_selector(DemonList::onProcessHttpRequestCompleted));
		extension::CCHttpClient::getInstance()->send(request);
		request->release();
	}

	void onProcessHttpRequestCompleted(extension::CCHttpClient* client, extension::CCHttpResponse* response) {
		if (response->isSucceed()) {
			std::string responseCodeInfo = "Response code: <cg>" + std::to_string(response->getResponseCode()) + "</c>";
			gd::FLAlertLayer::create("Response", responseCodeInfo.c_str(), "OK")->show();

			auto& responseData = *response->getResponseData();
			auto data = std::string(responseData.begin(), responseData.end());
			if (data.size()) {
				nlohmann::json parseJson = nlohmann::json::parse(data);
				if (parseJson.size()) {
					auto glm = gd::GameLevelManager::sharedState();
					glm->m_levelManagerDelegate = m_demonListLayer;
					std::string customSearchQuery;

					for (int i = 0; i < parseJson.size(); i++) {
						auto& parsedLevel = parseJson[i];
						if (i > 0) {
							customSearchQuery += ",";
						}

						m_idsPositions.insert({ static_cast<int>(parsedLevel["level_id"]), static_cast<int>(parsedLevel["position"]) });

						customSearchQuery += std::to_string(static_cast<int>(parsedLevel["level_id"]));
					}

					auto searchObject = gd::GJSearchObject::create(static_cast<gd::SearchType>(26));
					searchObject->m_searchQuery = customSearchQuery;

					if (auto storedLevels = glm->getStoredOnlineLevels(searchObject->getKey())) {
						this->loadLevelsFinished(storedLevels, "");
					}
					else {
						glm->getOnlineLevels(searchObject);
					}
				}
			}
		}
	}

	virtual void loadLevelsFinished(cocos2d::CCArray* levels, char const*) override {
		std::cout << "Loaded or got online levels..." << std::endl;
		for (auto level : CCArrayExt<gd::GJGameLevel*>(levels)) {
			if (level && m_idsPositions.size()) {
				m_listLevels[level] = m_idsPositions[level->m_levelID];
			}
		}

		for (const auto& level : m_listLevels) {
			std::cout << "Name: " << level.first->m_levelName.c_str() << " " << "Rank: " << level.second << std::endl;
		}

		//m_demonListLayer->populateList();

		auto glm = gd::GameLevelManager::sharedState();
		if (glm->m_levelManagerDelegate == this) glm->m_levelManagerDelegate = nullptr;
	}

	virtual void loadLevelsFailed(char const*) override {
		auto glm = gd::GameLevelManager::sharedState();
		if (glm->m_levelManagerDelegate == this) glm->m_levelManagerDelegate = nullptr;
	}
};

bool DemonListLayer::init() {
	m_demonListLayer = this;
	if (!CCLayer::init()) return false;

	auto director = CCDirector::sharedDirector();
	auto winSize = director->getWinSize();

	auto backgroundSprite = CCSprite::create("GJ_gradientBG.png");
	backgroundSprite->setAnchorPoint({ 0.f, 0.f });
	backgroundSprite->setScaleX(winSize.width / backgroundSprite->getContentSize().width);
	backgroundSprite->setScaleY(winSize.height / backgroundSprite->getContentSize().height);
	backgroundSprite->setColor(ccc3(100, 100, 100));
	this->addChild(backgroundSprite, -2);

	auto leftCornerSprite = CCSprite::createWithSpriteFrameName("GJ_sideArt_001.png");
	leftCornerSprite->setPosition({ director->getScreenLeft() - 1.f, director->getScreenBottom() - 1.f });
	leftCornerSprite->setAnchorPoint({ 0.f, 0.f });
	this->addChild(leftCornerSprite, 1);

	auto rightCornerSprite = CCSprite::createWithSpriteFrameName("GJ_sideArt_001.png");
	rightCornerSprite->setPosition({ director->getScreenRight() + 1.f, director->getScreenBottom() - 1.f });
	rightCornerSprite->setAnchorPoint({ 1.f, 0.f });
	rightCornerSprite->setFlipX(true);
	this->addChild(rightCornerSprite, 1);

	auto backMenu = CCMenu::create();
	backMenu->setPosition(director->getScreenLeft() + 24.f, director->getScreenTop() - 23.f);
	this->addChild(backMenu, 1);

	auto onBackSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
	auto onBack = gd::CCMenuItemSpriteExtra::create(onBackSprite, this, 0);
	onBack->setSizeMult(2.f);
	backMenu->addChild(onBack);

	m_listLayer = gd::GJListLayer::create(nullptr, "Demonlist", { 0, 0, 0, 180 }, 356.f, 220.f);
	m_listLayer->setPosition(winSize / 2.f - m_listLayer->getContentSize() / 2.f);
	this->addChild(m_listLayer);

	m_loadingCircle = gd::LoadingCircle::create();
	m_loadingCircle->m_parentLayer = this;
	m_loadingCircle->show();

	DemonList::get().getLevels();

	this->setKeypadEnabled(true);

	return true;
}

void DemonListLayer::populateList() {
	auto levels = CCArray::create();
	for (const auto& level : DemonList::get().m_listLevels) {
		levels->insertObject(level.first, level.second);
	}

	if (levels->count()) {
		this->loadLevelsFinished(levels, "");
	}
}

void DemonListLayer::onProcessHttpRequestCompleted(extension::CCHttpClient* client, extension::CCHttpResponse* response) {
}

void DemonListLayer::loadLevelsFinished(CCArray* levels, char const*) {

	//if (auto listView = m_listLayer->m_listView) {
	//	listView->removeFromParent();
	//	listView->release();
	//}

	auto listView = gd::CustomListView::create(levels, 220.f, 356.f, gd::BoomListType::Level);
	listView->retain();
	m_listLayer->addChild(listView, 6, 9);
	m_listLayer->m_listView = listView;

	this->m_loadingCircle->setVisible(false);
}

void DemonListLayer::keyBackClicked() {
	CCDirector::sharedDirector()->popScene();
}

void DemonListLayer::onExit() {
	auto glm = gd::GameLevelManager::sharedState();
	if (glm->m_levelManagerDelegate == this) glm->m_levelManagerDelegate = nullptr;
	m_demonListLayer = nullptr;
	CCLayer::onExit();
}