#ifndef __TABLEVIEW_H__
#define __TABLEVIEW_H__

#include <gd.h>

namespace gd {
	class CCScrollLayerExt;
	class CCScrollLayerExtDelegate;
	class CCIndexPath;
	class TableViewDelegate;
	class TableViewDataSource;

	class TableViewCell : public cocos2d::CCLayer {
	public:
		cocos2d::CCMenu* m_buttonMenu; // 0x118
		TableView* m_tableView; // 0x11c
		CCIndexPath m_indexPath; // 0x120
		bool m_editable; // 0x124
		cocos2d::ccColor3B m_delBtnColor; // 0x128
		cocos2d::CCMenuItem* m_deleteButton; // 0x12c
		std::string m_cellIdentifier; // 0x130
		float m_width; // 0x160
		float m_height; // 0x164
		cocos2d::CCLayerColor* m_backgroundLayer; // 0x168
		cocos2d::CCLayer* m_mainLayer; // 0x16c
	};
	
	class TableView : public CCScrollLayerExt, CCScrollLayerExtDelegate {
	public:
		bool m_touchOutOfBoundary; // m_isScheduled
		cocos2d::CCTouch* m_touchStart; // m_beginTouch
		cocos2d::CCPoint m_touchStartPosition2; // m_beginLocation
		cocos2d::CCPoint m_lastCellPos;
		cocos2d::CCPoint m_touchPosition2; // m_checkLocation
		TableViewCell* m_touchCell;
		bool m_editable;
		bool m_touchMoved; // m_isTouch
		cocos2d::CCArray* m_cellArray; // m_cellVisibleArray
		cocos2d::CCArray* m_cellRemovedArray;
		cocos2d::CCArray* m_indexPathAddedArray;
		TableViewDelegate* m_tableDelegate; // m_delegate
		TableViewDataSource* m_dataSource;
		bool m_clipsToBounds;
		CCIndexPath* m_scrollingToIndexPath;
		bool m_touchDispatch;
		float m_touchLastY;
		bool m_inStealingTouch;
		bool m_cancellingTouches; // m_stealTouches
	};

	class TableViewDataSource {
	public:
		virtual int numberOfRowsInSection(unsigned int, TableView*) {
			return 0;
		}
		virtual unsigned int numberOfSectionsInTableView(TableView*) {
			return 1;
		}
		virtual void TableViewCommitCellEditingStyleForRowAtIndexPath(TableView*, TableViewCellEditingStyle, CCIndexPath&) {
			return;
		}
		virtual TableViewCell* cellForRowAtIndexPath(CCIndexPath&, TableView*) { return nullptr; }
	};
}

#endif