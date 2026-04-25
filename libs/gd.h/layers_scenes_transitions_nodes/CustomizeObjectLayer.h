#ifndef __CUSTOMIZEOBJECTLAYER_H__
#define __CUSTOMIZEOBJECTLAYER_H__

#include <gd.h>

namespace gd {
	class GameObject;
	class ButtonSprite;

	class CustomizeObjectLayer : public FLAlertLayer {
	public:
		GameObject* m_targetObject; // 0x1bc
		cocos2d::CCArray* m_targetObjects; // 0x1c0
		cocos2d::CCArray* m_buttonsArray; // 0x1c4

		virtual bool init(GameObject* object, cocos2d::CCArray* objects) {
			return reinterpret_cast<bool(__thiscall*)(CustomizeObjectLayer*, GameObject*, cocos2d::CCArray*)>(base + 0x2dc70)(this, object, objects);
		}

		void highlightSelected(ButtonSprite* btn) {
			reinterpret_cast<void(__thiscall*)(CustomizeObjectLayer*, ButtonSprite*)>(base + 0x2e730)(this, btn);
		}

		void onSelectColor(cocos2d::CCObject* sender) {
			reinterpret_cast<void(__thiscall*)(CustomizeObjectLayer*, cocos2d::CCObject*)>(base + 0x2e600)(this, sender);
		}
	};
}

#endif
