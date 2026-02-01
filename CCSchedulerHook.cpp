#include "CCSchedulerHook.hpp"
#include "Setting.hpp"

void __fastcall CCSchedulerHook::updateH(cocos2d::CCScheduler* self, void*, float dt) {
	auto speedhack = self->getTimeScale();

	const float target_dt = 1.f / 30.f / speedhack;

	return CCSchedulerHook::update(self, target_dt);
}

void CCSchedulerHook::mem_init() {
	//MH_CreateHook(reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(GetModuleHandleA("libcocos2d.dll")) + 0xff970), CCSchedulerHook::updateH, reinterpret_cast<void**>(&CCSchedulerHook::update));
}