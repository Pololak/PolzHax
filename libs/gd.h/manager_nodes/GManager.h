#ifndef __GMANAGER_H__
#define __GMANAGER_H__

#include <gd.h>

class DS_Dictionary;

namespace gd {
	class GManager : public cocos2d::CCNode {
	public:
		std::string m_fileName; // 0xe8
		bool m_setup; // 0x100
		bool m_saved; // 0x101
		bool m_quickSave; // 0x102

		void load() {
			reinterpret_cast<void(__fastcall*)(GManager*)>(base + 0x193c0)(this);
		}
	};
}

#endif