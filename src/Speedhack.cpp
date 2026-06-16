#include "Speedhack.h"
#include "Setting.h"

void Speedhack::updateSpeedhack() {
    if (setting().speedhackValue <= 0.f) return;

    const float value = setting().onSpeedhack ? setting().speedhackValue : 1.f;
    LOGD("%i", setting().onSpeedhack);
    LOGD("%i", setting().onSpeedhackMusic);
    LOGD("%f", setting().speedhackValue);
    LOGD("%f", value);

    CCDirector::sharedDirector()->getScheduler()->setTimeScale(value);
    Speedhack::updateSpeedhackMusic();
}

void Speedhack::updateSpeedhackMusic() {
    if (setting().speedhackValue <= 0.f) return;

    const float value = setting().onSpeedhack ? setting().speedhackValue : 1.f;

    auto fme = FMODAudioEngine::sharedEngine();
    if (fme) {
        auto channel = fme->m_globalChannel;
        if (channel) {
            channel->setPitch(setting().onSpeedhackMusic ? value : 1.f);
        }
    }
}