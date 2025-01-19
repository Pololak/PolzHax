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
    class CustomSongWidget : public cocos2d::CCNode, MusicDownloadDelegate, FLAlertLayerProtocol {
    public:
        void onDownload(cocos2d::CCObject* sender) {
            return reinterpret_cast<void(__thiscall*)(CustomSongWidget*, cocos2d::CCObject*)>(base + 0x39a80)(this, sender);
        }
    };
    class LevelSettingsLayer;
    class SongInfoObject;
}

#endif