#ifndef __LEVELEDITORLAYER_H__
#define __LEVELEDITORLAYER_H__

#include <gd.h>

namespace gd {

class GJBaseGameLayer;
class EditorUI;
class GameObject;

class LevelEditorLayer : public GJBaseGameLayer {
    public:
        PAD(0x84)
        int m_nCurrentLayer;
        PAD(0x2c)
        EditorUI* m_pEditorUI;

    public:
        EditorUI* getEditorUI() {
            return from<EditorUI*>(this, 0x15C);
        }

        void removeObject(GameObject * obj, bool idk) {
            reinterpret_cast<void(__thiscall*)(
                LevelEditorLayer *,GameObject *,bool
            )>(
                base + 0x161cb0
            )(
                this, obj, idk
            );
        }

        int getNextFreeGroupID(cocos2d::CCArray* objs) {
            return reinterpret_cast<int(__thiscall*)(
                LevelEditorLayer*, cocos2d::CCArray*
            )>(
                base + 0x164ae0
            )(
                this, objs
            );
        }

        GameObject* addObjectFromString(std::string const& str) {
            auto ret = reinterpret_cast<GameObject*(__thiscall*)(
                LevelEditorLayer*, std::string
            )>(
                base + 0x160c80
            )(
                this, str
            );

            // no stack fix required... apparently

            return ret;
        }
    
        cocos2d::CCArray* getAllObjects() {
            return from<cocos2d::CCArray*>(this, 0x16C);
        }
        void removeAllObjects() {
            return reinterpret_cast<void(__thiscall*)(LevelEditorLayer*)>(base + 0x8d770)(this);
        }
        int getLayerGroup()
        {
            return from<int>(this, 0x12C);
        }
        //0x8d770

        int getCurrentLayer() { return m_nCurrentLayer; }
        void setCurrentLayer(int n) { m_nCurrentLayer = n; }
    };

}

#endif
