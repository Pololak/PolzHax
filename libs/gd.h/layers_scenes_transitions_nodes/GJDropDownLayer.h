#ifndef __GJDROPDOWNLAYER_H__
#define __GJDROPDOWNLAYER_H__

#include <gd.h>

namespace gd {
	class GJListLayer;
	class GJDropDownLayerDelegate;

	#pragma runtime_checks("s", off)
	class GJDropDownLayer : public cocos2d::CCLayerColor {
	public:
		cocos2d::CCPoint m_endPosition;
		cocos2d::CCPoint m_startPosition;
		cocos2d::CCMenu* m_buttonMenu;
		GJListLayer* m_listLayer;
		bool m_hideBackButton;
		cocos2d::CCLayer* m_mainLayer; // m_internalLayer
		bool m_removeOnExit;
		GJDropDownLayerDelegate* m_delegate;

		void showLayer(bool noTransition) {
			reinterpret_cast<void(__thiscall*)(GJDropDownLayer*, bool)>(base + 0x7bf20)(this, noTransition);
		}
		void hideLayer(bool noTransition) {
			reinterpret_cast<void(__thiscall*)(GJDropDownLayer*, bool)>(base + 0x7bff0)(this, noTransition);
		}
	};
	#pragma runtime_checks("s", restore)
}

#endif