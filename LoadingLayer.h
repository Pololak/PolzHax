#pragma once
#include "pch.h"

class LoadingLayer : public CCLayer {
public:
	static inline bool(__thiscall* init)(CCLayer* self);
	static bool __fastcall hook(CCLayer* self);
};