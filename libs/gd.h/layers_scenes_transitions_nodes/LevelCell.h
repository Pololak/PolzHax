#ifndef __LEVELCELL_H__
#define __LEVELCELL_H__

#include <gd.h>

namespace gd {
	class GJGameLevel;

	class LevelCell : public TableViewCell {
	public:
		GJGameLevel* m_level; // 0x170
	};
}

#endif // !__LEVELCELL_H__
