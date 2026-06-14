#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "../include/gd.h"

class CCSpritePlus;
class OBB2D;

class GameObject : public CCSpritePlus {
public:
    bool m_isFlippedX;
    bool m_isFlippedY;
    cocos2d::CCPoint m_boxOffset;
    OBB2D* m_objectOBB2D;
    bool m_isOriented;
    cocos2d::CCSprite* m_glowSprite;
    bool m_interactable;
    bool m_isRingObject;
    cocos2d::CCAction* m_myAction;
    bool m_isRotatingObject;
    bool m_objectPoweredOn;
    bool m_hideObject;
    float m_orientedBoxWidth;
    float m_orientedBoxHeight;
    bool m_isActive;
    bool m_hasGlow;
    bool m_isDestroyed;
    cocos2d::CCParticleSystemQuad* m_particleSystem;
    std::string m_particleString;
    bool m_particleAdded;
    cocos2d::CCPoint m_portalPosition;
    cocos2d::CCRect m_objectTextureRect;
    bool m_textureRectDirty;
    float m_fadeInPosOffset;
    cocos2d::CCRect m_objectRect2;
    bool m_isObjectRectDirty;
    bool m_isOrientedObjectRectDirty;
    bool m_hasBeenActivatedP1;
    bool m_hasBeenActivatedP2;
    bool m_canChangeCustomColor;
    bool m_hasCustomChildren;
    bool m_hasColor;
    cocos2d::CCSprite* m_colorSprite;
    bool m_ignoreScreenCheck;
    float m_objectRadius;
    bool m_isRotated;
    float m_scaleModX;
    float m_scaleModY;
    int m_ID;
    GameObjectType m_objectType;
    int m_sectionIdx;
    bool m_shouldSpawn;
    bool m_touchTriggered;
    cocos2d::CCPoint m_startPosition;
    std::string m_textureName;
    bool m_isDisabled;
    bool m_useAudioScale;
    bool m_isSleeping;
    float m_startRotation;
    float m_startScaleX;
    float m_startScaleY;
    bool m_startFlipX;
    bool m_startFlipY;
    bool m_shouldHide;
    float m_spawnXPos;
    bool m_isInvisible;
    float m_enterAngle;
    int m_activeEnterEffect;
    cocos2d::ccColor3B m_triggerColor;
    float m_triggerDuration;
    bool m_triggerTintGround;
    int m_objectID;
    bool m_dontEnter;
    bool m_dontFade;
    bool m_dontFadeTinted;
    bool m_isTintObject;
    bool m_stateVar;
    int m_objectZ;
    cocos2d::CCNode* m_objectParent;
    bool m_customAudioScale;
    float m_minAudioScale;
    float m_maxAudioScale;
    int m_secretCoinID;
    bool m_invisibleMode;
    bool m_glowUseBGColor;
    bool m_useSpecialLight;
    bool m_glowUseReverseColor;
    bool m_isColorObject;
    float m_opacityMod;
    float m_glowOpacityMod;
    bool m_dontShow;
    bool m_upSlope;
    int m_slopeType;
    float m_slopeAngle;
    bool m_hazardousSlope;
    GJCustomColorMode m_customColorMode;
    GJCustomColorMode m_defaultColorMode;
    bool m_forceBottomLayer;
    bool m_shouldPreview;
    bool m_copyPlayerColor1;
    bool m_copyPlayerColor2;
    bool m_triggerBlending;
    bool m_wasSelected;
    bool m_isSelected;
    int m_editorSelectIdx;
    cocos2d::CCPoint m_storedPosition;
    int m_editorGroup;

    void setChildColor(cocos2d::_ccColor3B color); /*{
        if (m_hasColor) {
            if (m_colorSprite) {
                m_colorSprite->setColor(color);
            }
        }
    }*/
    
    void setObjectColor(cocos2d::_ccColor3B);
    GJCustomColorMode getColorMode();
};

#endif