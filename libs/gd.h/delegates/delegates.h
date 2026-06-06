#ifndef __DELEGATES_H__
#define __DELEGATES_H__

#include <gd.h>

namespace gd {
	class ColorSelectPopup;
	class SongInfoObject;
	class PlayerObject;
	enum class GJSongError;
	class CCCircleWave;
	class SelectArtLayer;
	class CCTextInputNode;
	class GJGameLevel;
	class GJDropDownLayer;
	class CCIndexPath;
	class TableView;
	class TableViewCell;

	class AppDelegate : public cocos2d::CCApplication {
	public:
		static AppDelegate* get() {
			return static_cast<AppDelegate*>(cocos2d::CCApplication::sharedApplication());
		}
	};

	class GameRateDelegate {
		virtual void updateRate() {}
	};

	class ColorSelectDelegate {
		virtual void colorSelectClosed(ColorSelectPopup*) {}
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

	class SelectArtDelegate {
		virtual void selectArtClosed(SelectArtLayer*);
	};

	class CustomSongLayerDelegate {
		virtual void customSongLayerClosed();
	};

	class TextInputDelegate {
		virtual void textChanged(CCTextInputNode*) {}
		virtual void textInputOpened(CCTextInputNode*) {}
		virtual void textInputClosed(CCTextInputNode*) {}
		virtual void textInputShouldOffset(CCTextInputNode*, float) {}
		virtual void textInputReturn(CCTextInputNode*) {}
		virtual bool allowTextInput(CCTextInputNode*) { return true; }
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
		virtual void numberInputClosed(void*/*NumberInputLayer**/);
	};

	class GJDropDownLayerDelegate {
		virtual void dropDownLayerWillClose(GJDropDownLayer*);
	};

	class ListButtonBarDelegate {
		virtual void listButtonBarSwitchedPage(void*/*ListButtonBar**/, int) {}
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

	class PlatformDownloadDelegate {
		virtual void downloadFinished(char const*);
		virtual void downloadFailed(char const*);
	};
}

#endif // !__DELEGATES_H__
