#include "VideoOptionsLayer.hpp"
#include "Setting.hpp"

void VideoOptionsLayer::Callback::onTextures(CCObject*) {

}

bool __fastcall VideoOptionsLayer::initH(gd::VideoOptionsLayer* self) {
	if (!VideoOptionsLayer::init(self)) return false;

	auto onTexturesSprite = gd::ButtonSprite::create("Textures", 0x3c, 0, .6f, false, "goldFont.fnt", "GJ_button_04.png", 25.f);
	auto onTextures = gd::CCMenuItemSpriteExtra::create(onTexturesSprite, self, menu_selector(VideoOptionsLayer::Callback::onTextures));
	onTextures->setPosition(132.f, 80.f);
	self->m_buttonMenu->addChild(onTextures);

	return true;
}

void VideoOptionsLayer::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xd3360), VideoOptionsLayer::initH, reinterpret_cast<void**>(&VideoOptionsLayer::init));
}