#ifndef __DELEGATES_H__
#define __DELEGATES_H__

#include "../include/gd.h"

class ColorSelectPopup;
class SongInfoObject;
enum class GJSongError;
class PlayerObject;
class CCCircleWave;
class CCTextInputNode;
class BoomScrollLayer;
class GJGameLevel;
class SelectArtLayer;
class CCIndexPath;
class TableView;
class TableViewCell;

class AppDelegate : public cocos2d::CCApplication {
public:
	static AppDelegate* get();
	void trySaveGame();
};

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
    virtual void textChanged(CCTextInputNode*);
    virtual void textInputOpened(CCTextInputNode*);
    virtual void textInputClosed(CCTextInputNode*);
    virtual void textInputShouldOffset(CCTextInputNode*, float);
    virtual void textInputReturn(CCTextInputNode*);
    virtual bool allowTextInput(CCTextInputNode*);
};

class BoomScrollLayerDelegate {
	virtual void scrollLayerScrollingStarted(BoomScrollLayer*);
	virtual void scrollLayerScrolledToPage(BoomScrollLayer*, int);
	virtual void scrollLayerMoved(cocos2d::CCPoint);
};

class DynamicScrollDelegate {
	virtual void updatePageWithObject(cocos2d::CCObject*, cocos2d::CCObject*) {}
};

class LevelManagerDelegate {
	virtual void loadLevelsFinished(cocos2d::CCArray*, char const*);
	virtual void loadLevelsFailed(char const*);
	virtual void setupPageInfo(std::string, char const*);
};

class LevelDownloadDelegate {
	virtual void levelDownloadFinished(GJGameLevel*);
	virtual void levelDownloadFailed(int);
};

class LevelCommentDelegate {
	virtual void loadCommentsFinished(cocos2d::CCArray*, char const*);
	virtual void loadCommentsFailed(char const*);
	virtual void updateUserScoreFinished();
	virtual void setupPageInfo(std::string, char const*);
};

class CommentUploadDelegate {
	virtual void commentUploadFinished(int) {}
	virtual void commentUploadFailed(int) {}
};

class LevelUploadDelegate {
	virtual void levelUploadFinished(GJGameLevel*);
	virtual void levelUploadFailed(GJGameLevel*);
};

class LevelUpdateDelegate {
	virtual void levelUpdateFinished(GJGameLevel*, UpdateResponse);
	virtual void levelUpdateFailed(int);
};

class LeaderboardManagerDelegate {
	virtual void updateUserScoreFinished();
	virtual void updateUserScoreFailed();
	virtual void loadLeaderboardFinished(cocos2d::CCArray*, char const*);
	virtual void loadLeaderboardFailed(char const*);
};

class LevelDeleteDelegate {
	virtual void levelDeleteFinished(int);
	virtual void levelDeleteFailed(int);
};

class RateLevelDelegate {
	virtual void rateLevelClosed();
};

class LikeItemDelegate {
	virtual void likedItem(int, int, bool);
};

class NumberInputDelegate {
	virtual void numberInputClosed(void* /*NumberInputLayer**/);
};

class SelectArtDelegate {
	virtual void selectArtClosed(SelectArtLayer*);
};

class CustomSongLayerDelegate {
	virtual void customSongLayerClosed();
};

class TableViewDelegate {
	virtual void willTweenToIndexPath(CCIndexPath&, TableViewCell*, TableView*) {}
	virtual void didEndTweenToIndexPath(CCIndexPath&, TableView*) {}
	virtual void TableViewWillDisplayCellForRowAtIndexPath(CCIndexPath&, TableViewCell*, TableView*) {}
	virtual void TableViewDidDisplayCellForRowAtIndexPath(CCIndexPath&, TableViewCell*, TableView*) {}
	virtual void TableViewWillReloadCellForRowAtIndexPath(CCIndexPath&, TableViewCell*, TableView*) {}
	virtual float cellHeightForRowAtIndexPath(CCIndexPath&, TableView*) { return 0; }
	virtual void didSelectRowAtIndexPath(CCIndexPath&, TableView*) {}
};

#endif