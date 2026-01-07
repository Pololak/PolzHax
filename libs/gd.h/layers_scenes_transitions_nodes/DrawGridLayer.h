#ifndef __DRAWGRIDLAYER_H__
#define __DRAWGRIDLAYER_H__

#include <gd.h>

namespace gd {
	class LevelEditorLayer;

	class DrawGridLayer : public cocos2d::CCLayer {
	public:
		float m_songOffset1;
		float m_songOffset2;
		float m_lastMusicXPosition;
		LevelEditorLayer* m_levelEditorLayer;
		std::string m_guidelineString;
		cocos2d::CCNode* m_gameLayer;
		cocos2d::CCArray* m_timeMarkers;
		cocos2d::CCArray* m_effectObjects;
		cocos2d::CCArray* m_guideObjects;
		cocos2d::CCArray* m_speedObjects;
		cocos2d::CCArray* m_playerNodePoints;
		cocos2d::CCArray* m_player2NodePoints;
		double m_dUnused1;
		float m_guidelineSpacing;
		float m_slowGuidelineSpacing;
		float m_normalGuidelineSpacing;
		float m_fastGuidelineSpacing;
		float m_fasterGuidelineSpacing;
		bool m_timeNeedsUpdate;
		float m_activeGridNodeSize;

		void updateTimeMarkers() {
			reinterpret_cast<void(__fastcall*)(DrawGridLayer*)>(base + 0x932e0)(this);
		}

		float timeForXPos(float pos) {
			return reinterpret_cast<float(__vectorcall*)(
				float, float, float, float, float, float,
				DrawGridLayer*
				)>(base + 0x934f0)(0.f, pos, 0.f, 0.f, 0.f, 0.f, this);
		}

		float xPosForTime(float time) {
			__asm movss xmm1, time

			return reinterpret_cast<float(__vectorcall*)(DrawGridLayer*)>(base + 0x93350)(this);
		}
	};
}

#endif // !__DRAWGRIDLAYER_H__
