#pragma once
#include "pch.h"

namespace ObjectToolbox {
	inline bool(__thiscall* rampTab)();
	void __fastcall rampTab_H();

	inline bool(__thiscall* spikesTab)();
	void __fastcall spikesTab_H();

	inline bool(__thiscall* decoSpikesTab)();
	void __fastcall decoSpikesTab_H();

	inline bool(__thiscall* chainTab)();
	void __fastcall chainTab_H();

	inline bool(__thiscall* triggerTab)();
	void __fastcall triggerTab_H();

	void mem_init();
}
