#pragma once
#include "pch.h"

namespace PauseLayer {
	inline bool(__thiscall* customSetup)(gd::CCBlockLayer* self);
	bool __fastcall customSetup_H(gd::CCBlockLayer* self);

	inline bool(__thiscall* onEdit)(CCLayer* self);
	void __fastcall onEdit_H(CCLayer* self);

	inline bool(__thiscall* onRestart)(CCObject* sender);
	void __fastcall onRestart_H(CCObject* sender);

	inline bool(__thiscall* onQuit)(CCObject* btn);
	void __fastcall onQuit_H(CCObject* btn);

	class Callback {
		void PercentageToggler(CCObject*);
	};

	void mem_init();
}