#pragma once
#include <MinHook.h>

inline void matdash::add_hook_impl(void* addr, void* detour, void** trampoline) {
	static bool initialized = false;
	if (!initialized) {
		MH_Initialize();
		initialized = true;
	}
	MH_CreateHook(addr, detour, trampoline);
	MH_EnableHook(addr);
}