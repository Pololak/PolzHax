#include "InfoLayer.hpp"
#include "utils.hpp"

CCLayer* extendedLevelInfo;
gd::InfoLayer* infoLayer;
CCLayer* copiedToClipboardPopup;
extension::CCScale9Sprite* copiedLabel = nullptr;

class ExtendedLevelInfo : public CCLayer {
protected:
	bool init(gd::GJGameLevel* gameLevel) {
		if (!CCLayer::init()) return false;
		const float width = 440, height = 290;
		CCLayerColor* CCLayerCol = CCLayerColor::create(ccc4(0, 0, 0, 75));
		CCLayerCol->setZOrder(-1);
		CCLayerCol->setScale(10.f);
		this->addChild(CCLayerCol);

		auto touchDispatcher = CCDirector::sharedDirector()->m_pTouchDispatcher;
		touchDispatcher->incrementForcePrio();
		this->registerWithTouchDispatcher();
		setTouchEnabled(true);
		setKeypadEnabled(true);
		setMouseEnabled(true);

		auto director = CCDirector::sharedDirector();
		auto winSize = director->getWinSize();

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
		bg->setPosition(director->getWinSize().width / 2, director->getWinSize().height / 2);
		bg->setZOrder(2);
		this->addChild(bg);

		auto menu = CCMenu::create();
		menu->setZOrder(3);
		this->addChild(menu);

		auto closeSpr = CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
		auto closeBtn = gd::CCMenuItemSpriteExtra::create(
			closeSpr,
			nullptr,
			this,
			menu_selector(ExtendedLevelInfo::onClose));
		closeBtn->setPosition(-((width / 2)) + 10, (height / 2) - 10);

		menu->addChild(closeBtn);

		auto levelNameLabel = CCLabelBMFont::create("", "bigFont.fnt");
		levelNameLabel->setString(CCString::createWithFormat("%s", gameLevel->m_levelName.c_str())->getCString());
		levelNameLabel->limitLabelWidth(300.f, 1.f, 0.1f);
		levelNameLabel->setPosition({ winSize.width / 2.f, winSize.height / 2.f + 125.f });
		levelNameLabel->setZOrder(5);
		this->addChild(levelNameLabel);

		auto descBG = extension::CCScale9Sprite::create("square02b_001.png");
		descBG->setContentSize({ 340.f, 55.f });
		descBG->setColor({ 130, 64, 33 });
		descBG->setZOrder(2);
		descBG->setPosition({ winSize.width / 2.f, winSize.height / 2.f + 52.f });
		this->addChild(descBG);

		//auto levelDesc = gd::TextArea::create("ok", "chatFont.fnt", 1.f, 300.f, { 0.5f, 0.5f }, 10.f, false);
		//levelDesc->setZOrder(3);
		//levelDesc->setPosition({ winSize.width / 2.f, winSize.height / 2.f + 52.f });
		//this->addChild(levelDesc);

		auto moreBG = extension::CCScale9Sprite::create("square02b_001.png");
		moreBG->setContentSize({ 340.f, 140.f });
		moreBG->setColor({ 130, 64, 33 });
		moreBG->setZOrder(2);
		moreBG->setPosition({ winSize.width / 2.f, winSize.height / 2.f - 52.f });
		this->addChild(moreBG);

		auto uploadedLabel = CCLabelBMFont::create("", "chatFont.fnt");

		return true;
	}

	void keyBackClicked() override {
		this->removeFromParentAndCleanup(true);
		extendedLevelInfo = nullptr;
	}

	void onClose(CCObject*) {
		this->keyBackClicked();
	}

public:
	static ExtendedLevelInfo* create(gd::GJGameLevel* gameLevel) {
		ExtendedLevelInfo* obj = new ExtendedLevelInfo;
		if (obj && obj->init(gameLevel)) {
			obj->autorelease();
			return obj;
		}
		CC_SAFE_DELETE(obj);
		return nullptr;
	}
};

