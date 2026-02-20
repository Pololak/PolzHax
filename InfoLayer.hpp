#pragma once
#include "pch.h"

namespace InfoLayer {
	inline bool(__thiscall* init)(gd::InfoLayer*, gd::GJGameLevel*);
	bool __fastcall initH(gd::InfoLayer*, void*, gd::GJGameLevel*);

	inline void(__thiscall* onLevelInfo)(gd::InfoLayer*, CCObject*);
	void __fastcall onLevelInfoH(gd::InfoLayer*, void*, CCObject*);

	inline void(__thiscall* destructor)(gd::InfoLayer*);
	void __fastcall destructorH(gd::InfoLayer*);

	inline void(__thiscall* levelIDLabel)();
	void __fastcall levelIDLabelH();

	class Callback : public gd::InfoLayer {
	public:
		void onRefreshComments(CCObject*);
		void onCopyLevelID(CCObject*);
	};

	void mem_init();
}