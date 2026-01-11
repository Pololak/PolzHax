#ifndef __FMODAUDIOENGINE_H__
#define __FMODAUDIOENGINE_H__

#include <gd.h>
#include <fmod.hpp>

namespace gd {
	class FMODAudioEngine : public cocos2d::CCNode {
	public:
		cocos2d::CCDictionary* m_dictionary; // 0xe8
		std::string m_filePath; // 0xec
		float m_backgroundMusicVolume; // 0x104
		float m_effectsVolume; // 0x108
		float m_pulse1; // 0x10c
		float m_pulse2; // 0x110
		float m_pulse3; // 0x114
		int m_pulseCounter; // 0x118
		bool m_metering; // 0x11c
		bool m_fading; // 0x11d
		bool m_fadeIn; // 0x11e
		float m_fadeInDuration; // 0x120
		FMOD::System* m_system; // 0x124
		FMOD::Sound* m_sound; // 0x128
		FMOD::Channel* m_currentSoundChannel; // 0x12c
		FMOD::Channel* m_globalChannel; // 0x130
		FMOD::DSP* m_DSP; // 0x134
		FMOD_RESULT m_lastResult; // 0x138
		int m_version; // 0x13c
		void* m_extraDriverData; // 0x140
		int m_musicOffset; // 0x144

		static auto sharedEngine() {
			return reinterpret_cast<FMODAudioEngine * (__stdcall*)()>(base + 0x164c0)();
		}
	};
}

#endif