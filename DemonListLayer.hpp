#pragma once
#include "pch.h"
#include "json.hpp"

class DemonListLayer : public cocos2d::CCLayer, public gd::LevelManagerDelegate {
protected:
	gd::GJListLayer* m_listLayer;
	gd::LoadingCircle* m_loadingCircle;
	std::vector<int> m_searchResults;
	nlohmann::json m_jsonLevels;

	virtual bool init();
	virtual void keyBackClicked() override;
	virtual void loadLevelsFinished(cocos2d::CCArray*, char const*) override;
	void onProcessHttpRequestCompleted(extension::CCHttpClient*, extension::CCHttpResponse*);
	virtual void onExit() override;

public:
	static DemonListLayer* create() {
		DemonListLayer* ret = new DemonListLayer();
		if (ret && ret->init()) {
			ret->autorelease();
			return ret;
		}
		CC_SAFE_DELETE(ret);
		return nullptr;
	}

	void populateList();

	static cocos2d::CCScene* scene() {
		auto scene = CCScene::create();
		scene->addChild(DemonListLayer::create());
		return scene;
	}
};