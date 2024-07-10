#pragma once
#include "pch.h"

namespace EditorPauseLayer {
	inline bool(__thiscall* EditorPauseLayer_customSetup)(gd::CCBlockLayer* self);
	void __fastcall EditorPauseLayer_customSetup_H(gd::CCBlockLayer* self);

	inline bool(__thiscall* EditorPauseLayer_keyDown)(gd::EditorPauseLayer* self, enumKeyCodes key);
	void __fastcall EditorPauseLayer_keyDown_H(gd::EditorPauseLayer* self, void* edx, enumKeyCodes key);

	/*inline bool(__thiscall* EditorPauseLayer_centerMenu)();
	void __fastcall EditorPauseLayer_centerMenu_H();

	inline bool(__thiscall* EditorPauseLayer_bottomMenu)();
	void __fastcall EditorPauseLayer_bottomMenu_H();*/

	class Callback {
	public:
		void onResume(CCObject*);
	};
}
