#pragma once
#include "pch.h"

namespace EditorPauseLayer {
	inline bool(__thiscall* EditorPauseLayer_customSetup)(gd::CCBlockLayer* self);
	bool __fastcall EditorPauseLayer_customSetup_H(gd::CCBlockLayer* self);

	inline bool(__thiscall* EditorPauseLayer_centerMenu)();
	void __fastcall EditorPauseLayer_centerMenu_H();

	inline bool(__thiscall* EditorPauseLayer_bottomMenu)();
	void __fastcall EditorPauseLayer_bottomMenu_H();

	class Callback {
	public:
		void HTLToggler(CCObject*);
		void grid15(CCObject*);
		void grid30(CCObject*);
		void grid60(CCObject*);
	};
}
