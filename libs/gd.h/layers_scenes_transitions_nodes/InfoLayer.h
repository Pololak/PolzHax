#ifndef __INFOLAYER_H__
#define __INFOLAYER_H__

#include <gd.h>

namespace gd {
	class InfoLayer : public FLAlertLayer, LevelCommentDelegate, CommentUploadDelegate, FLAlertLayerProtocol {
	public:
		GJGameLevel* m_level; // 0x1c8
		std::string m_commentsKey; // 0x1cc
		LoadingCircle* m_loadingCircle; // 0x1e4
		cocos2d::CCLabelBMFont* m_noComments; // 0x1e8
		void* m_list; // GJCommentListLayer* // 0x1ec
		CCMenuItemSpriteExtra* m_rightArrow; // 0x1f0
		CCMenuItemSpriteExtra* m_leftArrow; // 0x1f4
		ButtonSprite* m_likeBtn; // 0x1f8
		ButtonSprite* m_timeBtn; // 0x1fc
		CCMenuItemSpriteExtra* m_reportBtn; // 0x200
		CCMenuItemSpriteExtra* m_loadCommentsBtn; // 0x204
		int m_itemCount; // 0x208
		int m_pageStartIdx; // 0x20c
		int m_pageEndIdx; // 0x210
		int m_page; // 0x214
		bool m_canUpdateUserScore; // 0x218

		void loadPage(int page) {
			reinterpret_cast<void(__thiscall*)(InfoLayer*, int)>(base + 0x85e30)(this, page);
		}
	};
}

#endif // !__INFOLAYER_H__
