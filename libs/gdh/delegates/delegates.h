#ifndef __DELEGATES_H__
#define __DELEGATES_H__

#include "../include/gd.h"

class ColorSelectPopup;
class SongInfoObject;
enum class GJSongError;
class PlayerObject;
class CCCircleWave;
class CCTextInputNode;

class ColorSelectDelegate {
    virtual void colorSelectClosed(ColorSelectPopup*);
};

class GJRotationControlDelegate {
public:
    virtual void angleChanged(float);
    virtual void angleChangeBegin();
    virtual void angleChangeEnded();
};

class MusicDownloadDelegate {
    virtual void loadSongInfoFinished(SongInfoObject*);
    virtual void loadSongInfoFailed(int, GJSongError);
    virtual void downloadSongFinished(SongInfoObject*);
    virtual void downloadSongFailed(int, GJSongError);
    virtual void songStateChanged();
};

class LevelSettingsDelegate {
    virtual void levelSettingsUpdated();
};

class GameplayDelegate {
	virtual void flipGravity(PlayerObject*, bool, bool);
};

class CCCircleWaveDelegate {
	virtual void circleWaveWillBeRemoved(CCCircleWave*);
};

class GameRateDelegate {
	virtual void updateRate() {}
};

class TextInputDelegate {
	virtual void textChanged(CCTextInputNode*) {}
	virtual void textInputOpened(CCTextInputNode*) {}
	virtual void textInputClosed(CCTextInputNode*) {}
	virtual void textInputShouldOffset(CCTextInputNode*, float) {}
	virtual void textInputReturn(CCTextInputNode*) {}
	virtual bool allowTextInput(CCTextInputNode*) {}
};

#endif