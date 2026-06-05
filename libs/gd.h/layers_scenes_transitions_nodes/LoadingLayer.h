#ifndef __LOADINGLAYER_H__
#define __LOADINGLAYER_H__

#include <gd.h>

namespace gd {
	class TextArea;

	class LoadingLayer : public cocos2d::CCLayer {
	public:
		bool m_unknown; // 0x118
		bool m_unknown2; // 0x119
		int m_loadStep; // 0x11c
		cocos2d::CCLabelBMFont* m_caption; // 0x120
		TextArea* m_textArea; // 0x124
		cocos2d::CCSprite* m_sliderBar; // 0x128
		float m_sliderGrooveXPos; // 0x12c
		float m_sliderGrooveHeight; // 0x130
		bool m_fromRefresh; // 0x134
	};
}

#endif