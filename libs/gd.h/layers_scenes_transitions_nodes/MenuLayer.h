#ifndef __MENULAYER_H__
#define __MENULAYER_H__

#include <gd.h>

namespace gd {

	class FLAlertLayerProtocol;
	class PlayerObject;

	class MenuGameLayer : public cocos2d::CCLayer {
	public:
		bool m_isDestroyingPlayer; // 0x118
		int m_initCount; // 0x11c
		cocos2d::CCPoint m_backgroundPosition; // 0x120
		PlayerObject* m_playerObject; // 0x128
		cocos2d::CCSprite* m_backgroundSprite; // 0x12c
		cocos2d::CCSprite* m_groundSprite; // 0x130
		cocos2d::CCLayer* m_groundLayer; // 0x134
		float m_groundSpeed; // 0x138
		float m_backgroundSpeed; // 0x13c
	};

	class MenuLayer : public cocos2d::CCLayer, public gd::FLAlertLayerProtocol {
	public:
		static MenuLayer* node() {
			return reinterpret_cast<MenuLayer* (__stdcall*)()>(
				base + 0x190550
				)();
		}
		static cocos2d::CCScene* scene(bool options) {
			return reinterpret_cast<cocos2d::CCScene* (__fastcall*)(bool)>(
				base + 0x190720
				)(options);
		}
	};
}

#endif