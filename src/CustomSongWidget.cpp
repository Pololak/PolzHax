#include "CustomSongWidget.hpp"

void PolzCustomSongWidget::onCopySongID(CCObject*) {
    clipboard::write(std::to_string(this->m_songID));
}

bool PolzCustomSongWidget::init(SongInfoObject* songInfo, LevelSettingsObject* levelSettings, bool p0, bool p1, bool p2, bool p3, bool p4) {
    if (!CustomSongWidget::init(songInfo, levelSettings, p0, p1, p2, p3, p4)) return false;

    auto onCopySongIDSpr = ButtonSprite::create("Copy", 0xdc, 0, .6f, false, "bigFont.fnt", "GJ_button_04.png", 25.f);
    onCopySongIDSpr->setScale(.7f);
    this->m_fields->m_copySongIDBtn = CCMenuItemSpriteExtra::create(onCopySongIDSpr, this, menu_selector(PolzCustomSongWidget::onCopySongID));
    this->m_fields->m_copySongIDBtn->setPosition(this->m_moreBtn->getPositionX() + 50.f, this->m_moreBtn->getPositionY());
    this->m_fields->m_copySongIDBtn->setVisible(this->m_moreBtn->isVisible() && !this->m_isRobtopSong);
    this->m_buttonMenu->addChild(this->m_fields->m_copySongIDBtn);

    return true;
}