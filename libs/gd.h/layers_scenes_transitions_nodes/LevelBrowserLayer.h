#ifndef __LEVELBROWSERLAYER_H__
#define __LEVELBROWSERLAYER_H__

#include <gd.h>

namespace gd {
	class TextArea;
	class GJListLayer;
	class CCMenuItemSpriteExtra;
	class GJSearchObject;
	class LoadingCircle;

	class LevelBrowserLayer : public cocos2d::CCLayer, LevelManagerDelegate, FLAlertLayerProtocol {
	public:
		TextArea* m_noInternet;
		GJListLayer* m_list;
		CCMenuItemSpriteExtra* m_rightArrow;
		CCMenuItemSpriteExtra* m_leftArrow;
		cocos2d::CCArray* m_array;
		GJSearchObject* m_searchObject;
		cocos2d::CCLabelBMFont* m_countText;
		int m_itemCount;
		int m_pageStartIdx;
		int m_pageEndIdx;
		LoadingCircle* m_circle;

		void loadPage(GJSearchObject* search) {
			reinterpret_cast<void(__thiscall*)(LevelBrowserLayer*, gd::GJSearchObject*)>(base + 0x8a000)(this, search);
		}
	};
}

#endif // !__LEVELBROWSERLAYER_H__
