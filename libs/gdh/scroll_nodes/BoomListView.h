#ifndef __BOOMLISTVIEW_H__
#define __BOOMLISTVIEW_H__

#include "../include/gd.h"

class TableViewDelegate;
class TableViewDataSource;
class TableView;
class TableViewCell;

class BoomListView : public cocos2d::CCLayer, public TableViewDelegate, public TableViewDataSource {
public:
	TableView* m_tableView; // 0x120
	cocos2d::CCArray* m_entries; // 0x124
	int m_type; // 0x128
	float m_height; // 0x12c
	float m_width; // 0x130
	float m_itemSeparation; // 0x134
	int m_currentPage; // 0x138
};

#endif