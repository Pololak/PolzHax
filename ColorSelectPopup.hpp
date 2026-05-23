#pragma once
#include "pch.h"

namespace ColorSelectPopup {
	inline bool(__thiscall* init)(gd::ColorSelectPopup*, gd::GameObject*, int, int, int);
	bool __fastcall initH(gd::ColorSelectPopup*, void*, gd::GameObject*, int, int, int);

	inline void(__thiscall* sliderChanged)(gd::ColorSelectPopup*, CCObject*);
	void __fastcall sliderChangedH(gd::ColorSelectPopup*, void*, CCObject*);

	inline void(__thiscall* colorValueChanged)(gd::ColorSelectPopup*, ccColor3B);
	void __fastcall colorValueChangedH(gd::ColorSelectPopup*, void*, ccColor3B);

	void updateLBGValueLabel(gd::ColorSelectPopup*);

	class Callback : public gd::ColorSelectPopup {
	public:
		void onDefault(CCObject*);
		void onPulseHelper(CCObject*);
		void onLiveEdit(CCObject*);
		void onLBGInfo(CCObject*);
	};

	void mem_init();
}