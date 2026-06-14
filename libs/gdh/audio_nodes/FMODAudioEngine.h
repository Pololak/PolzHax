#ifndef __FMODAUDIOENGINE_H__
#define __FMODAUDIOENGINE_H__

#include "../include/gd.h"
#include <../fmod/fmod.hpp>

class FMODAudioEngine : public cocos2d::CCNode {
public:
    cocos2d::CCDictionary* m_dictionary;
    std::string m_filePath;
    float m_backgroundMusicVolume;
    float m_effectsVolume;
    float m_pulse1;
    float m_pulse2;
    float m_pulse3;
    int m_pulseCounter;
    bool m_metering;
    bool m_fading;
    bool m_fadeIn;
    float m_fadeInDuration;
    FMOD::System* m_system;
    FMOD::Sound* m_sound;
    FMOD::Channel* m_currentSoundChannel;
    FMOD::Channel* m_globalChannel;
    FMOD::DSP* m_DSP;
    FMOD_RESULT m_lastResult;
    int m_version;
    void* m_extraDriverData;
    int m_musicOffset;

    static FMODAudioEngine* sharedEngine();
};

#endif