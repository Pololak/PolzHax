#include "GJScoreCell.h"

CCLayer* profilePagePopup;
gd::GJUserScore* gjus = nullptr;
cocos2d::ccColor3B firstColor = ccc3(255, 255, 255);
cocos2d::ccColor3B secondColor = ccc3(255, 255, 255);

class ProfilePagePopup : public CCLayer {
protected:
	CCLabelBMFont* m_nameLabel = nullptr;


	bool init(gd::GJUserScore* userScore) {
		if (!CCLayer::init()) return false;
		const float width = 260, height = 160;
		CCLayerColor* CCLayerCol = CCLayerColor::create(ccc4(0, 0, 0, 0));
		CCLayerCol->setZOrder(-1);
		CCLayerCol->setScale(10.f);
		this->addChild(CCLayerCol);
		auto actionColor = CCFadeTo::create(0.1f, 75);
		CCLayerCol->runAction(actionColor);

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

		auto appearAction = CCEaseElasticOut::create(CCScaleTo::create(.5f, 1.f), .6f);

		auto menu = CCMenu::create();
		menu->setZOrder(3);
		this->addChild(menu);

		auto closeSpr = CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
		closeSpr->setScale(0.8f);
		auto closeBtn = gd::CCMenuItemSpriteExtra::create(
			closeSpr,
			nullptr,
			this,
			menu_selector(ProfilePagePopup::onClose));
		closeBtn->setPosition(-((width / 2)), (height / 2));

		menu->addChild(closeBtn);

		auto playerBG = extension::CCScale9Sprite::create("square02_001.png");
		playerBG->setOpacity(75);
		playerBG->setZOrder(2);
		playerBG->setPosition({ winSize.width / 2.f - 90.f, winSize.height / 2.f + 40.f });
		playerBG->setContentSize({ 55.f, 55.f });
		this->addChild(playerBG);

		auto simplePlayer = gd::SimplePlayer::create(userScore->icon());
		simplePlayer->updatePlayerFrame(userScore->icon(), userScore->iconType());
		simplePlayer->setZOrder(3);
		simplePlayer->setPosition({ winSize.width / 2.f - 90.f, winSize.height / 2.f + 40.f });

		if (userScore->special() == 2) {
			simplePlayer->glowSpr()->setVisible(1);
		}
		if (userScore->color1() == 15) {
			simplePlayer->glowSpr()->setVisible(1);
		}
		simplePlayer->baseSpr()->setColor(firstColor);
		simplePlayer->colorSpr()->setColor(secondColor);
		simplePlayer->glowSpr()->setColor(secondColor);

		this->addChild(simplePlayer);

		auto nameLabel = CCLabelBMFont::create("", "bigFont.fnt");
		nameLabel->setString(CCString::create(userScore->userName())->getCString());
		nameLabel->limitLabelWidth(170.f, 0.8f, 0.4f);
		nameLabel->setZOrder(3);
		nameLabel->setPosition({ winSize.width / 2.f + 30.f, winSize.height / 2.f + 60.f });
		this->addChild(nameLabel);

		auto featuredIcon = CCSprite::createWithSpriteFrameName("GJ_featuredIcon_001.png");
		featuredIcon->setZOrder(3);
		featuredIcon->setPosition({ winSize.width / 2.f - 110.f, winSize.height / 2.f - 5.f });
		this->addChild(featuredIcon);

		auto rankLabel = CCLabelBMFont::create("", "bigFont.fnt");
		rankLabel->setString(CCString::create(std::to_string(userScore->globalRank()).c_str())->getCString());
		rankLabel->setZOrder(3);
		rankLabel->setAnchorPoint({ 0.f, 0.5f });
		rankLabel->limitLabelWidth(30.f, 0.6f, 0.3f);
		rankLabel->setPosition({ featuredIcon->getPositionX() + 15.f, featuredIcon->getPositionY() });
		this->addChild(rankLabel);

		auto levelsSprite = CCSprite::create("GJ_button_01.png");
		auto hammerSprite = CCSprite::createWithSpriteFrameName("GJ_hammerIcon_001.png");
		hammerSprite->setPosition({ levelsSprite->getContentSize() / 2.f });
		levelsSprite->setScale(0.8f);
		hammerSprite->setScale(1.25f);
		levelsSprite->addChild(hammerSprite);
		auto levelsButton = gd::CCMenuItemSpriteExtra::create(levelsSprite, nullptr, this, menu_selector(ProfilePagePopup::onLevels));
		levelsButton->setPosition({ -105.f, -55.f });
		menu->addChild(levelsButton);

		auto starIcon = CCSprite::createWithSpriteFrameName("GJ_starsIcon_001.png");
		starIcon->setPosition({ winSize.width / 2.f + 105.f, winSize.height / 2.f + 30.f });
		starIcon->setZOrder(3);
		this->addChild(starIcon);

		auto starCount = CCLabelBMFont::create("", "bigFont.fnt");
		starCount->setString(CCString::create(std::to_string(userScore->stars()).c_str())->getCString());
		starCount->setZOrder(3);
		starCount->setPosition({ starIcon->getPositionX() - 15.f, starIcon->getPositionY() });
		starCount->limitLabelWidth(130.f, 0.6f, 0.6f);
		starCount->setAnchorPoint({ 1.f, 0.5f });
		this->addChild(starCount);

		auto coinIcon = CCSprite::createWithSpriteFrameName("GJ_coinsIcon_001.png");
		coinIcon->setPosition({ starIcon->getPositionX(), starIcon->getPositionY() - 27.f });
		coinIcon->setZOrder(3);
		this->addChild(coinIcon);

		auto coinCount = CCLabelBMFont::create("", "bigFont.fnt");
		if (userScore->scoreType() == 1) {
			coinCount->setString(CCString::create("NA")->getCString());
		}
		else {
			coinCount->setString(CCString::create(std::to_string(userScore->secretCoins()).c_str())->getCString());
		}
		coinCount->setZOrder(3);
		coinCount->setPosition({ coinIcon->getPositionX() - 15.f, coinIcon->getPositionY() });
		coinCount->limitLabelWidth(130.f, 0.6f, 0.6f);
		coinCount->setAnchorPoint({ 1.f, 0.5f });
		this->addChild(coinCount);

		auto demonIcon = CCSprite::createWithSpriteFrameName("GJ_demonIcon_001.png");
		demonIcon->setPosition({ coinIcon->getPositionX(), coinIcon->getPositionY() - 27.f });
		demonIcon->setZOrder(3);
		this->addChild(demonIcon);

		auto demonCount = CCLabelBMFont::create("", "bigFont.fnt");
		demonCount->setString(CCString::create(std::to_string(userScore->demons()).c_str())->getCString());
		demonCount->setZOrder(3);
		demonCount->setPosition({ demonIcon->getPositionX() - 15.f, demonIcon->getPositionY() });
		demonCount->limitLabelWidth(130.f, 0.6f, 0.6f);
		demonCount->setAnchorPoint({ 1.f, 0.5f });
		this->addChild(demonCount);

		auto hammerIcon = CCSprite::createWithSpriteFrameName("GJ_hammerIcon_001.png");
		hammerIcon->setPosition({ demonIcon->getPositionX(), demonIcon->getPositionY() - 27.f });
		hammerIcon->setZOrder(3);
		this->addChild(hammerIcon);

		auto hammerCount = CCLabelBMFont::create("", "bigFont.fnt");
		hammerCount->setString(CCString::create(std::to_string(userScore->creatorPoints()).c_str())->getCString());
		hammerCount->setZOrder(3);
		hammerCount->setPosition({ hammerIcon->getPositionX() - 15.f, hammerIcon->getPositionY() });
		hammerCount->limitLabelWidth(130.f, 0.6f, 0.6f);
		hammerCount->setAnchorPoint({ 1.f, 0.5f });
		this->addChild(hammerCount);

		auto userIDLabel = CCLabelBMFont::create("ID: ", "chatFont.fnt");
		userIDLabel->setString(CCString::createWithFormat("ID: %i", userScore->userID())->getCString());
		userIDLabel->setAnchorPoint({ 0.f, 0.5f });
		userIDLabel->setScale(0.6f);
		userIDLabel->setOpacity(75);
		userIDLabel->setColor({ 0, 0, 0 });
		userIDLabel->setPosition({ winSize.width / 2.f - 85.f, winSize.height / 2.f - 69.f });
		userIDLabel->setZOrder(3);
		this->addChild(userIDLabel);

		auto iconTypeLabel = CCLabelBMFont::create("IType: ", "chatFont.fnt");
		iconTypeLabel->setString(CCString::createWithFormat("IType: %i", userScore->iconType())->getCString());
		iconTypeLabel->setAnchorPoint({ 0.f, 0.5f });
		iconTypeLabel->setScale(0.6f);
		iconTypeLabel->setOpacity(75);
		iconTypeLabel->setColor({ 0, 0, 0 });
		iconTypeLabel->setPosition({ winSize.width / 2.f - 40.f, winSize.height / 2.f - 69.f });
		iconTypeLabel->setZOrder(3);
		this->addChild(iconTypeLabel);

		auto iconLabel = CCLabelBMFont::create("I: ", "chatFont.fnt");
		iconLabel->setString(CCString::createWithFormat("I: %i", userScore->icon())->getCString());
		iconLabel->setAnchorPoint({ 0.f, 0.5f });
		iconLabel->setScale(0.6f);
		iconLabel->setOpacity(75);
		iconLabel->setColor({ 0, 0, 0 });
		iconLabel->setPosition({ winSize.width / 2.f - 5.f, winSize.height / 2.f - 69.f });
		iconLabel->setZOrder(3);
		this->addChild(iconLabel);

		auto primaryColorLabel = CCLabelBMFont::create("P1: ", "chatFont.fnt");
		primaryColorLabel->setString(CCString::createWithFormat("P1: %i", userScore->color1())->getCString());
		primaryColorLabel->setAnchorPoint({ 0.f, 0.5f });
		primaryColorLabel->setScale(0.6f);
		primaryColorLabel->setOpacity(75);
		primaryColorLabel->setColor({ 0, 0, 0 });
		primaryColorLabel->setPosition({ winSize.width / 2.f + 15.f, winSize.height / 2.f - 69.f });
		primaryColorLabel->setZOrder(3);
		this->addChild(primaryColorLabel);

		auto secondaryColorLabel = CCLabelBMFont::create("P2: ", "chatFont.fnt");
		secondaryColorLabel->setString(CCString::createWithFormat("P2: %i", userScore->color2())->getCString());
		secondaryColorLabel->setAnchorPoint({ 0.f, 0.5f });
		secondaryColorLabel->setScale(0.6f);
		secondaryColorLabel->setOpacity(75);
		secondaryColorLabel->setColor({ 0, 0, 0 });
		secondaryColorLabel->setPosition({ winSize.width / 2.f + 45.f, winSize.height / 2.f - 69.f });
		secondaryColorLabel->setZOrder(3);
		this->addChild(secondaryColorLabel);

		auto specialLabel = CCLabelBMFont::create("S: ", "chatFont.fnt");
		specialLabel->setString(CCString::createWithFormat("S: %i", userScore->special())->getCString());
		specialLabel->setAnchorPoint({ 0.f, 0.5f });
		specialLabel->setScale(0.6f);
		specialLabel->setOpacity(75);
		specialLabel->setColor({ 0, 0, 0 });
		specialLabel->setPosition({ winSize.width / 2.f + 75.f, winSize.height / 2.f - 69.f });
		specialLabel->setZOrder(3);
		this->addChild(specialLabel);

		auto scoreTypeLabel = CCLabelBMFont::create("ST: ", "chatFont.fnt");
		scoreTypeLabel->setString(CCString::createWithFormat("ST: %i", userScore->scoreType())->getCString());
		scoreTypeLabel->setAnchorPoint({ 0.f, 0.5f });
		scoreTypeLabel->setScale(0.6f);
		scoreTypeLabel->setOpacity(75);
		scoreTypeLabel->setColor({ 0, 0, 0 });
		scoreTypeLabel->setPosition({ winSize.width / 2.f + 95.f, winSize.height / 2.f - 69.f });
		scoreTypeLabel->setZOrder(3);
		this->addChild(scoreTypeLabel);

		this->setScale(0.1f);
		this->runAction(appearAction);

		return true;
	}

