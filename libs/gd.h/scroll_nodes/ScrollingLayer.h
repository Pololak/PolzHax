#ifndef __SCROLLINGLAYER_H__
#define __SCROLLINGLAYER_H__

#include <gd.h>

namespace gd {
	class ScrollingLayer : public cocos2d::CCLayerColor {
		public:
			PAD(0x28)
			float m_layerHeight; // 0x1bc
			PAD(0x4)
			cocos2d::CCLayer* m_internalLayer; // 0x1c4
			cocos2d::CCNode* m_parent; // 0x1c8

			static ScrollingLayer * create(cocos2d::CCSize _size, cocos2d::CCPoint _point, float _unknown) {
				auto pRet = reinterpret_cast<ScrollingLayer*(__stdcall*)(
					cocos2d::CCSize, cocos2d::CCPoint, float
				)>(
					base + 0x1c4a0
				)(
					_size, _point, _unknown
				);

				__asm add esp, 0x14

				return pRet;
			}
	};
}

#endif