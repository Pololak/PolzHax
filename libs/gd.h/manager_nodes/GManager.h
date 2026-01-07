#ifndef __GMANAGER_H__
#define __GMANAGER_H__

#include <gd.h>

class DS_Dictionary;

namespace gd {
	class GManager : public cocos2d::CCNode {
	protected:
		std::string m_fileName;
		bool m_setup;
		bool m_saved;
		bool m_quickSave;
	};
}

#endif