#include "VideoOptionsLayer.hpp"
#include "Setting.hpp"
#include "utils.hpp"

bool m_isBorderless = setting().onBorderlessFullscreen;

void VideoOptionsLayer::Callback::onBorderless(CCObject*) {
	m_isBorderless = !m_isBorderless;
}

bool __fastcall VideoOptionsLayer::initH(gd::VideoOptionsLayer* self) {
	m_isBorderless = setting().onBorderlessFullscreen;
	if (!VideoOptionsLayer::init(self)) return false;

	auto director = CCDirector::sharedDirector();
	auto winSize = director->getWinSize();

	auto fullscreenLabel = self->m_mainLayer->getChildByType<CCLabelBMFont*>(1);
	fullscreenLabel->setPositionY(fullscreenLabel->getPositionY() + 15.f);

	auto fullscreenToggler = static_cast<gd::CCMenuItemToggler*>(self->m_buttonMenu->getChildren()->objectAtIndex(0));
	fullscreenToggler->setPositionY(fullscreenToggler->getPositionY() + 15.f);

	self->createToggleButton(
		menu_selector(VideoOptionsLayer::Callback::onBorderless),
		m_isBorderless,
		self->m_buttonMenu,
		.8f, 120.f,
		false,
		"Borderless",
		ccp(winSize.width / 2.f - 70.f, winSize.height / 2.f + 55.f)
	);

	return true;
}

void __fastcall VideoOptionsLayer::onApplyH(gd::VideoOptionsLayer* self, void*, CCObject* sender) {
	VideoOptionsLayer::onApply(self, sender);

	//auto gm = gd::GameManager::sharedState();
	//auto egl = CCEGLView::sharedOpenGLView();
	//auto director = CCDirector::sharedDirector();

	//int resolution = static_cast<CCString*>(self->m_availableResolutions->objectAtIndex(self->m_currentResolution))->intValue();

	//CCSize size = m_isBorderless ? egl->getDisplaySize() : gm->resolutionForKey(resolution);

	//bool resourcesChanged = gm->m_texQuality != self->m_textureQuality;
	//bool windowChanged = gm->getGameVariable("0025") == self->m_fullscreen ||
	//	setting().onBorderlessFullscreen != m_isBorderless || egl->getWindowedSize() != size;

	//gm->m_texQuality = static_cast<TextureQuality>(self->m_textureQuality);
	//gm->setGameVariable("0025", !self->m_fullscreen);
	//gm->m_resolution = resolution;
	//setting().onBorderlessFullscreen = m_isBorderless;

	//director->updateContentScale(static_cast<TextureQuality>(self->m_textureQuality));
	//egl->setWindowedSize(size);

	//if (!resourcesChanged && !windowChanged) {
	//	return;
	//}

	//if (windowChanged) {
	//	egl->m_bIsFullscreen = self->m_fullscreen;
	//}

	//gm->reloadAll(windowChanged, self->m_fullscreen, true);
}

void VideoOptionsLayer::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xd3360), VideoOptionsLayer::initH, reinterpret_cast<void**>(&VideoOptionsLayer::init));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xd43d0), VideoOptionsLayer::onApplyH, reinterpret_cast<void**>(&VideoOptionsLayer::onApply));
}