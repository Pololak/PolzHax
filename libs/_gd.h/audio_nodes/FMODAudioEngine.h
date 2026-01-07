#ifndef __FMODAUDIOENGINE_H__
#define __FMODAUDIOENGINE_H__

#include <gd.h>

namespace FMOD {
	static auto base = GetModuleHandleA("fmod.dll");
	struct Channel {
		void setPitch(float pitch) {
			static const auto addr = GetProcAddress(base, "?setPitch@ChannelControl@FMOD@@QAG?AW4FMOD_RESULT@@M@Z");
			reinterpret_cast<void* (__stdcall*)(void*, float)>(addr)(this, pitch);
		}
		void setReverbProperties(int idk, float idk2) {
			static const auto addr = GetProcAddress(base, "?setReverbProperties@ChannelControl@FMOD@@QAG?AW4FMOD_RESULT@@HM@Z");
			reinterpret_cast<void* (__stdcall*)(void*, int, float)>(addr)(this, idk, idk2);
		}
	};
}

namespace gd {
	class FMODAudioEngine : public cocos2d::CCNode {
	protected:
		cocos2d::CCDictionary* m_pDictionary;
		std::string m_sFilePath;
		float m_fBackgroundMusicVolume;
		float m_fEffectsVolume;
		float m_fPulse1;
		float m_fPulse2;
		float m_fPulse3;
		int m_nPulseCounter;
		bool m_bMetering;
		bool m_bFading;
		bool m_bFadeIn;
		float m_fFadeInDuration;
		void* /*FMOD::Sound*/ m_pSound;
		void* /*FMOD::ChannelGroup*/ m_pChannelGroup;
		bool m_bPaused;
		bool* m_pbBackgroundMusicPlaying;
		void* /*FMOD::FMOD_DSP_METERING_INFO*/ m_pMeteringInfo;
		int /*FMOD::FMOD_RESULT*/ m_eLastResult;
		PAD(0x8);
		int m_nMusicOffset;

	public:
		static FMODAudioEngine* sharedEngine() {
			return reinterpret_cast<FMODAudioEngine * (__stdcall*)()>(
				base + 0x164c0 //16400 //239F0 gd 2.13
				)();
		}
		auto currentSound() {
			return from<FMOD::Channel*>(this, 0x130);
		}
		void preloadEffect(std::string filename) {
			return reinterpret_cast<void(__thiscall*)(FMODAudioEngine*, std::string)>(
				base + 0x24240
				)(this, filename);
		}
		float getCurrentVolume() {
			return from<float>(this, 0x104);
		}
		//my own function
		void reloadEffects() {
			using namespace std::filesystem;

			std::vector<std::string> oggs = {};
			path defaults = current_path() / "Resources";
			for (auto file : directory_iterator{ defaults }) {
				if (file.path().extension() == ".ogg")
					oggs.push_back(file.path().filename().string());
			}
			for (auto ogg : oggs) {
				m_pDictionary->removeObjectForKey(ogg);
				this->preloadEffect(ogg);
			}
		}
	};
}

#endif