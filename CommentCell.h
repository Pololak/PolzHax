#pragma once
#include "pch.h"

namespace CommentCell {
	inline void(__thiscall* loadFromComment)(CCLayer*, gd::GJComment*);
	void __fastcall loadFromCommentH(CCLayer* self, void*, gd::GJComment* comment);
}