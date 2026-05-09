#include <Geode/Geode.hpp>

#include <Geode/modify/AppDelegate.hpp>
#include <Geode/modify/AudioEffectsLayer.hpp>
#include <Geode/modify/CCCircleWave.hpp>
#include <Geode/modify/CCHttpClient.hpp>
#include <Geode/modify/CCKeyboardDispatcher.hpp>
#include <Geode/modify/FMODAudioEngine.hpp>
#include <Geode/modify/GameLevelManager.hpp>
#include <Geode/modify/GameManager.hpp>
#include <Geode/modify/GameObject.hpp>
#include <Geode/modify/LevelSearchLayer.hpp>
#include <Geode/modify/RingObject.hpp>

#include "PauseLayer.hpp"
#include "PlayLayer.hpp"

// Utils
#include "patching.hpp"
#include "SpeedHack.h"

// Menu
#include "Menu.hpp"
#include "Setting.hpp"

using namespace geode::prelude;

void CCCircleWave_draw(CCCircleWave* self) {
    if (!setting().onNoEffectCircle) reinterpret_cast<void(__thiscall*)(CCCircleWave*)>(geode::base::get() + 0xb4b0)(self);
}

void CCParticleSystemQuad_initWithTotalParticles(CCParticleSystemQuad* self, unsigned int amount) {
    if (setting().onNoParticles) return reinterpret_cast<void(__thiscall*)(CCParticleSystemQuad*, unsigned int)>(geode::base::getCocos() + 0xb7b60)(self, 0);

    reinterpret_cast<void(__thiscall*)(CCParticleSystemQuad*, unsigned int)>(geode::base::getCocos() + 0xb7b60)(self, amount);
}

class $modify(AudioEffectsLayer) {
    void updateTweenAction(float p0, char const* p1) {
        AudioEffectsLayer::updateTweenAction(p0, p1);
        if (setting().onNoPulse) this->m_baseAudioScale = .5f;
    }
};

class $modify(FMODAudioEngine) {
    void update(float p0) {
        FMODAudioEngine::update(p0);
        if (setting().onNoPulse) this->m_pulse1 = .5f;
    }

    void playBackgroundMusic(gd::string p0, bool p1, bool p2) {
        FMODAudioEngine::playBackgroundMusic(p0, p1, p2);


    }
};

bool CCTransitionScene_initWithDuration(CCTransitionScene* self, float duration, CCScene* scene) {
    if (setting().onNoTransition) return reinterpret_cast<bool(__thiscall*)(CCTransitionScene*, float, CCScene*)>(geode::base::getCocos() + 0xa4990)(self, 0.f, scene);
    return reinterpret_cast<bool(__thiscall*)(CCTransitionScene*, float, CCScene*)>(geode::base::getCocos() + 0xa4990)(self, duration, scene);
}

class $modify(GameManager) { // I need this because Zmx added custom icons lol.
    bool isIconUnlocked(int p0, IconType p1) {
        return setting().onIcons ? true : GameManager::isIconUnlocked(p0, p1);
    }

    bool isColorUnlocked(int p0, bool p1) {
        return setting().onIcons ? true : GameManager::isColorUnlocked(p0, p1);
    }
};

class $modify(CCKeyboardDispatcher) {
    bool dispatchKeyboardMSG(enumKeyCodes key, bool down) {
        bool ret = CCKeyboardDispatcher::dispatchKeyboardMSG(key, down);

        auto playLayer = GameManager::sharedState()->m_playLayer;
        if (playLayer && down) {
            if ((!setting().onPauseDuringCompletion ? !playLayer->m_endTriggered : true) && !playLayer->m_showingEndLayer) {
                if (setting().onRetryKeybind) {
                    if (key == setting().m_retryKeybind) {
                        bool wasPaused = false;
                        if (PolzPauseLayer::get()) {
                            wasPaused = true;
                            PolzPauseLayer::get()->onRestart(nullptr);
                        }
                        if (setting().m_retryKeybind != KEY_R && !wasPaused) {
                            wasPaused = false;
                            playLayer->resetLevel();
                        }
                    }
                }

                if (setting().onStartPosSwitcher) {
                    if (key == setting().m_previousStartPosKey) {
                        static_cast<PolzPlayLayer*>(playLayer)->prevStartPos();
                    }
                    if (key == setting().m_nextStartPosKey) {
                        static_cast<PolzPlayLayer*>(playLayer)->nextStartPos();
                    }
                }
            }
        }

        return ret;
    }
};

std::string str_replace(std::string haystack, std::string needle, std::string replacement) {
	std::string input = std::string(haystack.c_str());
	std::string replace_word = needle;
	std::string replace_by = replacement;

	size_t pos = input.find(replace_word);

	while (pos != std::string::npos) {
		input.replace(pos, replace_word.size(), replace_by);

		pos = input.find(replace_word,
			pos + replace_by.size());
	}

	return input;
}

