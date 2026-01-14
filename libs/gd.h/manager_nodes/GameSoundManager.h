#ifndef __GAMESOUNDMANAGER_H__
#define __GAMESOUNDMANAGER_H__

#include <gd.h>
#include <filesystem>

namespace gd {
	class GameSoundManager : public cocos2d::CCNode {
	public:
		cocos2d::CCDictionary* m_loopedSounds; // 0xe8
		cocos2d::CCDictionary* m_uniqueEffects; // 0xec
		double m_effectResetTime; // 0xf0
		bool m_bUnused; // 0xf8
		int m_state; // 0xfc
		std::string m_activeBGMusic; // 0x100

		static GameSoundManager* sharedState() {
			return reinterpret_cast<GameSoundManager*(__stdcall*)()>(base + 0x172d0)();
		}

		void playEffect(std::string effectName) {
			reinterpret_cast<void(__stdcall*)(GameSoundManager*, std::string)>(base + 0x179f0)(this, effectName);
		}

		static void playSound(std::string sName) {
			reinterpret_cast<void(__stdcall*)(
				std::string
				)>(
					base + 0x179f0
					)(
						sName
						);
		}
	};
}

#endif