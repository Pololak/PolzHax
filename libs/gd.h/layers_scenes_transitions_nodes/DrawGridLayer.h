#ifndef __DRAWGRIDLAYER_H__
#define __DRAWGRIDLAYER_H__

#include <gd.h>

namespace gd {
	class LevelEditorLayer;

	class DrawGridLayer : public cocos2d::CCLayer {
	public:
		float m_songOffset1; // 0x118
		float m_songOffset2; // 0x11c
		float m_lastMusicXPosition; // 0x120
		LevelEditorLayer* m_levelEditorLayer; // 0x124
		std::string m_guidelineString; // 0x128
		cocos2d::CCNode* m_gameLayer; // 0x140
		cocos2d::CCArray* m_timeMarkers; // 0x144
		cocos2d::CCArray* m_effectObjects; // 0x148
		cocos2d::CCArray* m_guideObjects; // 0x14c
		cocos2d::CCArray* m_speedObjects; // 0x150
		cocos2d::CCArray* m_playerNodePoints; // 0x154
		cocos2d::CCArray* m_player2NodePoints; // 0x158
		double m_dUnused1; // 0x15c
		float m_guidelineSpacing; // 0x164
		float m_slowGuidelineSpacing; // 0x168
		float m_normalGuidelineSpacing; // 0x16c
		float m_fastGuidelineSpacing; // 0x170
		float m_fasterGuidelineSpacing; // 0x174
		bool m_timeNeedsUpdate; // 0x178
		float m_activeGridNodeSize; // 0x17c

		void updateTimeMarkers() {
			reinterpret_cast<void(__fastcall*)(DrawGridLayer*)>(base + 0x932e0)(this);
		}

		float timeForXPos(float pos) {
			__asm movss xmm1, pos

			return reinterpret_cast<float(__vectorcall*)(DrawGridLayer*)>(base + 0x934f0)(this);

			/*return reinterpret_cast<float(__vectorcall*)(
				float, float, float, float, float, float,
				DrawGridLayer*
				)>(base + 0x934f0)(0.f, pos, 0.f, 0.f, 0.f, 0.f, this);*/
		}

		float xPosForTime(float time) {
			__asm movss xmm1, time

			return reinterpret_cast<float(__vectorcall*)(DrawGridLayer*)>(base + 0x93350)(this);
		}
	};
}

#endif // !__DRAWGRIDLAYER_H__
