#ifndef __LEVELSETTINGSOBJECT_H__
#define __LEVELSETTINGSOBJECT_H__

#include "../include/gd.h"

class SettingsColorObject;
class GJGameLevel;

class LevelSettingsObject : public cocos2d::CCNode {
public:
    SettingsColorObject* m_backgroundColor;
    SettingsColorObject* m_groundColor;
    SettingsColorObject* m_lineColor;
    SettingsColorObject* m_objectColor;
    SettingsColorObject* m_3DLineColor;
    SettingsColorObject* m_customColor01;
    SettingsColorObject* m_customColor02;
    SettingsColorObject* m_customColor03;
    SettingsColorObject* m_customColor04;
    int m_startMode;
    int m_startSpeed;
    bool m_startMini;
    bool m_startDual;
    bool m_twoPlayerMode;
    float m_songOffset;
    bool m_fadeIn;
    bool m_fadeOut;
    int m_backgroundIndex;
    int m_groundIndex;
    bool m_startsWithStartPos;
    bool m_isFlipped;
    GJGameLevel* m_level;
    std::string m_guidelineString;
    bool m_songChanged;
};

#endif
