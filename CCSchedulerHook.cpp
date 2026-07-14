#include "CCSchedulerHook.hpp"
#include "Setting.hpp"
#include <chrono>

float g_left_over = 0.f; // tps bypass by Mat ig (taken from ReplayBot https://github.com/matcool/ReplayBot/blob/9bb45e2ca6fa62aad939d9bcc18595085d90d7b6/src/hooks.cpp#L10)

void __fastcall CCSchedulerHook::updateH(cocos2d::CCScheduler* self, void*, float dt) {
	auto playLayer = gd::GameManager::sharedState()->getPlayLayer();

	if (playLayer && setting().onTPSBypass) {
		auto speedhack = self->getTimeScale();

		const float target_dt = 1.f / setting().tpsValue / speedhack;

		if (setting().onRealTime)
			return CCSchedulerHook::update(self, target_dt);

		unsigned times = static_cast<int>((dt + g_left_over) / target_dt);
		if (dt == 0.f) {
			return CCSchedulerHook::update(self, target_dt);
		}
		auto start = std::chrono::high_resolution_clock::now();
		for (unsigned i = 0; i < times; ++i) {
			CCSchedulerHook::update(self, target_dt);
			using namespace std::literals;
			if (std::chrono::high_resolution_clock::now() - start > 33.333ms) {
				times = i + 1;
				break;
			}
		}
		g_left_over += dt - target_dt * times;
	}
	else {
		CCSchedulerHook::update(self, dt);
	}
}

void CCSchedulerHook::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(GetModuleHandleA("libcocos2d.dll")) + 0xff970), CCSchedulerHook::updateH, reinterpret_cast<void**>(&CCSchedulerHook::update));
}