char originalServerURL[128];

std::string keyToURL(int key) {
	switch (key) {
	case 0:
		return "https://19gdps.com/gdapi/";
	case 1:
		return "https://rewind19.gdps.host/";
	case 2:
		return "https://nebulaps.ps.fhgdps.com/";
	case 3:
		return "https://www.boomlings.com/database/";
	case 4:
	default:
		return setting().m_customServerURL;
	}
}

class $modify(CCHttpClient) {
    void send(CCHttpRequest* request) {
        if (!setting().onEnableSwitcher) {
            return CCHttpClient::send(request);
        }

        auto new_request_url = std::string(request->getUrl());

        new_request_url = str_replace(
            new_request_url, originalServerURL,
            keyToURL(setting().m_serverIndex)
        );

        new_request_url = str_replace(
            new_request_url, originalServerURL,
            keyToURL(setting().m_serverIndex)
        );

        if (setting().m_serverIndex == 3) { // if main gd
            new_request_url = str_replace(
                new_request_url, "getGJLevels19",
                "getGJLevels21"
            );

            new_request_url = str_replace(
                new_request_url, "downloadGJLevel19",
                "downloadGJLevel22"
            );

            new_request_url = str_replace(
                new_request_url, "getGJMapPacks",
                "getGJMapPacks21"
            );
        }

        request->setUrl(new_request_url.c_str());

        log::debug("Original URL: {}", originalServerURL);
        log::debug("Request Sent: {}", request->getUrl());

        CCHttpClient::send(request);
    }
};

class $modify(GameLevelManager) {
    gd::string getLengthStr(bool isTiny, bool isShort, bool isMedium, bool isLong) {
        gd::string ret = GameLevelManager::getLengthStr(isTiny, isShort, isMedium, isLong);
        
        if (setting().onExtraLong) {
            if (isTiny || isShort || isMedium || isLong) {
                std::string sret = ret;
                sret.push_back(',');
                sret.push_back('4');
                ret = sret;
            }
            else {
                std::string sret = ret;
                sret.clear();
                sret.push_back('4');
                ret = sret;
            }
        }

        log::debug("{}", ret);
        log::debug("{} {} {} {} {}", isTiny, isShort, isMedium, isLong, setting().onExtraLong);

        return ret;
    }
};

class $modify(GameObject) {
    void playShineEffect() {
        if (!setting().onNoPortalShine) GameObject::playShineEffect();
    }

    void triggerObject() {
        if (setting().onShowLayout) {
            switch (this->m_objectID) {
            case 29: case 30: case 104: case 105: case 744: case 221: case 717: case 718: case 743:
                return;
            }
        }
        GameObject::triggerObject();
    }
};

class $modify(RingObject) {
    void spawnCircle() {
        if (!setting().onNoOrbRing) RingObject::spawnCircle();
    }
};

class $modify(LevelSearchLayer) {
    void clearFilters() {
        LevelSearchLayer::clearFilters();
        
        setting().onExtraLong = false;
    }
};

class $modify(AppDelegate) {
    void applicationDidEnterBackground() {
        AppDelegate::applicationDidEnterBackground();
        if (setting().onAutoSave) {
            setting().save();
        }
    }

    void applicationWillEnterForeground() {
        AppDelegate::applicationWillEnterForeground();
        if (setting().onAutoSave) {
            setting().save();
        }
    }
};

$on_mod(Loaded) {
    ReadProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(geode::base::get() + 0x11f338), &originalServerURL, 33, NULL);
    sequence_patch(geode::base::get() + 0x28bd5, { 0x6a, 0x00 }); // RGBA8888 format.
    sequence_patch(geode::base::get() + 0x3a49b, { 0xb8, 0x01, 0x00, 0x00, 0x00, 0x90, 0x90 }); // Play Music Button.
    sequence_patch(geode::base::get() + 0x3a669, { 0x00, 0x00, 0x00, 0x43 }); // CustomSongWidget m_artistLabel->limitLabelWidth(120.f, ...)

    SpeedHack::Setup();

    Mod::get()->hook(reinterpret_cast<void*>(geode::base::get() + 0xb4b0), &CCCircleWave_draw, "CCCircleWave::draw", tulip::hook::TulipConvention::Thiscall);
    Mod::get()->hook(reinterpret_cast<void*>(geode::base::getCocos() + 0xb7b60), &CCParticleSystemQuad_initWithTotalParticles, "CCParticleSystemQuad::initWithTotalParticles", tulip::hook::TulipConvention::Thiscall);
    Mod::get()->hook(reinterpret_cast<void*>(geode::base::getCocos() + 0xa4990), &CCTransitionScene_initWithDuration, "CCTransitionScene::initWithDuration", tulip::hook::TulipConvention::Thiscall);

    setupImGuiMenu();
}