#ifndef __EDITORPAUSELAYER_H__
#define __EDITORPAUSELAYER_H__

#include "../include/gd.h"

class CCMenuItemSpriteExtra;
class LevelEditorLayer;

class EditorPauseLayer : public CCBlockLayer, public FLAlertLayerProtocol {
public:
    bool m_exiting;
    CCMenuItemSpriteExtra* m_audioOnBtn;
    CCMenuItemSpriteExtra* m_audioOffBtn;
    LevelEditorLayer* m_levelEditorLayer;

    void saveLevel();
};

#endif
