#include "MenuGameLayer.hpp"

void __fastcall MenuGameLayer::tryJumpH(gd::MenuGameLayer* self, void*, float dt) {

}

CCTouch* m_touch;
CCEvent* m_event;

bool __fastcall MenuGameLayer::ccTouchBeganH(gd::MenuGameLayer* _self, void*, CCTouch* touch, CCEvent* event) {
	bool ret = MenuGameLayer::ccTouchBegan(_self, touch, event);
	auto self = reinterpret_cast<gd::MenuGameLayer*>(reinterpret_cast<uintptr_t>(_self) - 0xe8);

	std::cout << "Push" << std::endl;

	if (self->m_playerObject) {
		if (ret) {
			self->m_playerObject->pushButton(1);
		}
		else {
			self->m_playerObject->releaseButton(1);
		}
	}

	return ret;
}

void __fastcall MenuGameLayer::updateH(gd::MenuGameLayer* self, void*, float dt) {
	MenuGameLayer::update(self, dt);

}

void MenuGameLayer::mem_init() {
	//MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xadff0), MenuGameLayer::tryJumpH, reinterpret_cast<void**>(&MenuGameLayer::tryJump));
	//MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xaef10), MenuGameLayer::ccTouchBeganH, reinterpret_cast<void**>(&MenuGameLayer::ccTouchBegan));
	//MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xae210), MenuGameLayer::updateH, reinterpret_cast<void**>(&MenuGameLayer::update));
}