#include "CustomSongWidget.hpp"
#include "utils.hpp"

gd::CCMenuItemSpriteExtra* m_copySongIDBtn = nullptr;

void CustomSongWidget::Callback::onCopySongID(CCObject*) {
	clipboard::write(std::to_string(this->m_songID));
}

bool __fastcall CustomSongWidget::initH(gd::CustomSongWidget* self, void*, gd::SongInfoObject* songInfo, gd::LevelSettingsObject* levelSettings, bool p0, bool p1, bool p2, bool p3, bool p4) {
	if (!CustomSongWidget::init(self, songInfo, levelSettings, p0, p1, p2, p3, p4)) return false;

	auto onCopySongIDSpr = gd::ButtonSprite::create("Copy", 0xdc, 0, .6f, false, "bigFont.fnt", "GJ_button_04.png", 25.f);
	onCopySongIDSpr->setScale(.7f);
	m_copySongIDBtn = gd::CCMenuItemSpriteExtra::create(onCopySongIDSpr, self, menu_selector(CustomSongWidget::Callback::onCopySongID));
	m_copySongIDBtn->setPosition(self->m_moreBtn->getPositionX() + 50.f, self->m_moreBtn->getPositionY());
	m_copySongIDBtn->setVisible(self->m_moreBtn->isVisible() && !self->m_isRobtopSong);
	self->m_buttonMenu->addChild(m_copySongIDBtn);

	std::cout << self->m_moreBtn << std::endl;

	return true;
}

void __fastcall CustomSongWidget::updateSongInfoH(gd::CustomSongWidget* self) {
	if (m_copySongIDBtn) {
		m_copySongIDBtn->setPosition(self->m_moreBtn->getPositionX() + 50.f, self->m_moreBtn->getPositionY());
	}

	CustomSongWidget::updateSongInfo(self);

	if (m_copySongIDBtn) {
		m_copySongIDBtn->setPosition(self->m_moreBtn->getPositionX() + 50.f, self->m_moreBtn->getPositionY());
		m_copySongIDBtn->setVisible(self->m_moreBtn->isVisible());
	}
}

void __fastcall CustomSongWidget::destructorH(gd::CustomSongWidget* self) {
	std::cout << "CustomSongWidget::destructor" << std::endl;
	CustomSongWidget::destructor(self);
	m_copySongIDBtn = nullptr;
}

void CustomSongWidget::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x38bd0), CustomSongWidget::initH, reinterpret_cast<void**>(&CustomSongWidget::init));
	//MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x3a150), CustomSongWidget::updateSongInfoH, reinterpret_cast<void**>(&CustomSongWidget::updateSongInfo));

	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x38a40), CustomSongWidget::destructorH, reinterpret_cast<void**>(&CustomSongWidget::destructor));
}