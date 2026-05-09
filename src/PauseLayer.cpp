#include "PauseLayer.hpp"
#include "Setting.hpp"

PauseLayer* m_pauseLayer;

PauseLayer* PolzPauseLayer::get() {
    return m_pauseLayer;
}

void PolzPauseLayer::customSetup() {
    m_pauseLayer = this;
    PauseLayer::customSetup();

    this->setVisible(!setting().onHidePauseMenu);
}

void PolzPauseLayer::onEdit(CCObject* sender) {
    PauseLayer::onEdit(sender);
    m_pauseLayer = nullptr;
}

void PolzPauseLayer::onResume(CCObject* sender) {
    PauseLayer::onResume(sender);
    m_pauseLayer = nullptr;
}

class ExitAlertProtocol : public FLAlertLayerProtocol {
protected:
    virtual void FLAlert_Clicked(FLAlertLayer* layer, bool btn2) override {
        if (btn2) {
            GameManager::sharedState()->m_playLayer->onQuit();
            GameSoundManager::sharedManager()->playEffect("quitSound_01.ogg", 0, 0, .7f);
            m_pauseLayer = nullptr;
        }
    }
};

ExitAlertProtocol exitAlertProtocol;

void PolzPauseLayer::onQuit(CCObject* sender) {
    if (setting().onConfirmExit) {
        FLAlertLayer::create(&exitAlertProtocol, "Confirm Exit", "Are you sure you want to <cr>exit</c> the level?", "Cancel", "Exit")->show();
        return;
    }

    PauseLayer::onQuit(sender);
    m_pauseLayer = nullptr;
}

void PolzPauseLayer::onRestart(CCObject* sender) {
    PauseLayer::onRestart(sender);
    m_pauseLayer = nullptr;

    CCEGLView::sharedOpenGLView()->showCursor(GameManager::sharedState()->getGameVariable("0024"));
}