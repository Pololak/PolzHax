#pragma once
#include "cocos2d.h"
#include "pch.h"

namespace lvl_share {
	void init();
}

namespace EditLevelLayer {
	class Callback {
	public:
		void onMoveToTop(CCObject*);
	};
}