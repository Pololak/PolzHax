#ifndef __EDITLEVELLAYER_H__
#define __EDITLEVELLAYER_H__

#include <gd.h>

namespace gd
{
	class GJGameLevel;
	class TextArea;
	enum class GJLevelType;

	class EditLevelLayer : public cocos2d::CCLayer, TextInputDelegate, FLAlertLayerProtocol {
	public:
		cocos2d::CCMenu* m_buttonMenu; // 0x120
		GJGameLevel* m_level; // 0x124
		TextArea* m_descInput; // 0x128
		cocos2d::CCArray* m_textInputs; // 0x12c
		bool m_isLoadingLevel; // 0x130
		GJLevelType m_levelType; // 0x134
		gd::string m_levelName; // 0x138

		static auto scene(GJGameLevel* level) {
			return reinterpret_cast<cocos2d::CCScene * (__fastcall*)(GJGameLevel*)>(base + 0x3b4c0)(level); //0x3b4c0 - GD 1.92
		}
	};

}

#endif