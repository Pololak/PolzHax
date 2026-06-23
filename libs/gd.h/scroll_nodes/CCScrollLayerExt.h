#ifndef __CCSCROLLLAYEREXT_H__
#define __CCSCROLLLAYEREXT_H__

#include <gd.h>

namespace gd {

	class CCScrollLayerExtDelegate;
	class CCContentLayer;

	class CCScrollLayerExt : public cocos2d::CCLayer {
	public:
		cocos2d::CCTouch* m_touch; // 0x118
		cocos2d::CCPoint m_touchPosition; // 0x11c
		cocos2d::CCPoint m_touchStartPosition; // 0x124
		cocos2d::cc_timeval m_timeValue; // 0x12c
		bool m_touchDown; // 0x134
		bool m_notAtEndOfScroll; // 0x135
		cocos2d::CCLayerColor* m_verticalScrollbar; // 0x138
		cocos2d::CCLayerColor* m_horizontalScrollbar; // 0x13c
		CCScrollLayerExtDelegate* m_delegate; // 0x140
		CCContentLayer* m_contentLayer; // 0x144
		bool m_cutContent; // 0x148
		bool m_hScrollbarVisible; // 0x149
		bool m_vScrollbarVisible; // 0x14a
		bool m_disableHorizontal; // 0x14b
		bool m_disableVertical; // 0x14c
		bool m_disableMovement; // 0x14d
		float m_scrollLimitTop; // 0x150
		float m_scrollLimitBottom; // 0x154
		float m_peekLimitTop; // 0x158
		float m_peekLimitBottom; // 0x15c

		CCScrollLayerExt(cocos2d::CCRect rect) {
			reinterpret_cast<void(__thiscall*)(CCScrollLayerExt*, cocos2d::CCRect)>(base + 0xdd70)(this, rect);
		}

		virtual void visit() {
			reinterpret_cast<void(__fastcall*)(CCScrollLayerExt*)>(base + 0xec90)(this);
		}

		virtual bool ccTouchBegan(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent) {
			auto self = reinterpret_cast<CCScrollLayerExt*>(reinterpret_cast<uintptr_t>(this) + 0xe8);
			return reinterpret_cast<bool(__thiscall*)(CCScrollLayerExt*, cocos2d::CCTouch*, cocos2d::CCEvent*)>(base + 0xe720)(self, pTouch, pEvent);
		}
		virtual void ccTouchMoved(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent) {
			auto self = reinterpret_cast<CCScrollLayerExt*>(reinterpret_cast<uintptr_t>(this) + 0xe8);
			return reinterpret_cast<void(__thiscall*)(CCScrollLayerExt*, cocos2d::CCTouch*, cocos2d::CCEvent*)>(base + 0xea80)(self, pTouch, pEvent);
		}
		virtual void ccTouchEnded(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent) {
			auto self = reinterpret_cast<CCScrollLayerExt*>(reinterpret_cast<uintptr_t>(this) + 0xe8);
			return reinterpret_cast<void(__thiscall*)(CCScrollLayerExt*, cocos2d::CCTouch*, cocos2d::CCEvent*)>(base + 0xe820)(self, pTouch, pEvent);
		}
		virtual void ccTouchCancelled(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent) {
			auto self = reinterpret_cast<CCScrollLayerExt*>(reinterpret_cast<uintptr_t>(this) + 0xe8);
			return reinterpret_cast<void(__thiscall*)(CCScrollLayerExt*, cocos2d::CCTouch*, cocos2d::CCEvent*)>(base + 0xea60)(self, pTouch, pEvent);
		}

		void scrollLayer(float p0) {
			reinterpret_cast<void(__fastcall*)(CCScrollLayerExt*, float)>(base + 0xebe0)(this, p0);
		}
	};

	class CCScrollLayerExtDelegate {
	public:
		//lol nice typo rob
		virtual void scrllViewWillBeginDecelerating(CCScrollLayerExt*) {}
		virtual void scrollViewDidEndDecelerating(CCScrollLayerExt*) {}
		virtual void scrollViewTouchMoving(CCScrollLayerExt*) {}
		virtual void scrollViewDidEndMoving(CCScrollLayerExt*) {}
		virtual void scrollViewTouchBegin(CCScrollLayerExt*) {}
		virtual void scrollViewTouchEnd(CCScrollLayerExt*) {}
	};
}

#endif