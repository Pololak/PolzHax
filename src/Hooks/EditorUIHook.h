#pragma once
#include "../include.h"

namespace EditorUIHook {
    EditorUI* get();

    inline bool(*init)(EditorUI*, LevelEditorLayer*);
    bool initH(EditorUI*, LevelEditorLayer*);

    inline void(*selectObject)(EditorUI*, GameObject*);
    void selectObjectH(EditorUI*, GameObject*);

    inline void(*selectObjects)(EditorUI*, CCArray*);
    void selectObjectsH(EditorUI*, CCArray*);

    inline void(*moveObject)(EditorUI*, GameObject*, CCPoint);
    void moveObjectH(EditorUI*, GameObject*, CCPoint);

    inline void(*angleChanged)(EditorUI*, float);
    void angleChangedH(EditorUI*, float);

    inline void(*updateButtons)(EditorUI*);
    void updateButtonsH(EditorUI*);

    inline void(*clickOnPosition)(EditorUI*, CCPoint);
    void clickOnPositionH(EditorUI*, CCPoint);

    inline void(*transformObject)(EditorUI*, GameObject*, EditCommand, bool);
    void transformObjectH(EditorUI*, GameObject*, EditCommand, bool);

    inline void(*onGroupDown)(EditorUI*, CCObject*);
    void onGroupDownH(EditorUI*, CCObject*);

    inline void(*onGroupUp)(EditorUI*, CCObject*);
    void onGroupUpH(EditorUI*, CCObject*);

    inline void(*destructor)(EditorUI*);
    void destructorH(EditorUI*);

    void updateObjectInfoLabel(EditorUI*);
    void updateGuideTogglePosition(EditorUI*);

    class Callback : EditorUI {
    public:
        void onAllGroup(CCObject*);
        void onNextFreeGroup(CCObject*);
        void onGoToGroup(CCObject*);
    };

    void mem_init();
}