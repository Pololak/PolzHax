#include "MenuLayer.h"
#include "Icons.h"

#include "GameVariables.hpp"
#include "nodes.hpp"

bool isReload = false;

class ReloadTexAlertProtocol : public gd::FLAlertLayerProtocol {
protected:

	void FLAlert_Clicked(gd::FLAlertLayer* layer, bool btn2) override
	{
		if (btn2)
		{
			isReload = true;

			gd::GameManager::sharedState()->reloadAll(false, false, true);
		}
	}
};

ReloadTexAlertProtocol raProtocol;

void MenuLayer::Callback::onReload(CCObject*) {
	gd::FLAlertLayer::create(&raProtocol, "Reload Textures", "Are you sure you want to <cj>reload textures</c>?", "Cancel", "Yes", 300.f, false, 0)->show();
}

static CCScene* scene(bool p0) {
	if (!isReload) return gd::MenuLayer::scene(p0);
	else {
		isReload = false;
		return gd::MenuLayer::scene(false);
	}
}

bool __fastcall MenuLayer::init_H(gd::MenuLayer* self, void* edx) {
	if (!MenuLayer::init(self)) return false;

	auto manager = gd::GameManager::sharedState();
	auto director = CCDirector::sharedDirector();
	auto menu = CCMenu::create();
	menu->setPosition(0, 0);

	auto reloadSpr = CCSprite::createWithSpriteFrameName("GJ_replayBtn_001.png");
	reloadSpr->setScale(0.525f);
	auto reloadBtn = gd::CCMenuItemSpriteExtra::create(reloadSpr, nullptr, self, menu_selector(MenuLayer::Callback::onReload));
	reloadBtn->setPosition({ director->getScreenLeft() + 18.750f, director->getScreenTop() - 55 });

	menu->addChild(reloadBtn);
	self->addChild(menu);

	Icons::patchCube(Icons::getCount("player", "001"));
	Icons::patchShip(Icons::getCount("ship", "001"));
	Icons::patchBall(Icons::getCount("player_ball", "001"));
	Icons::patchBird(Icons::getCount("bird", "001"));
	Icons::patchDart(Icons::getCount("dart", "001"), setting().selected_dart);
	//Icons::patchTrail(Icons::getCount("player_special", "001"));

	//auto inputNode = gd::CCTextInputNode::create("X", self, "bigFont.fnt", 30.f, 20.f);
	//self->addChild(inputNode);

	auto t = std::time(nullptr);
	auto tm = *std::localtime(&t);

	switch (tm.tm_mon)
	{
	case 0:
	case 1:
	case 11:
		CCParticleSnow * snow = CCParticleSnow::createWithTotalParticles(700);
		self->addChild(snow);
		break;
	}

	return true;
}

void __fastcall MenuGameLayer::tryJumpH(gd::MenuGameLayer* self, void* edx, float idk) {

}

void __fastcall MenuGameLayer::updateH(gd::MenuGameLayer* self, void* edx, float dt) {
	if (self->getPlayerObject()->isRoll() && GetAsyncKeyState(KEY_W)) self->destroyPlayer(); // Destroys player if it's ball gamemode.

	if ((GetAsyncKeyState(KEY_W))) {
		self->getPlayerObject()->pushButton(gd::PlayerButton::Jump);
	}
	else {
		self->getPlayerObject()->releaseButton(gd::PlayerButton::Jump);
	}

	MenuGameLayer::update(self, dt);
}

void MenuLayer::mem_init() {
	MH_CreateHook(
		reinterpret_cast<void*>(gd::base + 0xaf210),
		MenuLayer::init_H,
		reinterpret_cast<void**>(&MenuLayer::init));
}

void MenuGameLayer::mem_init() {
	//MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xadff0), MenuGameLayer::tryJumpH, reinterpret_cast<void**>(&MenuGameLayer::tryJump));
	//MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xae210), MenuGameLayer::updateH, reinterpret_cast<void**>(&MenuGameLayer::update));
}