#ifndef __OBJECTTOOLBOX_H__
#define __OBJECTTOOLBOX_H__

#include <gd.h>

namespace gd {
	class ObjectToolbox : public cocos2d::CCNode {
	public:
		cocos2d::CCDictionary* m_frameToKeyDict; // 0xe8
		cocos2d::CCDictionary* m_keyToFrameDict; // 0xec

		static ObjectToolbox* sharedState() {
			return reinterpret_cast<ObjectToolbox * (__stdcall*)()>(base + 0xb5140)();
		}

		const char* intKeyToFrame(int key) {
			return m_keyToFrameDict->valueForKey(key)->getCString();
		}

		float gridNodeSizeForKey(int key) {
			return reinterpret_cast<float(__thiscall*)(ObjectToolbox*, int)>(base + 0xcfc90)(this, key);
		}
	};
}

#endif // !__OBJECTTOOLBOX_H__