	void keyBackClicked() override {
		this->removeFromParentAndCleanup(true);
		profilePagePopup = nullptr;
		gjus = nullptr;
		firstColor = ccc3(255, 255, 255);
		secondColor = ccc3(255, 255, 255);
	}

	void onClose(CCObject*) {
		this->keyBackClicked();
	}

	void onLevels(CCObject* obj) {
		if (gjus != nullptr) {
			auto searchObject = gd::GJSearchObject::create(gd::SearchType::UsersLevels, std::to_string(gjus->userID()).c_str());
			gd::LevelBrowserLayer::scene(searchObject);
		}
		this->keyBackClicked();
	}

public:
	static ProfilePagePopup* create(gd::GJUserScore* userScore) {
		ProfilePagePopup* obj = new ProfilePagePopup;
		if (obj && obj->init(userScore)) {
			obj->autorelease();
			return obj;
		}
		CC_SAFE_DELETE(obj);
		return nullptr;
	}
};

void GJScoreCell::Callback::onMoreInfo(CCObject* obj) {
	auto scoreCellLayer = reinterpret_cast<gd::GJScoreCell*>(reinterpret_cast<CCNode*>(obj)->getParent())->getParent();
	auto scoreCell = from<gd::GJScoreCell*>(scoreCellLayer, 0xac);
	auto score = scoreCell->userScore();
	gd::SimplePlayer* simplePlayer;
	if (score->scoreType() == 1) {
		simplePlayer = reinterpret_cast<gd::SimplePlayer*>(scoreCell->layer()->getChildren()->objectAtIndex(5));
	}
	else if (scoreCell->layer()->getChildrenCount() < 9) {
		simplePlayer = reinterpret_cast<gd::SimplePlayer*>(scoreCell->layer()->getChildren()->objectAtIndex(4));
	}
	else {
		simplePlayer = reinterpret_cast<gd::SimplePlayer*>(scoreCell->layer()->getChildren()->objectAtIndex(8));
	}
	std::cout << "GJScoreCell: " << scoreCell << std::endl;
	std::cout << "SimplePlayer: " << simplePlayer << std::endl;
	std::cout << "--------------------" << std::endl;
	std::cout << "Score Type: " << score->scoreType() << std::endl;
	std::cout << "Username: " << score->userName().c_str() << std::endl;
	std::cout << "UserID: " << score->userID() << std::endl;
	std::cout << "Rank: " << score->globalRank() << std::endl;
	std::cout << "Stars: " << score->stars() << std::endl;
	std::cout << "Demons: " << score->demons() << std::endl;
	std::cout << "Secret Coins: " << score->secretCoins() << std::endl;
	std::cout << "Creator Points: " << score->creatorPoints() << std::endl;
	std::cout << "IconType: " << score->iconType() << std::endl;
	std::cout << "Icon: " << score->icon() << std::endl;
	std::cout << "Primary Color: " << score->color1() << std::endl;
	std::cout << "Secondary Color: " << score->color2() << std::endl;
	std::cout << "Special: " << score->special() << std::endl;
	std::cout << "--------------------" << std::endl;
	gjus = score;

	if (simplePlayer != nullptr) {
		firstColor = simplePlayer->baseSpr()->getColor();
		secondColor = simplePlayer->colorSpr()->getColor();
	}

	auto profilePage = ProfilePagePopup::create(score);
	profilePage->setZOrder(105);
	CCDirector::sharedDirector()->getRunningScene()->addChild(profilePage);
	profilePagePopup = profilePage;
}

void __fastcall GJScoreCell::loadFromScoreH(gd::GJScoreCell* self, void*, gd::GJUserScore* userScore) {
	GJScoreCell::loadFromScore(self, userScore);

	CCLayer* mainLayer = nullptr;
	CCObject* obj;
	CCARRAY_FOREACH(self->getChildren(), obj) {
		CCLayer* search = static_cast<CCLayer*>(obj);
		if (search != nullptr && search->getChildrenCount() > 2) {
			mainLayer = search;
			break;
		}
	}

	auto menu = CCMenu::create();
	auto infoSprite = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
	infoSprite->setScale(0.6f);
	auto moreButton = gd::CCMenuItemSpriteExtra::create(infoSprite, nullptr, self, menu_selector(GJScoreCell::Callback::onMoreInfo));
	menu->setPosition({ 40.f, 40.f });
	menu->addChild(moreButton);
	menu->setZOrder(5);
	mainLayer->addChild(menu);
}