#pragma once
#include "pch.h"

namespace InfoLayer {
	inline void(__thiscall* onLevelInfo)(gd::InfoLayer*, CCObject*);
	void __fastcall onLevelInfoH(gd::InfoLayer* self, void*, CCObject* obj);

	inline bool(__thiscall* init)(gd::InfoLayer*, gd::GJGameLevel*);
	bool __fastcall initH(gd::InfoLayer* self, void*, gd::GJGameLevel* gameLevel);

	class Callback : public gd::InfoLayer {
	public:
		void onRefreshComments(CCObject*);
		void onCopyLevelID(CCObject*);
	};
}