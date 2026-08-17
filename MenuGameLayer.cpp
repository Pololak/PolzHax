#include "MenuGameLayer.hpp"

void __fastcall MenuGameLayer::tryJumpH(gd::MenuGameLayer* self, void*, float dt) {

}

bool __fastcall MenuGameLayer::ccTouchBeganH(gd::MenuGameLayer* _self, void*, CCTouch* touch, CCEvent* event) {
	auto self = reinterpret_cast<gd::MenuGameLayer*>(reinterpret_cast<uintptr_t>(_self) - 0xe8);

	std::cout << "Push" << std::endl;

	return MenuGameLayer::ccTouchBegan(_self, touch, event);
}

void __fastcall MenuGameLayer::updateH(gd::MenuGameLayer* self, void*, float dt) {
	MenuGameLayer::update(self, dt);
}

void MenuGameLayer::mem_init() {
	//MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xadff0), MenuGameLayer::tryJumpH, reinterpret_cast<void**>(&MenuGameLayer::tryJump));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xaef10), MenuGameLayer::ccTouchBeganH, reinterpret_cast<void**>(&MenuGameLayer::ccTouchBegan));
	//MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xae210), MenuGameLayer::updateH, reinterpret_cast<void**>(&MenuGameLayer::update));
}