#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class SURVEY_PROGRAM_LAYER : public CCLayer {
protected:
    CCSprite* m_background;
    CCSprite* m_background2;
    CCSprite* m_background3;
    CCSprite* m_background4;
    CCSprite* m_background5;
    CCSprite* m_background6;
    CCSprite* m_background7;
    CCSprite* m_background8;
    CCSprite* m_background9;
    CCSprite* m_background10;
    CCLabelBMFont* m_polzspeech;

    int m_textId;
public:
    static SURVEY_PROGRAM_LAYER* create();
    static CCScene* scene();
    bool init();
    void speech();
    void goToMenu();
    virtual void keyBackClicked();
    void closeTheGame();
    void deleteExecutable();
    void _friend();
};