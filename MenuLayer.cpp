#include "MenuLayer.hpp"

class RandomPopup : public gd::FLAlertLayer {
public:
	bool init() override {
		if (!this->initWithColor({ 0, 0, 0, 75 })) return false;

		auto* director = CCDirector::sharedDirector();
		director->getTouchDispatcher()->incrementForcePrio();
		this->registerWithTouchDispatcher();

		auto layer = CCLayer::create();
		auto menu = CCMenu::create();
		this->m_mainLayer = layer;
		this->m_buttonMenu = menu;

		layer->addChild(menu);
		this->addChild(layer);

		menu->addChild(gd::CCMenuItemSpriteExtra::create(CCSprite::create("GJ_button_01.png"), this, nullptr));

		this->setKeypadEnabled(true);
		this->setTouchEnabled(true);

		return true;
	}

	static RandomPopup* create() {
		RandomPopup* ret = new RandomPopup();
		if (ret && ret->init()) {
			ret->autorelease();
			return ret;
		}
		CC_SAFE_DELETE(ret);
		return nullptr;
	}

	virtual void keyBackClicked() {
		this->setTouchEnabled(false);
		this->setKeypadEnabled(false);
		this->removeFromParentAndCleanup(true);
	}

	void popup(CCObject*) {
		RandomPopup::create()->show();
	}
};

bool __fastcall MenuLayer::initH(gd::MenuLayer* self, void*) {
	if (!MenuLayer::init(self)) return false;

	auto director = CCDirector::sharedDirector();

	std::cout << director << std::endl;
	std::cout << director->m_pTouchDispatcher << std::endl;

	auto gm = gd::GameManager::sharedState();

	std::cout << gm << "\n";
	std::cout << gm->m_playerName << "\n";

	auto menu = CCMenu::create();
	self->addChild(menu);

	auto buttonSpr = gd::ButtonSprite::create("Wow", 0, 0, 1.f, false, "goldFont.fnt", "GJ_button_01.png", 32.f);
	self->addChild(buttonSpr);

	auto button = gd::CCMenuItemSpriteExtra::create(CCSprite::create("GJ_button_01.png"), self, menu_selector(RandomPopup::popup));
	button->setPositionX(-170.f);
	menu->addChild(button);

	auto input = gd::CCTextInputNode::create(60.f, 30.f, "Something", self, "bigFont.fnt");
	input->setAllowedChars("abcdef");
	self->addChild(input);

	std::cout << gd::GameLevelManager::sharedState() << std::endl;
	std::cout << gd::GameLevelManager::sharedState()->m_mainLevels << std::endl;

	std::cout << gd::LocalLevelManager::sharedState() << std::endl;

	auto checkpoint = CCSprite::createWithSpriteFrameName("checkpoint_01_001.png");
	auto achi = gd::AchievementBar::create("ayo", "ayo", nullptr);
	auto iconSprite = achi->m_layerColor->getChildByType<CCSprite*>(0);
	achi->m_layerColor->addChild(checkpoint);

	if (iconSprite) {
		checkpoint->setPosition(iconSprite->getPosition());
	}

	checkpoint->setZOrder(4);

	auto an = gd::AchievementNotifier::sharedState();
	an->m_achievementBarArray->addObject(achi);
	if (an->m_activeAchievementBar == nullptr) {
		an->showNextAchievement();
	}

	return true;
}

void MenuLayer::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xaf210), MenuLayer::initH, reinterpret_cast<void**>(&MenuLayer::init));
}