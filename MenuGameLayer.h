#pragma once
#include "pch.h"

class MenuGameLayer : public CCLayer {
public:
	static inline bool(__thiscall* init)(gd::MenuGameLayer* self);
	static bool __fastcall hook(gd::MenuGameLayer* self);
};