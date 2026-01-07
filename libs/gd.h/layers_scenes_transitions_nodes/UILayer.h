#ifndef __UILAYER_H__
#define __UILAYER_H__

#include <gd.h>

namespace gd {
	class UILayer : public cocos2d::CCLayerColor {
	public:
		void* m_stupidDelegate; // why Zmx named it like this? I don't know.
		void* m_pUnknown1;
		cocos2d::CCMenu* m_checkpointMenu;
		CCMenuItemSpriteExtra* m_pauseBtn;
		bool m_bUnknown2;
		bool m_bUnknown3;
		bool m_p1Jumping;
		bool m_p2Jumping;
		bool m_checkpointBtnDown;
		int m_p1TouchId;
		int m_p2TouchId;
		float m_clkTimer;
	};
}

#endif // !__UILAYER_H__
