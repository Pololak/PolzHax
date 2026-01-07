#ifndef __OBJECTTOOLBOX_H__
#define __OBJECTTOOLBOX_H__

#include <gd.h>

namespace gd {
	class ObjectToolbox : public cocos2d::CCNode {
	public:
		cocos2d::CCDictionary* m_frameToKey;
		cocos2d::CCDictionary* m_keyToFrame;
		PAD(4);

		static auto sharedState() {
			return reinterpret_cast<ObjectToolbox * (__stdcall*)()>(base + 0xb5140)();
		}

		virtual bool init() {
			return reinterpret_cast<bool(__thiscall*)(void*)>(base + 0xb51f0)(this);
		}

		void addObject(int id, const char* frame) {
			m_frameToKey->setObject(cocos2d::CCString::createWithFormat("%i", id), frame);
			m_keyToFrame->setObject(cocos2d::CCString::create(frame), id);
		}
	};
}

#endif
