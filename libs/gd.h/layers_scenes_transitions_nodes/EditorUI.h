#ifndef __EDITORUI_H__
#define __EDITORUI_H__

#include <gd.h>
#include "cocos2d.h"
#include "cocos-ext.h"

namespace gd {

    enum EditCommand {
        kEditCommandSmallLeft = 1,
        kEditCommandSmallRight = 2,
        kEditCommandSmallUp = 3,
        kEditCommandSmallDown = 4,

        kEditCommandLeft = 5,
        kEditCommandRight = 6,
        kEditCommandUp = 7,
        kEditCommandDown = 8,

        kEditCommandBigLeft = 9,
        kEditCommandBigRight = 10,
        kEditCommandBigUp = 11,
        kEditCommandBigDown = 12,

        kEditCommandTinyLeft = 13,
        kEditCommandTinyRight = 14,
        kEditCommandTinyUp = 15,
        kEditCommandTinyDown = 16,

        kEditCommandFlipX = 17,
        kEditCommandFlipY = 18,
        kEditCommandRotateCW = 19,
        kEditCommandRotateCCW = 20,
        kEditCommandRotateCW45 = 21,
        kEditCommandRotateCCW45 = 22,
        kEditCommandRotateFree = 23,
        kEditCommandRotateSnap = 24,

        kEditCommandScale = 25,
    };

class EditButtonBar : public cocos2d::CCNode {
public:
    cocos2d::CCArray* m_buttonArray;


};
class GJRotationControl : public cocos2d::CCLayer {};
class GJScaleControl : public cocos2d::CCLayer {};

class CCMenuItemSpriteExtra;
class CCMenuItemToggler;
class Slider;
class GameObject;

class BoomScrollLayer : public cocos2d::CCLayer {
public:
    void instantMoveToPage(int page) {
        reinterpret_cast<void(__thiscall*)(BoomScrollLayer*, int)>(
            base + 0x8430
            )(this, page);
    }

    void moveToPage(int page) {
        reinterpret_cast<void(__thiscall*)(BoomScrollLayer*, int)>(
            base + 0x8500
            )(this, page);
    }
};

class EditorUI : public cocos2d::CCLayer {
public:
    EditButtonBar* m_pEditButtonBar;
    EditButtonBar* m_pEditButtonBar2;



    auto pasteObjects(const std::string& str) {
        return reinterpret_cast<cocos2d::CCArray * (__thiscall*)(EditorUI*, gd::string)>(base + 0x492a0)(this, str);
    }

    void moveForCommand(cocos2d::CCPoint* pos, EditCommand* command) {
        reinterpret_cast<void(__thiscall*)(EditorUI*, cocos2d::CCPoint*, EditCommand*)>(base + 0x4b040)(this, pos, command);
    }

    //CCMenuItemSpriteExtra* getSpriteButton(
    //    const char* sprite,
    //    cocos2d::SEL_MenuHandler callback,
    //    cocos2d::CCMenu* menu,
    //    float scale
    //) {
    //    return reinterpret_cast<CCMenuItemSpriteExtra * (__thiscall*)(
    //        EditorUI*, const char*, cocos2d::SEL_MenuHandler,
    //        cocos2d::CCMenu*, float
    //        )>(
    //            base + 0x41790
    //            )(
    //                this, sprite, callback, menu, scale
    //                );
    //}

    void rotateObjects(cocos2d::CCArray* objects, float angle, cocos2d::CCPoint center) {
        __asm movss xmm2, angle;
        reinterpret_cast<void(__thiscall*)(
            EditorUI*, cocos2d::CCArray*, cocos2d::CCPoint
            )>(base + 0x4c280)(this, objects, center);
    }

    CCMenuItemSpriteExtra* getSpriteButton(
        const char* sprite,
        cocos2d::SEL_MenuHandler callback,
        cocos2d::CCMenu* menu,
        float scale,
        int idk,
        cocos2d::CCPoint* idk2,
        cocos2d::CCPoint* idk3
    ) {
        return reinterpret_cast<CCMenuItemSpriteExtra*(__thiscall*)(
            EditorUI*,
            const char*,
            cocos2d::SEL_MenuHandler,
            cocos2d::CCMenu*,
            float,
            int,
            cocos2d::CCPoint*,
            cocos2d::CCPoint*
            )>(
                base + 0x41790
            )(
            this, sprite, callback, menu, scale, idk, idk2, idk3
            );
    }

    void moveObjectCall(cocos2d::CCObject* pSender) {
        reinterpret_cast<void(__thiscall*)(EditorUI*, cocos2d::CCObject*)>(
            base + 0x4b2a0
            )(this, pSender);
    }

    void moveObjectCall(EditCommand command) {
        reinterpret_cast<void(__thiscall*)(EditorUI*, EditCommand)>(
            base + 0x4b2c0
            )(this, command);
    }

    auto& clipboard() {
        return from<gd::string>(this, 0x264);
    }

    auto selectedObjectToolboxID() {
        return from<int>(this, 0x220);
    }
    
