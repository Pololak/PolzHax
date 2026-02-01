#include "GJScoreCell.hpp"

class PlayerScorePopup : public gd::FLAlertLayer {
protected:
	gd::GJUserScore* m_userScore;

	virtual bool init(gd::GJUserScore* userScore) {
		m_userScore = userScore;
		if (!this->initWithColor({ 0, 0, 0, 75 })) return false;

		auto* director = CCDirector::sharedDirector();
		auto winSize = director->getWinSize();
		director->getTouchDispatcher()->incrementForcePrio();
		this->registerWithTouchDispatcher();

		auto layer = CCLayer::create();
		auto menu = CCMenu::create();
		this->m_mainLayer = layer;
		this->m_buttonMenu = menu;

		layer->addChild(menu);
		this->addChild(layer);

		auto bg = extension::CCScale9Sprite::create("GJ_square01.png");
		bg->setContentSize({ 260.f, 160.f });
		bg->setPosition({ winSize.width / 2.f, winSize.height / 2.f });
		layer->addChild(bg, -2);

		auto onCloseSpr = CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
		onCloseSpr->setScale(.8f);
		auto onClose = gd::CCMenuItemSpriteExtra::create(onCloseSpr, this, menu_selector(PlayerScorePopup::onClose));
		onClose->setPosition(-126.f, 74.5f);
		menu->addChild(onClose);

		auto gm = gd::GameManager::sharedState();

		auto playerNameBg = extension::CCScale9Sprite::create("square02_small.png");
		playerNameBg->setPosition(winSize.width / 2.f + 35.f, winSize.height / 2.f + 58.f);
		playerNameBg->setContentSize({ 220.f, 32.f });
		playerNameBg->setOpacity(75);
		playerNameBg->setScale(.8f);
		layer->addChild(playerNameBg, -1);

		auto playerNameLabel = CCLabelBMFont::create("", "bigFont.fnt");
		playerNameLabel->setPosition(playerNameBg->getPositionX(), playerNameBg->getPositionY() + 2.f);
		playerNameLabel->setString(userScore->m_userName.c_str());
		playerNameLabel->limitLabelWidth(170.f, .8f, .1f);
		layer->addChild(playerNameLabel);

		auto playerBg = extension::CCScale9Sprite::create("square02_001.png");
		playerBg->setPosition(winSize.width / 2.f - 84.f, winSize.height / 2.f + 32.f);
		playerBg->setContentSize({ 55.f, 55.f });
		playerBg->setOpacity(75);
		layer->addChild(playerBg, -1);

		auto playerPreview = gd::SimplePlayer::create(userScore->m_iconID);
		playerPreview->setPosition(playerBg->getPosition());
		playerPreview->updatePlayerFrame(userScore->m_iconID, userScore->m_iconType);
		playerPreview->setColor(gm->colorForIdx(userScore->m_color1));
		playerPreview->setSecondColor(gm->colorForIdx(userScore->m_color2));
		playerPreview->m_special = userScore->m_special;
		playerPreview->updateColors();
		layer->addChild(playerPreview);

		auto playerRankIcon = CCSprite::createWithSpriteFrameName("GJ_featuredIcon_001.png");
		playerRankIcon->setPosition({ winSize.width / 2.f - 110.f, winSize.height / 2.f - 10.f });
		layer->addChild(playerRankIcon);

		auto playerRankLabel = CCLabelBMFont::create("", "bigFont.fnt");
		playerRankLabel->setPosition(winSize.width / 2.f - 95.f, winSize.height / 2.f - 10.f);
		playerRankLabel->setAnchorPoint({ 0.f, .5f });
		playerRankLabel->setString(CCString::createWithFormat("%i", userScore->m_playerRank)->getCString());
		playerRankLabel->limitLabelWidth(35.f, .5f, .1f);
		layer->addChild(playerRankLabel);

		//

		auto starIcon = CCSprite::createWithSpriteFrameName("GJ_starsIcon_001.png");
		starIcon->setPosition({ winSize.width / 2.f - 35.f, winSize.height / 2.f + 25.f });
		layer->addChild(starIcon);

		auto starsLabel = CCLabelBMFont::create("", "bigFont.fnt");
		starsLabel->setPosition(starIcon->getPositionX() + 20.f, starIcon->getPositionY());
		starsLabel->setAnchorPoint({ 0.f, .5f });
		if (userScore->m_stars == -1) {
			starsLabel->setString("NA");
		}
		else {
			starsLabel->setString(CCString::createWithFormat("%i", userScore->m_stars)->getCString());
		}
		starsLabel->limitLabelWidth(130.f, .65f, .1f);
		layer->addChild(starsLabel);

		//

		auto coinIcon = CCSprite::createWithSpriteFrameName("GJ_coinsIcon_001.png");
		coinIcon->setPosition({ winSize.width / 2.f - 35.f, winSize.height / 2.f - 2.5f });
		layer->addChild(coinIcon);

		auto coinsLabel = CCLabelBMFont::create("", "bigFont.fnt");
		coinsLabel->setPosition(coinIcon->getPositionX() + 20.f, coinIcon->getPositionY());
		coinsLabel->setAnchorPoint({ 0.f, .5f });
		if (userScore->m_secretCoins == -1) {
			coinsLabel->setString("NA");
		}
		else {
			coinsLabel->setString(CCString::createWithFormat("%i", userScore->m_secretCoins)->getCString());
		}
		coinsLabel->limitLabelWidth(130.f, .65f, .1f);
		layer->addChild(coinsLabel);

		//

		auto demonIcon = CCSprite::createWithSpriteFrameName("GJ_demonIcon_001.png");
		demonIcon->setPosition({ winSize.width / 2.f - 35.f, winSize.height / 2.f - 30.f });
		layer->addChild(demonIcon);

		auto demonsLabel = CCLabelBMFont::create("", "bigFont.fnt");
		demonsLabel->setPosition(demonIcon->getPositionX() + 20.f, demonIcon->getPositionY());
		demonsLabel->setAnchorPoint({ 0.f, .5f });
		if (userScore->m_demons == -1) {
			demonsLabel->setString("NA");
		}
		else {
			demonsLabel->setString(CCString::createWithFormat("%i", userScore->m_demons)->getCString());
		}
		demonsLabel->limitLabelWidth(130.f, .65f, .1f);
		layer->addChild(demonsLabel);

		//

		auto hammerIcon = CCSprite::createWithSpriteFrameName("GJ_hammerIcon_001.png");
		hammerIcon->setPosition({ winSize.width / 2.f - 35.f, winSize.height / 2.f - 57.5f });
		layer->addChild(hammerIcon);

		auto hammersLabel = CCLabelBMFont::create("", "bigFont.fnt");
		hammersLabel->setPosition(hammerIcon->getPositionX() + 20.f, hammerIcon->getPositionY());
		hammersLabel->setAnchorPoint({ 0.f, .5f });
		if (userScore->m_creatorPoints == -1) {
			hammersLabel->setString("NA");
		}
		else {
			hammersLabel->setString(CCString::createWithFormat("%i", userScore->m_creatorPoints)->getCString());
		}
		hammersLabel->limitLabelWidth(130.f, .65f, .1f);
		layer->addChild(hammersLabel);

		//

		auto onLevelsSpr = CCSprite::create("GJ_button_01.png");
		onLevelsSpr->setScale(.8f);
		auto onLevelSpr02 = CCSprite::createWithSpriteFrameName("GJ_hammerIcon_001.png");
		onLevelSpr02->setPosition(onLevelsSpr->getContentSize() / 2.f);
		onLevelSpr02->setScale(1.25f);
		onLevelsSpr->addChild(onLevelSpr02);
		auto onLevels = gd::CCMenuItemSpriteExtra::create(onLevelsSpr, this, menu_selector(PlayerScorePopup::onLevels));
		onLevels->setPosition(-105.f, -55.f);
		menu->addChild(onLevels);

		this->setTouchEnabled(true);
		this->setKeypadEnabled(true);

		return true;
	}

