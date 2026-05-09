#include "CCScheduler.hpp"
#include "LevelEditorLayer.hpp"
#include "Setting.hpp"

float g_left_over = 0.f; // tps bypass by Mat ig (taken from ReplayBot https://github.com/matcool/ReplayBot/blob/9bb45e2ca6fa62aad939d9bcc18595085d90d7b6/src/hooks.cpp#L10)

void PolzCCScheduler::update(float dt) {
    auto playLayer = GameManager::sharedState()->m_playLayer;
    auto editorLayer = PolzLevelEditorLayer::get();

    if ((playLayer || editorLayer) && setting().onTPSBypass) {
        auto speedhack = this->getTimeScale();

        const float target_dt = 1.f / setting().tpsValue / speedhack;

        unsigned times = static_cast<int>((dt + g_left_over) / target_dt);
        if (dt == 0.f) {
            return CCScheduler::update(target_dt);
        }
        auto start = std::chrono::high_resolution_clock::now();
        for (unsigned i = 0; i < times; ++i) {
            CCScheduler::update(target_dt);
            using namespace std::literals;
            if (std::chrono::high_resolution_clock::now() - start > 33.333ms) {
                times = i + 1;
                break;
            }
        }
        g_left_over += dt - target_dt * times;
    }
    else {
        CCScheduler::update(dt);
    }
}