    void updateButtons() {
        reinterpret_cast<void(__thiscall*)(EditorUI*)>(base + 0x41450)(this);
    }

    bool isPlayback() {
        return from<bool>(this, 0x134);
    }

    void updateZoom(float amt) {
        reinterpret_cast<void(__vectorcall*)(float, float, EditorUI*)>(base + 0x48c30)(0.f, amt, this);
    }

    LevelEditorLayer* getLevelEditorLayer() {
        return from<LevelEditorLayer*>(this, 0x22C);
    }

    void selectObjects(cocos2d::CCArray* arr) {
        return reinterpret_cast<void(__thiscall*)(EditorUI*, cocos2d::CCArray*)>(base + 0x47fa0)(this, arr); //0xf1f20 - GHS 1.92 // - Taswert: wtf is this??? Why did I put it here??? // - Polz: maybe for select all?
    }

    EditButtonBar* editButtonBar() {
        return from<EditButtonBar*>(this, 0x160);
    }

    cocos2d::CCArray* getSomeObjects() {
        return from<cocos2d::CCArray*>(this, 0x1E8);
    }

    CCMenuItemSpriteExtra* getRedoBtn() {
        return from<CCMenuItemSpriteExtra*>(this, 0x1c4);
    }

    CCMenuItemSpriteExtra* getDeselectBtn() {
        return from<CCMenuItemSpriteExtra*>(this, 0x1a8);
    }

    cocos2d::CCArray* getSelectedObjectsOfCCArray() {
        auto output = cocos2d::CCArray::create();
        gd::GameObject* single = from<gd::GameObject*>(this, 0x258);
        if (single)
        {
            output->addObject(reinterpret_cast<cocos2d::CCObject*>(single));
            return output;
        }
        return from<cocos2d::CCArray*>(this, 0x18c);
    }

    cocos2d::CCArray* getAllObjects() {
        return from<cocos2d::CCArray*>(this, 0x224); //no
    }

    std::vector<GameObject*> getSelectedObjects() {
        const auto single = from<GameObject*>(this, 0x258);
        if (single) return { single };
        const auto selectedArr = from<cocos2d::CCArray*>(this, 0x18c);
        if (!selectedArr) return {};
        std::vector<GameObject*> output;
        for (size_t i = 0; i < selectedArr->count(); ++i)
            output.push_back(reinterpret_cast<GameObject*>(selectedArr->objectAtIndex(i)));
        return output;
    }



    gd::GameObject* getSingleSelectedObj() {
        return from<gd::GameObject*>(this, 0x258);
    }

    gd::EditorUI* selectedObjectID() {
        return from<gd::EditorUI*>(this, 0x220);
    }

    void onDeleteSelected(CCObject* sender) {
        return reinterpret_cast<void(__thiscall*)(EditorUI*, CCObject*)>(base + 0x42bc0)(this, sender);
    }

    void onSettings(CCObject* sender) {
        return reinterpret_cast<void(__thiscall*)(EditorUI*, CCObject*)>(base + 0x41190)(this, sender);
    }

    void onDeselectAll(CCObject* sender) {
        return reinterpret_cast<void(__thiscall*)(EditorUI*, CCObject*)>(base + 0x48340)(this, sender);
    }

    void getCreateBtn() {
        return reinterpret_cast<void(__fastcall*)()>(base + 0x47200)();
    }

    int selectedMode() {
        return from<int>(this, 0x228);
    }

    static constexpr const int Mode_Create = 2;
    static constexpr const int Mode_Delete = 1;
    static constexpr const int Mode_Edit = 3;

};

class EditorPauseLayer : public gd::CCBlockLayer {
public:
    PAD(0x8)
        gd::CCMenuItemSpriteExtra* m_pButton0;  // 0x1a4
    gd::CCMenuItemSpriteExtra* m_pButton1;  // 0x1a8
    gd::LevelEditorLayer* m_pEditorLayer;   // 0x1ac

    void onResume(cocos2d::CCObject* pSender) {
        reinterpret_cast<void(__thiscall*)(EditorPauseLayer*, cocos2d::CCObject*)>(
            base + 0x3eea0
            )(this, pSender);
    }

    void saveLevel(cocos2d::CCObject* pSender) {
        reinterpret_cast<void(__thiscall*)(EditorPauseLayer*, cocos2d::CCObject*)>(
            base + 0x3eec0
            )(this, pSender);
    }

    CCMenuItemSpriteExtra* bpmButton() {
        return from<CCMenuItemSpriteExtra*>(this, 0x1a4);
    }

