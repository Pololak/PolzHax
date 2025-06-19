#ifndef __CUSTOMSONGLAYER_H__
#define __CUSTOMSONGLAYER_H__

#include <gd.h>

namespace gd {
    class FLAlertLayer;
    class FLAlertLayerProtocol;
    class TextInputDelegate;
    class GJDropDownLayerDelegate;
    class LevelSettingsObject;
    class CCTextInputNode;
    class LevelSettingsLayer;
    class SongInfoObject;
    class CustomSongWidget : public cocos2d::CCNode, MusicDownloadDelegate, FLAlertLayerProtocol {
    public:
        SongInfoObject* m_songInfo; // 0xf0
        cocos2d::CCMenu* m_buttonMenu; // 0xf4
        cocos2d::CCLabelBMFont* m_songLabel; // 0xf8
        cocos2d::CCLabelBMFont* m_artistLabel; // 0xfc
        cocos2d::CCLabelBMFont* m_songIDLabel; // 0x100
        cocos2d::CCLabelBMFont* m_errorLabel; // 0x104
        CCMenuItemSpriteExtra* m_downloadBtn; // 0x108
        CCMenuItemSpriteExtra* m_cancelDownloadBtn; // 0x10c
        CCMenuItemSpriteExtra* m_selectSongBtn; // 0x110
        CCMenuItemSpriteExtra* m_getSongInfoBtn; // 0x114
        CCMenuItemSpriteExtra* m_playbackBtn; // 0x118
        CCMenuItemSpriteExtra* m_moreBtn; // 0x11c
        cocos2d::CCSprite* m_sliderGroove; // 0x120
        cocos2d::CCSprite* m_sliderBar; // 0x124
        LevelSettingsObject* m_levelSettings; // 0x128
        bool m_showSelectSongBtn; // 0x12c
        bool m_showPlayMusicBtn; // 0x12d
        bool m_showDownloadBtn; // 0x12e
        bool m_isNotDownloadingSong; // 0x12f
        bool m_isRobtopSong; // 0x130
        PAD(0x3)
        int m_songID; // 0x134
        bool m_reportSongSelect; // 0x138

        void onDownload(cocos2d::CCObject* sender) {
            return reinterpret_cast<void(__thiscall*)(CustomSongWidget*, cocos2d::CCObject*)>(base + 0x39a80)(this, sender);
        }
    };
}

#endif