void __fastcall InfoLayer::onLevelInfoH(gd::InfoLayer* self, void*, CCObject* obj) {
	auto gameLevel = from<gd::GJGameLevel*>(self, 0x1c8);
	
	std::cout << "Level Name: " << gameLevel->m_levelName.c_str() << std::endl;
	std::cout << "Level Desc: " << gameLevel->m_levelDesc.c_str() << std::endl;
	std::cout << "Username: " << gameLevel->m_userName.c_str() << std::endl;
	std::cout << "UserID: " << gameLevel->m_userID << std::endl;
	std::cout << "Uploaded: " << gameLevel->m_uploadDate.c_str() << std::endl;
	std::cout << "Updated: " << gameLevel->m_updateDate.c_str() << std::endl;
	std::cout << "Two-Player Mode: " << gameLevel->getUserID() << std::endl;

	auto extendedInfo = ExtendedLevelInfo::create(gameLevel);
	extendedInfo->setZOrder(600);
	CCDirector::sharedDirector()->getRunningScene()->addChild(extendedInfo);
	extendedLevelInfo = extendedInfo;
}

void InfoLayer::Callback::onRefreshComments(CCObject*) {
	auto glm = gd::GameLevelManager::sharedState();
	glm->makeTimeStamp("upd_comments");
	glm->resetTimerForKey(infoLayer->m_commentsKey.c_str());
	infoLayer->loadPage(infoLayer->m_page);
}

void InfoLayer::Callback::onCopyLevelID(CCObject*) {
	clipboard::write(std::to_string(from<gd::GJGameLevel*>(infoLayer, 0x1c8)->m_levelID).c_str());
	copiedLabel->setOpacity(100);
	copiedLabel->stopAllActions();
	copiedLabel->runAction(
		CCSequence::create(CCDelayTime::create(0.5f), CCFadeTo::create(.5f, 0), nullptr)
	);
	auto label = (CCLabelBMFont*)copiedLabel->getChildren()->objectAtIndex(1);
	label->setOpacity(255);
	label->stopAllActions();
	label->runAction(
		CCSequence::create(CCDelayTime::create(0.5f), CCFadeOut::create(0.5f), nullptr)
	);
}

bool __fastcall InfoLayer::initH(gd::InfoLayer* self, void*, gd::GJGameLevel* gameLevel) {
	if (!InfoLayer::init(self, gameLevel)) return false;
	infoLayer = self;

	auto director = CCDirector::sharedDirector();
	auto winSize = director->getWinSize();

	auto menu = self->m_buttonMenu;

	auto sprite = CCSprite::createWithSpriteFrameName("GJ_updateBtn_001.png");
	sprite->setScale(.7f);
	auto button = gd::CCMenuItemSpriteExtra::create(sprite, nullptr, self, menu_selector(InfoLayer::Callback::onRefreshComments));
	button->setPosition({ 406.f, -134.f });

	menu->addChild(button);

	reinterpret_cast<CCLabelBMFont*>(self->m_mainLayer->getChildren()->objectAtIndex(6))->setVisible(0);
	auto idLabel = CCLabelBMFont::create("ID:", "goldFont.fnt");
	idLabel->setString(CCString::createWithFormat("ID: %i", gameLevel->m_levelID)->getCString());
	idLabel->setScale(0.6f);
	auto idButton = gd::CCMenuItemSpriteExtra::create(idLabel, nullptr, self, menu_selector(InfoLayer::Callback::onCopyLevelID));
	idButton->setPosition({ 260.f, -266.f });
	menu->addChild(idButton);

	copiedLabel = extension::CCScale9Sprite::create("square02_small.png");
	copiedLabel->setContentSize({ 200.f, 30.f });
	copiedLabel->setOpacity(0);
	copiedLabel->setPosition({ winSize / 2.f });
	copiedLabel->setZOrder(999);
	auto label = CCLabelBMFont::create("Copied To Clipboard", "bigFont.fnt");
	label->setScale(0.5f);
	label->setPosition({ copiedLabel->getContentSize() / 2.f });
	label->setZOrder(1000);
	label->setOpacity(0);
	copiedLabel->addChild(label);
	self->m_mainLayer->addChild(copiedLabel);

	return true;
}