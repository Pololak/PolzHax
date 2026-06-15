#ifndef __EDITLEVELLAYER_H__
#define __EDITLEVELLAYER_H__

#include "../include/gd.h"

class GJGameLevel;
class TextArea;
enum class GJLevelType;

class EditLevelLayer : public cocos2d::CCLayer, TextInputDelegate, public FLAlertLayerProtocol {
public:
    cocos2d::CCMenu* m_buttonMenu; // 0x120
    GJGameLevel* m_level; // 0x124
    TextArea* m_descriptionInput; // 0x128
    cocos2d::CCArray* m_textInputs; // 0x12c
    bool m_isLoadingLevel; // 0x130
    GJLevelType m_levelType; // 0x134
    std::string m_levelName; // 0x138

    static cocos2d::CCScene* scene(GJGameLevel*);
};

#endif