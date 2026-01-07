#include "LevelBrowserLayer.h"
#include "share.hpp"
#include <nfd.h>

gd::LevelBrowserLayer* levelBrowserLayer;
gd::ButtonSprite* m_moveToPageButtonSprite = nullptr;
gd::CCMenuItemSpriteExtra* m_moveToLast = nullptr;
gd::CCMenuItemSpriteExtra* m_moveToFirst = nullptr;
CCLayer* gotoPagePopup;

void updateButtonLabel();

class GoToPagePopup : public CCLayer, public gd::TextInputDelegate {
public:
	gd::CCTextInputNode* input_node = nullptr;

	static GoToPagePopup* create() {
		GoToPagePopup* obj = new GoToPagePopup;
		if (obj && obj->init()) {
			obj->autorelease();
			return obj;
		}
		CC_SAFE_DELETE(obj);
		return nullptr;
	}

	bool init() {
		if (!CCLayer::init()) return false;
		const float width = 220, height = 150;
		CCLayerColor* CCLayerCol = CCLayerColor::create(ccc4(0, 0, 0, 75));
		CCLayerCol->setZOrder(1);
		CCLayerCol->setScale(10.f);
		this->addChild(CCLayerCol);

		auto touchDispatcher = CCDirector::sharedDirector()->m_pTouchDispatcher;
		touchDispatcher->incrementForcePrio();
		this->registerWithTouchDispatcher();
		setTouchEnabled(true);
		setKeypadEnabled(true);
		setMouseEnabled(true);

		auto director = CCDirector::sharedDirector();

		auto bgSprite = CCSprite::create("GJ_button_03.png");
		bgSprite->setScale(100.f);
		bgSprite->setOpacity(0);
		auto bgButton = gd::CCMenuItemSpriteExtra::create(bgSprite, nullptr, this, nullptr);
		auto bgMenu = CCMenu::create();
		bgMenu->addChild(bgButton);
		bgMenu->setZOrder(0);
		bgMenu->setPosition((CCDirector::sharedDirector()->getScreenRight()) - 25, (CCDirector::sharedDirector()->getScreenTop()) - 25);
		this->addChild(bgMenu); // DONT EVEN ASK WHAT IS THIS

		auto bg = extension::CCScale9Sprite::create("GJ_square01.png");
		bg->setContentSize({ width, height });
		bg->setPosition(director->getWinSize().width / 2, director->getWinSize().height / 2 + 60.f);
		bg->setZOrder(2);
		this->addChild(bg);

		auto appearAction = CCEaseElasticOut::create(CCScaleTo::create(.5f, 1.f), .6f);

		auto menu = CCMenu::create();
		menu->setZOrder(3);
		this->addChild(menu);

		auto closeBtn = gd::CCMenuItemSpriteExtra::create(
			(CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png")),
			nullptr,
			this,
			menu_selector(GoToPagePopup::onClose));
		closeBtn->setPosition({-105, 130});

		menu->addChild(closeBtn);

		input_node = gd::CCTextInputNode::create("Num", this, "bigFont.fnt", 50.f, 40.f);
		input_node->setMaxLabelScale(0.6f);
		input_node->setMaxLabelWidth(200);
		input_node->setLabelPlaceholderColor({ 120, 170, 240 });
		input_node->setMaxLabelLength(8);
		input_node->setAllowedChars("0123456789");
		input_node->setDelegate(this);
		input_node->setZOrder(5);
		input_node->setPosition({ director->getWinSize().width / 2, director->getWinSize().height / 2 + 65 });
		auto currentPage = (from<int>(levelBrowserLayer, 0x140)) / 10 + 1;
		input_node->setString(CCString::createWithFormat("%i", currentPage)->getCString());
		this->addChild(input_node);

		auto inputBg = extension::CCScale9Sprite::create("square02_small.png");
		inputBg->setContentSize({ 50, 30 });
		inputBg->setPosition({ director->getWinSize().width / 2, director->getWinSize().height / 2 + 65 });
		inputBg->setZOrder(3);
		inputBg->setOpacity(100);
		this->addChild(inputBg);

		auto go = gd::CCMenuItemSpriteExtra::create(
			gd::ButtonSprite::create("Go", 0x28, 0, 1.f, true, "goldFont.fnt", "GJ_button_01.png", 30.f),
			nullptr,
			this,
			menu_selector(GoToPagePopup::onGo));
		go->setPositionY(10);
		menu->addChild(go);

		auto label = CCLabelBMFont::create("Go To Page", "goldFont.fnt");
		label->setZOrder(5);
		label->setScale(0.7f);
		label->setPosition({ director->getWinSize().width / 2, director->getWinSize().height / 2 + 117.f });
		this->addChild(label);

		auto inc = gd::CCMenuItemSpriteExtra::create(
			CCSprite::createWithSpriteFrameName("edit_rightBtn_001.png"),
			nullptr,
			this,
			menu_selector(GoToPagePopup::onIncrement)
		);
		inc->setPosition({ 40, 65 });
		menu->addChild(inc);

		auto dec = gd::CCMenuItemSpriteExtra::create(
			CCSprite::createWithSpriteFrameName("edit_leftBtn_001.png"),
			nullptr,
			this,
			menu_selector(GoToPagePopup::onDecrement)
		);
		dec->setPosition({ -40, 65 });
		menu->addChild(dec);

		return true;
	}

	void keyBackClicked() override {
		this->removeFromParentAndCleanup(true);
		gotoPagePopup = nullptr;
	}

	void onClose(CCObject*) {
		this->removeFromParentAndCleanup(true);
		gotoPagePopup = nullptr;
	}

	void onIncrement(CCObject*) {
		input_node->setString(std::to_string(std::atoi(input_node->getString()) + 1).c_str());
	}

	void onDecrement(CCObject*) {
		input_node->setString(std::to_string(std::atoi(input_node->getString()) - 1).c_str());
		if (std::atoi(input_node->getString()) < 1) {
			input_node->setString("1");
		}
	}

	void onGo(CCObject*) {
		auto searchObject = from<gd::GJSearchObject*>(levelBrowserLayer, 0x134);

		searchObject->m_page = std::atoi(input_node->getString()) - 1;

		if (std::atoi(input_node->getString()) < 1) {
			searchObject->m_page = 0;
		}

		auto totalPages = from<int>(levelBrowserLayer, 0x13c) / 10;

		if (std::atoi(input_node->getString()) > totalPages) {
			searchObject->m_page = totalPages;
		}

		levelBrowserLayer->loadPage(searchObject);
		updateButtonLabel();

		this->keyBackClicked();
	}

	void showCallback(CCObject* btn) {
		auto myIconLayer = GoToPagePopup::create();
		myIconLayer->setZOrder(105);
		CCDirector::sharedDirector()->getRunningScene()->addChild(myIconLayer);
		gotoPagePopup = myIconLayer;
	}
};

void updateButtonLabel() {
	if (m_moveToPageButtonSprite) {
		auto currentPage = (from<int>(levelBrowserLayer, 0x140)) / 10 + 1;
		m_moveToPageButtonSprite->getLabel()->setString(CCString::createWithFormat("%i", currentPage)->getCString());
	}
}

void LevelBrowserLayer::Callback::onRefresh(CCObject*) {
    if (from<gd::GJSearchObject*>(this, 0x134) != nullptr) {	
        auto searchObject = from<gd::GJSearchObject*>(this, 0x134);
        std::cout << searchObject->getKey() << " getKey" << std::endl;
		gd::GameLevelManager::sharedState()->resetTimerForKey(searchObject->getKey());
        this->loadPage(searchObject);
        if (from<bool>(this, 0x9c) != 0)
            from<bool>(this, 0x9c) = 0;
    }
    return;
}

void LevelBrowserLayer::Callback::onLastPage(CCObject*) {
    auto searchObject = from<gd::GJSearchObject*>(this, 0x134);
    searchObject->m_page = from<int>(this, 0x13c) / 10;
    this->loadPage(searchObject);
    updateButtonLabel();
	m_moveToLast->setVisible(0);
	m_moveToFirst->setVisible(1);
}

void LevelBrowserLayer::Callback::onFirstPage(CCObject*) {
    auto searchObject = from<gd::GJSearchObject*>(this, 0x134);
    searchObject->m_page = 0;
    this->loadPage(searchObject);
    updateButtonLabel();
	m_moveToFirst->setVisible(0);
	m_moveToLast->setVisible(1);
}

void LevelBrowserLayer::Callback::onImportLevel(CCObject*) {
	nfdchar_t* path = nullptr;
	if (NFD_OpenDialog("gmd", nullptr, &path) == NFD_OKAY) {
		std::ifstream file(path);
		auto* const level = import_level(file);
		free(path);
		if (!level) {
			gd::FLAlertLayer::create(nullptr, "Error", "Failed to import", "OK", nullptr, 320.f, false, 0)->show();
			return;
		}
		auto scene = gd::EditLevelLayer::scene(level);
		CCDirector::sharedDirector()->pushScene(scene);
	}
}

bool __fastcall LevelBrowserLayer::initH(gd::LevelBrowserLayer* self, void*, gd::GJSearchObject* obj) {
    if (!LevelBrowserLayer::init(self, obj)) return false;
    levelBrowserLayer = self;

    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    if (!(obj->m_type == gd::SearchType::MapPack || obj->m_type == gd::SearchType::MyLevels || obj->m_type == gd::SearchType::SavedLevels || obj->m_type == gd::SearchType::MapPackOnClick)) {
		m_moveToFirst = nullptr;
		m_moveToLast = nullptr;
		m_moveToPageButtonSprite = nullptr;

        auto menu = CCMenu::create();
        auto sprite = CCSprite::createWithSpriteFrameName("GJ_updateBtn_001.png");
        auto button = gd::CCMenuItemSpriteExtra::create(sprite, nullptr, self, menu_selector(LevelBrowserLayer::Callback::onRefresh));

        menu->setPosition({ director->getScreenRight() - 27.f, director->getScreenBottom() + 27.f });
        menu->setZOrder(5);

        menu->addChild(button);
        self->addChild(menu);
    }
    else if (obj->m_type == gd::SearchType::MyLevels || obj->m_type == gd::SearchType::SavedLevels) {
        auto menu = CCMenu::create();
        auto currentPage = (from<int>(self, 0x140)) / 10 + 1;
		auto totalItems = from<int>(self, 0x13c);
        m_moveToPageButtonSprite = gd::ButtonSprite::create("", 0x18, 0, 0.7f, true, "bigFont.fnt", "GJ_button_02.png", 40.f);
        updateButtonLabel();
        m_moveToPageButtonSprite->setScale(0.75f);
        auto moveToPage = gd::CCMenuItemSpriteExtra::create(m_moveToPageButtonSprite, nullptr, self, menu_selector(GoToPagePopup::showCallback));
        moveToPage->setPosition(menu->convertToNodeSpace({director->getScreenRight() - 25.f, director->getScreenTop() - 40.f}));
        menu->addChild(moveToPage);

        auto moveToLastSpr = CCSprite::createWithSpriteFrameName("edit_rightBtn2_001.png");
        m_moveToLast = gd::CCMenuItemSpriteExtra::create(moveToLastSpr, nullptr, self, menu_selector(LevelBrowserLayer::Callback::onLastPage));
        m_moveToLast->setPosition(menu->convertToNodeSpace({ director->getScreenRight() - 25.f, director->getScreenTop() - 70.f }));
        menu->addChild(m_moveToLast);
		if (totalItems < 11) {
			m_moveToLast->setVisible(0);
		}

        auto moveToFirstSpr = CCSprite::createWithSpriteFrameName("edit_leftBtn2_001.png");
        m_moveToFirst = gd::CCMenuItemSpriteExtra::create(moveToFirstSpr, nullptr, self, menu_selector(LevelBrowserLayer::Callback::onFirstPage));
        m_moveToFirst->setPosition(menu->convertToNodeSpace({ director->getScreenLeft() + 25.f, director->getScreenTop() - 70.f }));
        menu->addChild(m_moveToFirst);
		if (currentPage == 1) {
			m_moveToFirst->setVisible(0);
		}

        self->addChild(menu);
    }
	if (obj->m_type == gd::SearchType::MyLevels) {
		auto menu = CCMenu::create();
		menu->setPosition(CCDirector::sharedDirector()->getWinSize().width - 30.f, 90);
		self->addChild(menu);

		auto btn_spr = CCSprite::createWithSpriteFrameName("GJ_downloadBtn_001.png");
		if (!btn_spr->initWithFile("BE_Import_File.png")) {
			btn_spr->initWithSpriteFrameName("GJ_downloadBtn_001.png");
		}

		auto button = gd::CCMenuItemSpriteExtra::create(btn_spr, nullptr, self, menu_selector(LevelBrowserLayer::Callback::onImportLevel));

		menu->setZOrder(1);
		menu->addChild(button);
	}

    return true;
}

void __fastcall LevelBrowserLayer::onNextPageH(gd::LevelBrowserLayer* self, void*, CCObject* sender) {
    LevelBrowserLayer::onNextPage(self, sender);
	if (m_moveToPageButtonSprite != nullptr)
		updateButtonLabel();
	if (m_moveToFirst != nullptr) {
		auto currentPage = (from<int>(self, 0x140)) / 10 + 1;
		if (currentPage > 1) {
			m_moveToFirst->setVisible(1);
		}
	}
	if (m_moveToLast != nullptr) {
		auto searchObject = from<gd::GJSearchObject*>(self, 0x134);
		auto totalItems = from<int>(self, 0x13c);
		if (searchObject->m_page == (from<int>(self, 0x13c) / 10)) {
				m_moveToLast->setVisible(0);
		}
	}
    std::cout << from<gd::GJSearchObject*>(self, 0x134)->m_page << std::endl;
}

void __fastcall LevelBrowserLayer::onPrevPageH(gd::LevelBrowserLayer* self, void*, CCObject* sender) {
    LevelBrowserLayer::onPrevPage(self, sender);
	if (m_moveToPageButtonSprite != nullptr)
		updateButtonLabel();
	if (m_moveToFirst != nullptr) {
		auto currentPage = (from<int>(self, 0x140)) / 10 + 1;
		if (currentPage == 1) {
			m_moveToFirst->setVisible(0);
		}
	}
	if (m_moveToLast != nullptr) {
		auto totalItems = from<int>(self, 0x13c);
		if (totalItems > 10) {
			m_moveToLast->setVisible(1);
		}
	}
    std::cout << from<gd::GJSearchObject*>(self, 0x134)->m_page << std::endl;
}

void __fastcall LevelBrowserLayer::dtorH(gd::LevelBrowserLayer* self) {
	m_moveToPageButtonSprite = nullptr;
	m_moveToFirst = nullptr;
	m_moveToLast = nullptr;
	LevelBrowserLayer::dtor(self);
	std::cout << "LevelBrowserLayer::dtor" << std::endl;
}

void LevelBrowserLayer::mem_init() {
    MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x89590), LevelBrowserLayer::initH, reinterpret_cast<void**>(&LevelBrowserLayer::init));
    MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x8ac70), LevelBrowserLayer::onNextPageH, reinterpret_cast<void**>(&LevelBrowserLayer::onNextPage));
    MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x8ad10), LevelBrowserLayer::onPrevPageH, reinterpret_cast<void**>(&LevelBrowserLayer::onPrevPage));
    MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x89330), LevelBrowserLayer::dtorH, reinterpret_cast<void**>(&LevelBrowserLayer::dtor));
}