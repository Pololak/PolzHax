#ifndef __GAMEMANAGER_H__
#define __GAMEMANAGER_H__

#include "../include/gd.h"

class PlayLayer;
class LevelSelectLayer;
class PremiumPopup : public cocos2d::CCLayer {};
class GameRateDelegate;

class GameManager : public GManager {
public:
    PAD(40)
    PlayLayer* m_playLayer;
    PAD(4)
    bool m_inMenuLayer;
    PAD(8)
    std::string m_playerUDID;
    std::string m_playerName;
    int m_playerUserID;
    PAD(4)
    float m_bgVolume;
    float m_sfxVolume;
    int m_timeOffset;
    PAD(8)
    LastGameScene m_lastScene;
    LastGameScene m_lastScene2;
    PAD(4)
    int m_playerFrame;
    int m_playerShip;
    int m_playerBall;
    int m_playerBird;
    int m_playerColor;
    int m_playerColor2;
    int m_playerStreak;
    IconType m_playerIconType;
    PAD(4)
    bool m_showProgressBar;
    PAD(11)
    GameRateDelegate* m_rateDelegate;
    PAD(4)
    int m_lastLevelID;
    int m_loadedBgID;
    int m_loadedGroundID;
    int m_totalAttempts;
    int m_bootups;

    static GameManager* sharedState();
    
    bool getGameVariable(char const*);
    void setGameVariable(char const*, bool);

    int getIntGameVariable(char const*);
    void setIntGameVariable(char const*, int);

    void toggleGameVariable(char const*);

    cocos2d::ccColor3B colorForIdx(int);

    void reloadAll(bool, bool, bool);

    PlayLayer* getPlayLayer() const;
};

#endif