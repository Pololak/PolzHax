#ifndef __SETGROUPIDLAYER_H__
#define __SETGROUPIDLAYER_H__

#include <gd.h>

namespace gd {
	class GameObject;

	class SetGroupIDLayer : public FLAlertLayer {
	public:
		GameObject* m_targetObject; // 0x1bc
		cocos2d::CCArray* m_targetObjects; // 0x1c0
		cocos2d::CCLabelBMFont* m_groupLabel; // 0x1c4
		int m_groupIndex; // 0x1c8

		void updateGroupID() {
			reinterpret_cast<void(__fastcall*)(SetGroupIDLayer*)>(base + 0xf7e80)(this);
		}

		void updateLabel() {
			if (m_groupIndex < 0) {
				m_groupLabel->setString("Mixed");
			}
			else {
				m_groupLabel->setString(cocos2d::CCString::createWithFormat("%i", m_groupIndex)->getCString());
			}
		}
	};
}

#endif // !__SETGROUPIDLAYER_H__
