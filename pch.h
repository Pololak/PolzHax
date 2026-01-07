// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

//#define _CRT_SECURE_NO_WARNINGS

// add headers that you want to pre-compile here
#pragma once
#pragma warning (disable : 4305)
#pragma warning (disable : 4244)
#pragma warning (disable : 4018)
#pragma warning (disable : 4390)
#pragma warning (disable : 5260)
#pragma warning (disable : 4305)
//#include <Windows.h>
#include "framework.h"
#include <MinHook.h>
#include <gd.h>
#include <cocos2d.h>
#include <GUI/CCControlExtension/CCScale9Sprite.h>
#include <GUI/CCControlExtension/CCControlUtils.h>
#include <detours.h>
#pragma comment (lib, "libdetours.lib")
using namespace cocos2d;
#endif //PCH_H
