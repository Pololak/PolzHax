#ifndef __LOCALLEVELMANAGER_H__
#define __LOCALLEVELMANAGER_H__

#include <gd.h>

namespace gd {
	class LocalLevelManager : public GManager {
	public:
		cocos2d::CCDictionary* m_mainLevels; // 0x104
		cocos2d::CCArray* m_localLevels; // 0x108

		static LocalLevelManager* sharedState() {
			return reinterpret_cast<LocalLevelManager * (__stdcall*)()>(base + 0xac180)();
		}

		void updateLevelOrder() {
			reinterpret_cast<void(__fastcall*)(LocalLevelManager*)>(base + 0xacce0)(this);
		}
	};
}

#endif // !__LOCALLEVELMANAGER_H__
