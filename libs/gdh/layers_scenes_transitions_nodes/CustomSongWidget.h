#ifndef __CUSTOMSONGWIDGET_H__
#define __CUSTOMSONGWIDGET_H__

#include "../include/gd.h"

class SongInfoObject;
class CCMenuItemSpriteExtra;
class LevelSettingsObject;

class CustomSongWidget : public cocos2d::CCNode, MusicDownloadDelegate, FLAlertLayerProtocol {
public:
    SongInfoObject* m_songObject; // 0xf0
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
    bool m_isNotDownloading; // 0x12f
    bool m_isRobtopSong; // 0x130
    int m_songID; // 0x134
    bool m_reportSongSelect; // 0x138

    void onDownload(cocos2d::CCObject* sender);
};

#endif // !__CUSTOMSONGWIDGET_H__
