#include "LoadingLayer.hpp"
#include "Setting.hpp"
#include <filesystem>

bool __fastcall LoadingLayer::initH(gd::LoadingLayer* self, void*, bool reload) {
	CCTexture2D::setDefaultAlphaPixelFormat(cocos2d::CCTexture2DPixelFormat::kCCTexture2DPixelFormat_RGBA8888);
	
	auto fileUtils = CCFileUtils::sharedFileUtils();
	fileUtils->removeAllPaths();
	fileUtils->addSearchPath((fileUtils->getWritablePath2() + "PolzHax\\texturePacks").c_str());
	if (setting().m_customTexturePackPath.size() && setting().m_customTexturePackPath != "Base") {
		std::string texturePackPath = fileUtils->getWritablePath2() + "PolzHax\\texturePacks\\" + setting().m_customTexturePackPath;
		if (std::filesystem::is_directory(texturePackPath) || std::filesystem::exists(texturePackPath)) {
			fileUtils->addSearchPath(texturePackPath.c_str());
		}
	}
	fileUtils->addSearchPath((fileUtils->getWritablePath2().c_str()));
	fileUtils->addSearchPath((fileUtils->getWritablePath2() + "Resources").c_str());
	if (reload) {
		gd::GameSoundManager::sharedState()->m_activeBGMusic.clear();
		gd::FMODAudioEngine::sharedEngine()->m_filePath.clear();
		gd::GameSoundManager::sharedState()->m_preloaded = false;
		gd::GameSoundManager::sharedState()->preload();
	}

	if (!LoadingLayer::init(self, reload)) return false;
	return true;
}

const char* __fastcall LoadingLayer::getLoadingStringH(gd::LoadingLayer* self) {
	auto t = std::time(nullptr);
	auto tm = *std::localtime(&t);

	if (tm.tm_mday == 5 && tm.tm_mon == 6) {
		return "Happy birthday PolzHax!";
	}

	if (tm.tm_mday == 9 && tm.tm_mon == 10) {
		return "Happy birthday 1.9!";
	}

	if (tm.tm_mday == 14 && tm.tm_mon == 11) {
		return "Happy birthday Pololak!";
	}

	return LoadingLayer::getLoadingString(self);
}

void LoadingLayer::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xab150), LoadingLayer::initH, reinterpret_cast<void**>(&LoadingLayer::init));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xabef0), LoadingLayer::getLoadingStringH, reinterpret_cast<void**>(&LoadingLayer::getLoadingString));
}