#ifndef __UNDOOBJECT_H__
#define __UNDOOBJECT_H__

#include <gd.h>

namespace gd {
	class GameObject;

	enum class UndoCommand {
		Delete = 1,
		New = 2,
		Paste = 3,
		DeleteMulti = 4,
		Transform = 5,
		Select = 6,
	};

	class GameObjectCopy : public cocos2d::CCObject {
	public:
		GameObject* m_object; // 0x18
		cocos2d::CCPoint m_position; // 0x1c
		float m_rotation; // 0x24
		bool m_flipX; // 0x28
		bool m_flipY; // 0x29
	};

	class UndoObject : public cocos2d::CCObject {
	public:
		GameObjectCopy* m_object; // 0x18
		UndoCommand m_command; // 0x1c
		cocos2d::CCArray* m_objects; // 0x20
		bool m_isLinked; // 0x24

		static UndoObject* createWithArray(cocos2d::CCArray* objects, UndoCommand command) {
			return reinterpret_cast<UndoObject*(__fastcall*)(cocos2d::CCArray*, UndoCommand)>(base + 0x92970)(objects, command);
		}

		static UndoObject* createWithTransformObjects(cocos2d::CCArray* objects, UndoCommand command) {
			return reinterpret_cast<UndoObject*(__fastcall*)(cocos2d::CCArray*, UndoCommand)>(base + 0x92770)(objects, command);
		}
	};
}

#endif // !__UNDOOBJECT_H__
