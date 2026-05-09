#pragma once
#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "PracticeFix.hpp"

using namespace geode::prelude;

class PolzPlayLayer : public geode::Modify<PolzPlayLayer, PlayLayer> {
public:
    struct Fields {
        std::vector<GameObject*> m_coinsToPickup;
        bool m_deafenPressed = false;
        std::vector<Checkpoint> m_checkpoints;
        int m_currentStartPos = 0;
        std::vector<StartPosObject*> m_startPositions;
        std::vector<GameObject*> m_dualPortals, m_gamemodePortals, m_miniPortals, m_speedChanges, m_mirrorPortals;
        std::unordered_map<StartPosObject*, std::pair<float, float>> m_startPositionsBestRun;

        CCNode* m_labelsNode = nullptr;
        CCLabelBMFont* m_cheatIndicatorLabel = nullptr;
        CCLabelBMFont* m_messageLabel = nullptr;
        CCLabelBMFont* m_attemptsLabel = nullptr;
        CCLabelBMFont* m_fpsCounterLabel = nullptr;
        float fps;
        float _updateInterval = .5f;
        float _timeLeft = _updateInterval;
        float _accum = 0;
        int _frames = 0;
        CCLabelBMFont* m_cpsCounterLabel = nullptr;
        bool m_hasClicked;
        bool m_isHolding;
        std::vector<time_t> m_clickFrames;
        int m_totalClicks;
        CCLabelBMFont* m_jumpsLabel = nullptr;
        CCLabelBMFont* m_sessionTimeLabel = nullptr;
        CCLabelBMFont* m_bestRunLabel = nullptr;
        float m_lastRun;
        float m_bestRunPercentage;
        CCLabelBMFont* m_clockLabel = nullptr;
        std::time_t clockTime;
        SYSTEMTIME st;
        CCLabelBMFont* m_noclipAccuracyLabel = nullptr;
        CCLabelBMFont* m_noclipDeathsLabel = nullptr;
        bool m_wouldDie;
        bool m_prevDied;
        int	m_noclipFrames;
        int m_deaths;
        int m_deathsFull;
        float m_totalDelta;
        float m_prevX;
        
        bool m_cheatingBeforeRestart;
    };

    bool getCheatingBeforeRestart();
    bool isCheating();
    void nextStartPos();
    void prevStartPos();
    void updateShowLayout();
    void updateShowHitboxes();
    void clearHitboxes();
    void updatePlayerColors();

    void updateCheatIndicator();
    void updateMessageLabel();
    void updateAttemptsLabel();
    void updateFPSLabel();
    void updateCPSLabel();
    void updateJumpsLabel();
    void updateSessionTimeLabel();
    void updateBestRunLabel();
    void updateClockLabel();
    void updateNoclipAccuracyLabel(bool tintRed = false);
    void updateNoclipDeathsLabel(bool tintRed = false);
    void updateStatusLabels();
    void updateStartPosSwitcherLabel();

    bool init(GJGameLevel*);
    void update(float);
    void resetLevel();
    void addToSection(GameObject*);
    void createObjectsFromSetup(gd::string);
    void togglePracticeMode(bool);
    void showNewBest();
    void updateVisibility();
    void updateAttempts();
    void processItems();
    void destroyPlayer(PlayerObject*);
    void levelComplete();
    void pauseGame(bool);
    CheckpointObject* createCheckpoint();
    void removeLastCheckpoint();
    void spawnPlayer2();
    void pushButton(int, bool);
    void releaseButton(int, bool);
};