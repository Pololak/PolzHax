#ifndef __EDITORPAUSELAYER_H__
#define __EDITORPAUSELAYER_H__

#include <gd.h>

namespace gd {
	class CCMenuItemSpriteExtra;
	class LevelEditorLayer;

	class EditorPauseLayer : public CCBlockLayer, public FLAlertLayerProtocol {
	public:
		bool m_exiting; // 0x19c
		CCMenuItemSpriteExtra* m_audioOnBtn; // 0x1a0
		CCMenuItemSpriteExtra* m_audioOffBtn; // 0x1a4
		LevelEditorLayer* m_levelEditorLayer; // 0x1a8

		void saveLevel() {
			reinterpret_cast<void(__thiscall*)(EditorPauseLayer*)>(base + 0x3eec0)(this);
		}

		void onResume(cocos2d::CCObject* sender) {
			reinterpret_cast<void(__thiscall*)(EditorPauseLayer*, CCObject*)>(base + 0x3eea0)(this, sender);
		}
	};
}

#endif // !__EDITORPAUSELAYER_H__