    LevelEditorLayer* getEditorLayer() {
        return from<LevelEditorLayer*>(this, 0xac);
    }
};

//class ColorSelectPopup : public CCLayer {
//public:
//    auto fadeTimeInTrigger() {
//        return from<float>(this, 0x1EC);
//    }
//};

class ColorSelectPopup : public gd::FLAlertLayer {
public:
    auto colorPicker() {
        return from<cocos2d::extension::CCControlColourPicker*>(this, 0x1c0);
    }
    auto fadeTime() {
        return from<float>(this, 0x1ec); //1d0
    }
};

/*
class EditorUI : public cocos2d::CCLayer,
    public gd::FLAlertLayerProtocol,
    public gd::ColorSelectDelegate,
    public gd::GJRotationControlDelegate,
    public gd::GJScaleControlDelegate,
    public gd::MusicDownloadDelegate {

    public:
        EditButtonBar* m_pEditButtonBar;
        PAD(0x4)
        cocos2d::CCArray* m_pUnknownArray;
        PAD(0x28)
        cocos2d::CCLabelBMFont* m_pUnknownLabel;
        GJRotationControl* m_pRotationControl;
        PAD(0x10)
        GJScaleControl* m_pScaleControl;
        cocos2d::CCDictionary* m_pUnknownDict;
        EditButtonBar* m_pEditButtonBar2;
        EditButtonBar* m_pEditButtonBar3;
        Slider* m_pPositionSlider;
        PAD(0x20)
        cocos2d::CCArray* m_pUnknownArray2;
        PAD(0x8)
        cocos2d::CCArray* m_pUnknownArray3;
        cocos2d::CCMenu* m_pUnknownMenu;
        cocos2d::CCArray* m_pUnknownArray4;
        CCMenuItemSpriteExtra* m_pButton0;
        CCMenuItemSpriteExtra* m_pButton1;
        CCMenuItemSpriteExtra* m_pButton2;
        CCMenuItemSpriteExtra* m_pButton3;
        CCMenuItemSpriteExtra* m_pButton4;
        CCMenuItemSpriteExtra* m_pButton5;
        CCMenuItemSpriteExtra* m_pButton6;
        CCMenuItemSpriteExtra* m_pButton7;
        CCMenuItemSpriteExtra* m_pButton8;
        CCMenuItemSpriteExtra* m_pButton9;
        CCMenuItemSpriteExtra* m_pButton10;
        CCMenuItemSpriteExtra* m_pButton11;
        CCMenuItemSpriteExtra* m_pButton12;
        CCMenuItemSpriteExtra* m_pButton13;
        CCMenuItemSpriteExtra* m_pButton14;
        CCMenuItemSpriteExtra* m_pButton15;
        CCMenuItemSpriteExtra* m_pButton16;
        CCMenuItemSpriteExtra* m_pButton17;
        CCMenuItemSpriteExtra* m_pButton18;
        CCMenuItemSpriteExtra* m_pButton19;
        CCMenuItemSpriteExtra* m_pButton20;
        CCMenuItemSpriteExtra* m_pButton21;
        CCMenuItemSpriteExtra* m_pButton22;
        CCMenuItemSpriteExtra* m_pButton23;
        CCMenuItemSpriteExtra* m_pButton24;
        CCMenuItemSpriteExtra* m_pButton25;
        CCMenuItemSpriteExtra* m_pButton26;
        CCMenuItemToggler* m_pUnknownToggler;
        cocos2d::CCArray* m_pUnknownArray5;
        cocos2d::CCMenu* m_pUnknownMenu2;
        cocos2d::CCArray* m_pUnknownArray6;
        cocos2d::CCSprite* m_pIdkSprite0;
        cocos2d::CCSprite* m_pIdkSprite1;
        CCMenuItemSpriteExtra* m_pButton27;
        CCMenuItemSpriteExtra* m_pButton28;
        CCMenuItemSpriteExtra* m_pButton29;
        CCMenuItemSpriteExtra* m_pButton30;
        CCMenuItemSpriteExtra* m_pButton31;
        CCMenuItemSpriteExtra* m_pButton32;
        cocos2d::CCLabelBMFont* m_pCurrentLayerLabel;
        CCMenuItemSpriteExtra* m_pButton33;
        CCMenuItemSpriteExtra* m_pButton34;
        CCMenuItemSpriteExtra* m_pButton35;
        PAD(0xc)
        cocos2d::CCArray* m_pUnknownArray7;
        cocos2d::CCArray* m_pUnknownArray8;
        cocos2d::CCArray* m_pUnknownArray9;
        PAD(0x4)
        LevelEditorLayer* m_pEditorLayer;
    
    public:
        cocos2d::CCArray* pasteObjects(std::string const& _str) {
            auto ret = reinterpret_cast<cocos2d::CCArray*(__thiscall*)(
                EditorUI*, std::string
            )>(
                base + 0x88240
            )(
                this, _str
            );

            return ret;
        }

        void deselectAll() {
            reinterpret_cast<void(__fastcall*)(EditorUI*)>(
                base + 0x86af0
            )(this);
        }

        void selectObjects(cocos2d::CCArray* objs, bool idk) {
            reinterpret_cast<void(__thiscall*)(
                EditorUI*, cocos2d::CCArray*, bool
            )>(
                base + 0x864a0
            )(
                this, objs, idk
            );
        }

        cocos2d::CCArray* getSelectedObjects() {
            return reinterpret_cast<cocos2d::CCArray*(__thiscall*)(EditorUI*)>(
                base + 0x86900
            )(this);
        }
};
*/

}

#endif
