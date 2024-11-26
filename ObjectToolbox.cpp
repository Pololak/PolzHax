#include "ObjectToolbox.h"
#include "pch.h"
#include "state.h"

void objectAdd(int id)
{
    __asm {
        push ecx
        push 0x4
        push id
        mov ecx, ebx
        call gd::EditorUI::getCreateBtn
        push eax
        mov ecx, esi
        call edi
    }
}

void __fastcall ObjectToolbox::rampTab_H() {
    objectAdd(0x173);
    objectAdd(0x174);
    objectAdd(0x175);
    objectAdd(0x176);
    ObjectToolbox::rampTab();
}

void __fastcall ObjectToolbox::spikesTab_H() {
    objectAdd(0x1A5);
    objectAdd(0x1A6);
    ObjectToolbox::spikesTab();
}

void __fastcall ObjectToolbox::decoSpikesTab_H() {
    objectAdd(0x1A3);
    objectAdd(0x1A4);
    ObjectToolbox::decoSpikesTab();
}

void __fastcall ObjectToolbox::chainTab_H() {
    objectAdd(0x1d2); // another bushes that don't overlaps player
    objectAdd(0x1cd);
    objectAdd(0x1ce);
    objectAdd(0x1cf);
    objectAdd(0x1d0);
    objectAdd(0x1d1);
    objectAdd(0x196); // grass
    objectAdd(0x197);
    objectAdd(0x198);
    objectAdd(0x2D5); // dot
    ObjectToolbox::chainTab();
}

void __fastcall ObjectToolbox::triggerTab_H() {
    objectAdd(0x37);
    objectAdd(0x8E);
    ObjectToolbox::triggerTab();
}

void __stdcall ObjectToolbox::gridNodeSizeForKeyH(int objectID) {
	//float gridSize = setting().gridSize;
	//__asm {
	//	movss xmm0, gridSize
	//	add esp, 0x4
	//}
}

void ObjectToolbox::mem_init() {
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x44964),
        reinterpret_cast<void*>(&ObjectToolbox::rampTab_H),
        reinterpret_cast<void**>(&ObjectToolbox::rampTab));
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x44f99),
        reinterpret_cast<void*>(&ObjectToolbox::spikesTab_H),
        reinterpret_cast<void**>(&ObjectToolbox::spikesTab));
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x45f4b),
        reinterpret_cast<void*>(&ObjectToolbox::decoSpikesTab_H),
        reinterpret_cast<void**>(&ObjectToolbox::decoSpikesTab));
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x464d6),
        reinterpret_cast<void*>(&ObjectToolbox::chainTab_H),
        reinterpret_cast<void**>(&ObjectToolbox::chainTab));
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x46d37),
        reinterpret_cast<void*>(&ObjectToolbox::triggerTab_H),
        reinterpret_cast<void**>(&ObjectToolbox::triggerTab));
    //MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xcfc90), ObjectToolbox::gridNodeSizeForKeyH, reinterpret_cast<void**>(&ObjectToolbox::gridNodeSizeForKey));
}