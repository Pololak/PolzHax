#ifndef __SIMPLEPLAYER_H__
#define __SIMPLEPLAYER_H__

#include <gd.h>

namespace gd {
	class SimplePlayer : public cocos2d::CCSprite {
	public:
		cocos2d::CCSprite* m_firstLayer; // 0x1b8
		cocos2d::CCSprite* m_secondLayer; // 0x1bc
		cocos2d::CCSprite* m_birdDome; // 0x1c0
		cocos2d::CCSprite* m_outlineSprite; // 0x1c4
		int m_special; // 0x1c8

		static SimplePlayer* create(int iconID) {
			return reinterpret_cast<SimplePlayer * (__fastcall*)(int)>(base + 0x80770)(iconID);
		}

		void setFrames(char const* firstLayer, char const* secondLayer, const char* birdDome, const char* outlineSprite) {
			reinterpret_cast<void(__thiscall*)(SimplePlayer*, char const*, char const*, char const*, char const*)>(base + 0x80ef0)(this, firstLayer, secondLayer, birdDome, outlineSprite);
		}

		void setSecondColor(cocos2d::ccColor3B const& color) {
			reinterpret_cast<void(__thiscall*)(SimplePlayer*, cocos2d::ccColor3B const&)>(base + 0x80b50)(this, color);
		}

		void updateColors() {
			reinterpret_cast<void(__thiscall*)(SimplePlayer*)>(base + 0x80b80)(this);
		}

		void updatePlayerFrame(int iconID, IconType iconType) {
			reinterpret_cast<void(__thiscall*)(SimplePlayer*, int, IconType)>(base + 0x80c70)(this, iconID, iconType);
		}

		virtual void setColor(cocos2d::ccColor3B const& color) {
			return reinterpret_cast<void(__thiscall*)(SimplePlayer*, cocos2d::_ccColor3B const&)>(base + 0x80b20)(this, color);
		}
	};
}

#endif // !__SIMPLEPLAYER_H__
