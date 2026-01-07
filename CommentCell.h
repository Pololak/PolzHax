#pragma once
#include "pch.h"

namespace CommentCell {
	inline void(__thiscall* loadFromComment)(gd::CommentCell*, gd::GJComment*);
	void __fastcall loadFromCommentH(gd::CommentCell* self, void*, gd::GJComment* comment);
}