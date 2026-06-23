#ifndef __BOOMSCROLLLAYER_H__
#define __BOOMSCROLLLAYER_H__

#include <gd.h>

namespace gd {
	class BoomScrollLayer : public cocos2d::CCLayer {
	public:
		cocos2d::CCArray* m_dotsArray; // 0x118
		int m_animatingToPage; // 0x11c
		float m_touchX; // 0x120
		float m_animateSpeed; // 0x124
		bool m_bUnknown1; // 0x128
		DynamicScrollDelegate* m_dynamicScrollDelegate; // 0x12c
		cocos2d::CCArray* m_dynamicScrollPages; // 0x130
		bool m_dynamic; // 0x134
		int m_touchType; // 0x138
		bool m_bUnknown3; // 0x13c
		cocos2d::CCTouch* m_currentTouch; // 0x140
		cocos2d::CCArray* m_pages; // 0x144
		double m_touchTimer; // 0x148
		cocos2d::CCPoint m_pUnknown4; // 0x150
		cocos2d::CCLayer* m_mainLayer; // 0x158
		cocos2d::CCRect m_scrollArea; // 0x15c
		float m_minTouchSpeed; // 0x16c
		float m_touchSpeedFast; // 0x170
		float m_touchSpeedMid; // 0x174
		BoomScrollLayerDelegate* m_delegate; // 0x178
		bool m_movingToPage; // 0x17c
		float m_minimumTouchLengthToSlide; // 0x180
		float m_minimumTouchLengthToChangePage; // 0x184
		float m_marginOffset; // 0x188 m_width
		bool m_stealTouches; // 0x18c
		bool m_showPagesIndicator; // 0x18d
		cocos2d::CCPoint m_pagesIndicatorPosition; // 0x190
		cocos2d::ccColor4B m_pagesIndicatorSelectedColor; // 0x198
		cocos2d::ccColor4B m_pagesIndicatorNormalColor; // 0x19c
		int m_page; // 0x1a0
		float m_pagesWidthOffset; // 0x1a4
		void* m_unusedPages; // 0x1a8

		virtual bool init(cocos2d::CCArray* pages, int offset, bool looped, cocos2d::CCArray* dynamicObject, DynamicScrollDelegate* delegate) {
			return reinterpret_cast<bool(__thiscall*)(BoomScrollLayer*, cocos2d::CCArray*, int, bool, cocos2d::CCArray*, DynamicScrollDelegate*)>(base + 0x79b0)(this, pages, offset, looped, dynamicObject, delegate);
		}

		static BoomScrollLayer* create(cocos2d::CCArray* pages, int offset, bool looped, cocos2d::CCArray* dynamicObject, DynamicScrollDelegate* delegate) {
			BoomScrollLayer* ret = new BoomScrollLayer();
			if (ret && ret->init(pages, offset, looped, dynamicObject, delegate)) {
				ret->autorelease();
				return ret;
			}
			CC_SAFE_DELETE(ret);
			return nullptr;
		}

		static BoomScrollLayer* create(cocos2d::CCArray* pages, int offset, bool looped) {
			return BoomScrollLayer::create(pages, offset, looped, nullptr, nullptr);
		}

		void addPage(cocos2d::CCLayer* page, int index) {
			index = std::clamp<int>(index, 0, m_pages->count());
			m_pages->insertObject(page, index);
			this->updatePages();
			this->moveToPage(m_page);
		}

		void updatePages() {
			reinterpret_cast<void(__fastcall*)(BoomScrollLayer*)>(base + 0x7ff0)(this);
		}

		void moveToPage(int page) {
			reinterpret_cast<void(__thiscall*)(BoomScrollLayer*, int)>(base + 0x8500)(this, page);
		}

		void instantMoveToPage(int page) {
			reinterpret_cast<void(__thiscall*)(BoomScrollLayer*, int)>(base + 0x8430)(this, page);
		}

		int getTotalPages() {
			return m_dynamic ? m_dynamicScrollPages->count() : m_pages->count();
		}
	};
}

#endif