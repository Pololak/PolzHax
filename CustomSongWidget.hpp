#pragma once
#include "pch.h"

namespace CustomSongWidget {
	inline bool(__thiscall* init)(gd::CustomSongWidget*, gd::SongInfoObject*, gd::LevelSettingsObject*, bool, bool, bool, bool, bool);
	bool __fastcall initH(gd::CustomSongWidget*, void*, gd::SongInfoObject*, gd::LevelSettingsObject*, bool, bool, bool, bool, bool);

	inline void(__thiscall* updateSongInfo)(gd::CustomSongWidget*);
	void __fastcall updateSongInfoH(gd::CustomSongWidget*);

	inline void(__thiscall* destructor)(gd::CustomSongWidget*);
	void __fastcall destructorH(gd::CustomSongWidget*);

	class Callback : public gd::CustomSongWidget {
	public:
		void onCopySongID(CCObject*);
	};

	void mem_init();
}