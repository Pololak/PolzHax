#ifndef __LEVELSETTINGSLAYER_H__
#define __LEVELSETTINGSLAYER_H__

#include <gd.h>

namespace gd {
	class LevelSettingsObject;
	class LevelSettingsDelegate;
	class LevelEditorLayer;
	class CCMenuItemSpriteExtra;
	class CustomSongWidget;

	class LevelSettingsLayer : public FLAlertLayer, ColorSelectDelegate, SelectArtDelegate, FLAlertLayerProtocol, CustomSongLayerDelegate {
	public:
		int m_songIndex; // 0x1cc
		int m_speed; // 0x1d0
		cocos2d::CCSprite* m_bgBtnSpr; // 0x1d4
		cocos2d::CCSprite* m_gBtnSpr; // 0x1d8
		cocos2d::CCSprite* m_lBtnSpr; // 0x1dc
		cocos2d::CCSprite* m_objBtnSpr; // 0x1e0
		cocos2d::CCSprite* m_dlBtnSpr; // 0x1e4
		cocos2d::CCSprite* m_col1BtnSpr; // 0x1e8
		cocos2d::CCSprite* m_col2BtnSpr; // 0x1ec
		cocos2d::CCSprite* m_col3BtnSpr; // 0x1f0
		cocos2d::CCSprite* m_col4BtnSpr; // 0x1f4
		cocos2d::CCSprite* m_bgIcon; // 0x1f8
		cocos2d::CCSprite* m_gIcon; // 0x1fc
		LevelSettingsObject* m_settingsObject; // 0x200
		cocos2d::CCLabelBMFont* m_selectedSong; // 0x204
		cocos2d::CCArray* m_unkArray208; // 0x208
		cocos2d::CCArray* m_unkArray20c; // 0x20c
		LevelSettingsDelegate* m_delegate; // 0x210
		LevelEditorLayer* m_editorLayer; // 0x214
		cocos2d::CCArray* m_unkArray218; // 0x218
		cocos2d::CCArray* m_unkArray21c; // 0x21c
		CCMenuItemSpriteExtra* m_normalBtn; // 0x220
		CCMenuItemSpriteExtra* m_customBtn; // 0x224
		CCMenuItemSpriteExtra* m_selectCustomSongBtn; // 0x228
		CCMenuItemSpriteExtra* m_changeSongBtn; // 0x22c
		CustomSongWidget* m_customSongWidget; // 0x230

		void onGravityFlipped(CCObject* sender) {
			reinterpret_cast<void(__thiscall*)(LevelSettingsLayer*, CCObject*)>(base + 0x99b10)(this, sender);
		}

		void onClose(CCObject* sender) {
			reinterpret_cast<void(__thiscall*)(LevelSettingsLayer*, CCObject*)>(base + 0x99dc0)(this, sender);
		}
	};
}

#endif // !__LEVELSETTINGSLAYER_H__