	void onLevels(CCObject*) {
		if (!m_userScore) return;

		gd::GameManager::sharedState()->m_lastScene2 = static_cast<gd::LastGameScene>(6);
		CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(.5f, gd::LevelBrowserLayer::scene(gd::GJSearchObject::create(gd::SearchType::UsersLevels, CCString::createWithFormat("%i", m_userScore->m_userID)->getCString()))));
	}

	virtual void keyBackClicked() {
		this->removeFromParentAndCleanup(true);
		this->setTouchEnabled(false);
		this->setKeypadEnabled(false);
	}

	void onClose(CCObject*) {
		this->keyBackClicked();
	}

public:
	static PlayerScorePopup* create(gd::GJUserScore* userScore) {
		PlayerScorePopup* ret = new PlayerScorePopup();
		if (ret && ret->init(userScore)) {
			ret->autorelease();
			return ret;
		}
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
};

void GJScoreCell::Callback::onPlayerScore(CCObject*) {
	PlayerScorePopup::create(this->m_userScore)->show();
}

void __fastcall GJScoreCell::loadFromScoreH(gd::GJScoreCell* self, void*, gd::GJUserScore* userScore) {
	GJScoreCell::loadFromScore(self, userScore);

	std::cout << self->m_mainLayer << std::endl;

	auto menu = CCMenu::create();
	menu->setPosition(41.f, 40.f);
	self->m_mainLayer->addChild(menu, 10);

	auto onPlayerScoreSpr = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
	onPlayerScoreSpr->setScale(.6f);
	auto onPlayerScore = gd::CCMenuItemSpriteExtra::create(onPlayerScoreSpr, self, menu_selector(GJScoreCell::Callback::onPlayerScore));
	menu->addChild(onPlayerScore);
}

void GJScoreCell::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x355f0), GJScoreCell::loadFromScoreH, reinterpret_cast<void**>(&GJScoreCell::loadFromScore));
}