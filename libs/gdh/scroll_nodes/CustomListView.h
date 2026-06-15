#ifndef __CUSTOMLISTVIEW_H__
#define __CUSTOMLISTVIEW_H__

#include "../include/gd.h"

class BoomListView;

#pragma runtime_checks("s", off)
class CustomListView : public BoomListView {
public:
	CustomListView();

	static CustomListView* create(cocos2d::CCArray* entries, int type, float width, float height);
};
#pragma runtime_checks("s", restore)

#endif