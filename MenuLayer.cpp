#include "MenuLayer.hpp"
#include "Setting.hpp"
#include "Icons.hpp"
#include "ColorChannelSprite.hpp"
//#include <discord.h>
//
//class DiscordManager {
//public:
//	discord::Core* core{};
//	discord::ClientId id = 1475062646033354843;
//
//	long long timeStart = 0;
//
//	void initDiscord() {
//		if (!core) auto result = discord::Core::Create(id, DiscordCreateFlags_NoRequireDiscord, &core);
//
//		if (!core) return;
//
//		timeStart = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
//	}
//};
//
//DiscordManager ds;

bool __fastcall MenuLayer::initH(gd::MenuLayer* self, void*) {
	if (!MenuLayer::init(self)) return false;

	auto director = CCDirector::sharedDirector();
	auto winSize = director->getWinSize();

	auto gm = gd::GameManager::sharedState();

	Icons::patchCube(Icons::getCount("player", "001"));
	Icons::patchShip(Icons::getCount("ship", "001"));
	Icons::patchBall(Icons::getCount("player_ball", "001"));
	Icons::patchBird(Icons::getCount("bird", "001"));
	Icons::patchDart(Icons::getCount("dart", "001"), setting().selectedDartIdx);

	auto t = std::time(nullptr);
	auto tm = *std::localtime(&t);

	switch (tm.tm_mon)
	{
	case 11:
		CCParticleSnow* snow = CCParticleSnow::createWithTotalParticles(700);
		self->addChild(snow);
		break;
	}

	setting().save();
	//auto checkpoint = CCSprite::createWithSpriteFrameName("checkpoint_01_001.png");
	//auto achi = gd::AchievementBar::create("PolzHax", "Welcome to PolzHax: <cg>Reworked</c> beta (010226).", nullptr);
	//auto iconSprite = achi->m_layerColor->getChildByType<CCSprite*>(0);
	//achi->m_layerColor->addChild(checkpoint);

	//if (iconSprite) {
	//	checkpoint->setPosition(iconSprite->getPosition());
	//}

	//checkpoint->setZOrder(4);

	//auto an = gd::AchievementNotifier::sharedState();
	//an->m_achievementBarArray->addObject(achi);
	//if (an->m_activeAchievementBar == nullptr) {
	//	an->showNextAchievement();
	//}

	//setPitch(.8f);

	std::cout << "CCFileUtils search paths:\n";
	for (auto str : CCFileUtils::sharedFileUtils()->m_searchPathArray) {
		std::cout << str.c_str() << std::endl;
	}

	return true;
}

void MenuLayer::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xaf210), MenuLayer::initH, reinterpret_cast<void**>(&MenuLayer::init));
}