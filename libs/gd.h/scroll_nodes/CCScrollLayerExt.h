#ifndef __CCSCROLLLAYEREXT_H__
#define __CCSCROLLLAYEREXT_H__

#include <gd.h>

namespace gd {

	class CCScrollLayerExtDelegate;
	class CCContentLayer;

	class CCScrollLayerExt : public cocos2d::CCLayer {
	public:
		cocos2d::CCTouch* m_touch;
		cocos2d::CCPoint m_touchPosition;
		cocos2d::CCPoint m_touchStartPosition;
		cocos2d::cc_timeval m_timeValue;
		bool m_touchDown; // m_isTouch
		bool m_notAtEndOfScroll; // m_isScrolling
		cocos2d::CCLayerColor* m_verticalScrollbar; // m_verticalScrollIndicator
		cocos2d::CCLayerColor* m_horizontalScrollbar; // m_horizontalScrollIndicator
		CCScrollLayerExtDelegate* m_delegate; // m_scrollDelegate
		CCContentLayer* m_contentLayer; // m_contentLayer
		bool m_cutContent; // m_clipsToBounds
		bool m_hScrollbarVisible; // m_showsHorizontalScrollIndicator
		bool m_vScrollbarVisible; // m_showsVerticalScrollIndicator
		bool m_disableHorizontal; // m_lockHorizontal
		bool m_disableVertical; // m_lockVertical
		bool m_disableMovement; // m_touchDispatch
		float m_scrollLimitTop; // m_topPadding
		float m_scrollLimitBottom; // m_bottomPadding
		float m_peekLimitTop; // m_maxOffsetTop
		float m_peekLimitBottom; // m_maxOffsetBottom

	protected:
		CCScrollLayerExt(cocos2d::CCRect rect) {
			reinterpret_cast<void(__thiscall*)(CCScrollLayerExt*, cocos2d::CCRect)>(
				base + 0x1B020
				)(this, rect);
		}

	public:
		//own vtable
		virtual void preVisitWithClippingRect(cocos2d::CCRect rect) {
			return reinterpret_cast<void(__thiscall*)(CCScrollLayerExt*, cocos2d::CCRect)>(
				base + 0x1C000
				)(this, rect);
		}
		virtual void postVisit() {
			return reinterpret_cast<void(__thiscall*)(CCScrollLayerExt*)>(
				base + 0x1C090
				)(this);
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