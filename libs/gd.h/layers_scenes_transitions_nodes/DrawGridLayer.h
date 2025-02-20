#ifndef __DRAWGRIDLAYER_H__
#define __DRAWGRIDLAYER_H__

#include <gd.h>

namespace gd {
	class DrawGridLayer : public cocos2d::CCLayer {
	public:
		float m_songOffset1; // 0x118
		float m_songOffset2; // 0x11c
		float m_lastMusicXPos; // 0x120
		LevelEditorLayer* m_levelEditorLayer; // 0x124
		std::string m_guidelineString; // 0x128
		cocos2d::CCNode* m_gameLayer; // 0x140
		cocos2d::CCArray* m_timeMarkers; // 0x144
		cocos2d::CCArray* m_effectObjects; // 0x148
		cocos2d::CCArray* m_guideObjects; // 0x14c
		cocos2d::CCArray* m_speedObjects; // 0x150
		cocos2d::CCArray* m_playerNodePoints; // 0x154
		cocos2d::CCArray* m_player2NodePoints; // 0x158
		PAD(4) // smth unused on pc
		PAD(8)
		float m_guidelineSpacing; // 0x168
		float m_slowGuidelineSpacing; // 0x16c
		float m_normalGuidelineSpacing; // 0x170
		float m_fastGuidelineSpacing; // 0x174
		float m_fasterGuidelineSpacing; // 0x178
		bool m_timeNeedsUpdate; // 0x17c
		float m_activeGridNodeSize; // 0x180

		float timeForXPos(float pos) {
			return reinterpret_cast<float(__vectorcall*)(
				float, float, float, float, float, float,
				DrawGridLayer*
				)>(base + 0x934f0)(0.f, pos, 0.f, 0.f, 0.f, 0.f, this);
		}
		auto getPlaybackLinePos() {
			return from<float>(this, 0x120);
		}
		LevelEditorLayer* getEditorLayer() {
			return from<LevelEditorLayer*>(this, 0x124);
		}
	};
}

#endif
