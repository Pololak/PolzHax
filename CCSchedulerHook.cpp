#include "CCSchedulerHook.hpp"

void __fastcall CCSchedulerHook::updateH(cocos2d::CCScheduler* self, void*, float dt) {
	CCSchedulerHook::update(self, 1.f / 240.f);
}

void CCSchedulerHook::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(GetModuleHandleA("libcocos2d.dll")) + 0xff970), CCSchedulerHook::updateH, reinterpret_cast<void**>(&CCSchedulerHook::update));
}