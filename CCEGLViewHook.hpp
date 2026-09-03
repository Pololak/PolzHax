#pragma once
#include "pch.h"

namespace CCEGLViewHook {
	inline void(__thiscall* setupWindow)(cocos2d::CCEGLView*, cocos2d::CCRect);
	void __fastcall setupWindowH(cocos2d::CCEGLView*, void*, cocos2d::CCRect);

    inline void(__thiscall* toggleFullScreen)(cocos2d::CCEGLView*, bool);
    void __fastcall toggleFullScreenH(cocos2d::CCEGLView*, void*, bool);

	void mem_init();
}