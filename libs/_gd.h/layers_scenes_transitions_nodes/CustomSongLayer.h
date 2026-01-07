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
    class CustomSongWidget : public cocos2d::CCNode, MusicDownloadDelegate, FLAlertLayerProtocol {};
    class LevelSettingsLayer;
    class SongInfoObject;
}

#endif