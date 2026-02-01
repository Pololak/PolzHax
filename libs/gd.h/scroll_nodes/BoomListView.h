#ifndef __BOOMLISTVIEW_H__
#define __BOOMLISTVIEW_H__

#include <gd.h>

namespace gd {

	class TableViewDelegate;
	class TableViewDataSource;
	class TableView;
	class TableViewCell;

	class BoomListView : public cocos2d::CCLayer, public TableViewDelegate, public TableViewDataSource {
	public:

	};
}

#endif