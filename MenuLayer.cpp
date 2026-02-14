#include "MenuLayer.hpp"
#include "Setting.hpp"
#include "Icons.hpp"

void setPitch(float pitch) {
	FMOD::DSP* pitchShifter = nullptr;

	auto fme = gd::FMODAudioEngine::sharedEngine();

	if (!fme || !fme->m_system || !fme->m_globalChannel) {
		return;
	}

	pitchShifter->release();
	pitchShifter = nullptr;

	fme->m_system->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &pitchShifter);
	pitchShifter->setParameterFloat(FMOD_DSP_PITCHSHIFT_FFTSIZE, 4096);
	pitchShifter->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH, pitch);

	fme->m_globalChannel->addDSP(0, pitchShifter);
}

bool __fastcall MenuLayer::initH(gd::MenuLayer* self, void*) {
	if (!MenuLayer::init(self)) return false;

	auto director = CCDirector::sharedDirector();
	auto winSize = director->getWinSize();

	std::cout << director << std::endl;

	auto gm = gd::GameManager::sharedState();

	Icons::patchCube(Icons::getCount("player", "001"));
	Icons::patchShip(Icons::getCount("ship", "001"));
	Icons::patchBall(Icons::getCount("player_ball", "001"));
	Icons::patchBird(Icons::getCount("bird", "001"));
	Icons::patchDart(Icons::getCount("dart", "001"), setting().selectedDartIdx);

	std::cout << gm << "\n";
	std::cout << gm->m_playerName << "\n";

	std::cout << gd::GameLevelManager::sharedState() << std::endl;
	std::cout << gd::GameLevelManager::sharedState()->m_mainLevels << std::endl;

	std::cout << gd::LocalLevelManager::sharedState() << std::endl;

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

	return true;
}

void MenuLayer::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xaf210), MenuLayer::initH, reinterpret_cast<void**>(&MenuLayer::init));
}