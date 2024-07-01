#include "windows.h"
#include "cocos2d.h"
#include "MinHook.h"

// original function pointers

void (__thiscall *fpMainLoop)(cocos2d::CCDirector *self);
void (__thiscall *fpPollEvents)(cocos2d::CCEGLView *self);

// hook functions

void __fastcall hkMainLoop(cocos2d::CCDirector *self) {
    fpPollEvents(self->getOpenGLView());
    fpMainLoop(self);
}

void __fastcall hkPollEvents(void *self) {};


DWORD WINAPI thread(LPVOID lpParameter) {
    MH_Initialize();
    const HINSTANCE cocos = GetModuleHandleA("libcocos2d.dll");
    MH_CreateHook(reinterpret_cast<LPVOID>(reinterpret_cast<uintptr_t>(cocos) + 0xfc240), hkMainLoop, reinterpret_cast<LPVOID *>(&fpMainLoop));
    MH_CreateHook(GetProcAddress(cocos, "?pollEvents@CCEGLView@cocos2d@@QAEXXZ"), hkPollEvents, reinterpret_cast<LPVOID *>(&fpPollEvents));

    MH_EnableHook(MH_ALL_HOOKS);
    }

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
		CreateThread(0, 0, thread, 0, 0, 0);
	return TRUE;
}