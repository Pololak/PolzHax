#pragma once
#include "pch.h"

namespace PauseLayer {
	inline void(__thiscall* customSetup)(gd::PauseLayer*);
	void __fastcall customSetupH(gd::PauseLayer*);

	inline void(__thiscall* onEdit)(gd::PauseLayer*, CCObject*);
	void __fastcall onEditH(gd::PauseLayer*, void*, CCObject*);

	inline void(__thiscall* onResume)(gd::PauseLayer*, CCObject*);
	void __fastcall onResumeH(gd::PauseLayer*, void*, CCObject*);

	inline void(__thiscall* onQuit)(gd::PauseLayer*, CCObject*);
	void __fastcall onQuitH(gd::PauseLayer*, void*, CCObject*);

	inline void(__thiscall* onRestart)(gd::PauseLayer*, CCObject*);
	void __fastcall onRestartH(gd::PauseLayer*, void*, CCObject*);

	inline void(__thiscall* onProgressBar)(gd::PauseLayer*, CCObject*);
	void __fastcall onProgressBarH(gd::PauseLayer*, void*, CCObject*);

	inline void(__thiscall* bottomMenu)();
	void __fastcall bottomMenuH();

	inline void(__thiscall* progressBarLabel)();
	void __fastcall progressBarLabelH();

	gd::PauseLayer* get();

	class Callback : public gd::PauseLayer {
	public:
		void onShowPercentage(CCObject*);
	};

	void mem_init();
}