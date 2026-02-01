#ifndef __GARAGEPAGE_H__
#define __GARAGEPAGE_H__

#include <gd.h>

namespace gd {
	class GJGarageLayer;
	enum IconType;

	class GaragePage : public cocos2d::CCLayer, ListButtonBarDelegate {
	public:
		GJGarageLayer* m_garageLayer; // 0x11c
		cocos2d::SEL_MenuHandler m_callback; // 0x120
		cocos2d::CCSprite* m_selectSprite; // 0x124
		cocos2d::CCNode* m_selectedSprite; // 0x128
		IconType m_iconType; // 0x12c
		int m_perPage; // 0x130

		GaragePage() {
			reinterpret_cast<void(__thiscall*)(GaragePage*)>(
				base + 0x7c320
				)(this);
		}

		bool init(IconType iconType, GJGarageLayer* garageLayer, cocos2d::SEL_MenuHandler callback) {
			return reinterpret_cast<bool(__thiscall*)(GaragePage*, IconType, GJGarageLayer*, cocos2d::SEL_MenuHandler)>(base + 0x7fd40)(this, iconType, garageLayer, callback);
		}

		static GaragePage* create(IconType iconType, GJGarageLayer* garageLayer, cocos2d::SEL_MenuHandler callback) {
			GaragePage* ret = new GaragePage();
			if (ret && ret->init(iconType, garageLayer, callback)) {
				ret->autorelease();
				return ret;
			}
			CC_SAFE_DELETE(ret);
			return nullptr;
		}
	};
}

#endif // !__GARAGEPAGE_H__
