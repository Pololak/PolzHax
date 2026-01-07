#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <gd.h>

namespace gd {
	class CCSpritePlus;
	class OBB2D;

	enum class GameObjectType {
		Solid = 0,
		Basic = 1,
		Hazard = 2,
		InverseGravityPortal = 3,
		NormalGravityPortal = 4,
		ShipPortal = 5,
		CubePortal = 6,
		Decoration = 7,
		PulsingDecoration = 8,
		YellowJumpPad = 9,
		PinkJumpPad = 10,
		GravityPad = 11,
		YellowJumpRing = 12,
		PinkJumpRing = 13,
		GravityRing = 14,
		NormalMirrorPortal = 15,
		InverseMirrorPortal = 16,
		BallPortal = 17,
		RegularSizePortal = 18,
		MiniSizePortal = 19,
		UfoPortal = 20,
		Modifier = 21,
		Breakable = 22,
		SecretCoin = 23,
		DualPortal = 24,
		SoloPortal = 25,
		Slope = 26,
		WavePortal = 27
	};

	enum class GJCustomColorMode {
		Default = 0,
		PCol1 = 1,
		PCol2 = 2,
		Col1 = 3,
		Col2 = 4,
		LightBG = 5,
		Col3 = 6,
		Col4 = 7,
		Col3DL = 8
	};

	class GameObject : public CCSpritePlus {
	public:
		bool m_isFlippedX; // 0x1c4
		bool m_isFlippedY; // 0x1c5
		cocos2d::CCPoint m_boxOffset; // 0x1c8
		OBB2D* m_objectOBB2D; // 0x1d0
		bool m_isOriented; // 0x1d4
		cocos2d::CCSprite* m_glowSprite; // 0x1d8
		bool m_interactable; // 0x1dc
		bool m_isRingObject; // 0x1dd
		cocos2d::CCAction* m_myAction; // 0x1e0
		bool m_isRotatingObject; // 0x1e4
		bool m_objectPoweredOn; // 0x1e5
		bool m_hideObject; // 0x1e6
		float m_orientedBoxWidth; // 0x1e8
		float m_orientedBoxHeight; // 0x1ec
		bool m_isActive; // 0x1f0
		bool m_hasGlow; // 0x1f1
		bool m_isDestroyed; // 0x1f2
		cocos2d::CCParticleSystemQuad* m_particleSystem; // 0x1f4
		std::string m_particleString; // 0x1f8
		bool m_particleAdded; // 0x210
		cocos2d::CCPoint m_portalPosition; // 0x214
		cocos2d::CCRect m_objectTextureRect; // 0x21c
		bool m_textureRectDirty; // 0x22c
		float m_fadeInPosOffset; // 0x230
		cocos2d::CCRect m_objectRect2; // 0x234
		bool m_isObjectRectDirty; // 0x244
		bool m_isOrientedObjectRectDirty; // 0x245
		bool m_hasBeenActivatedP1; // 0x246
		bool m_hasBeenActivatedP2; // 0x247
		bool m_canChangeCustomColor; // 0x248
		bool m_hasCustomChildren; // 0x249
		bool m_hasColor; // 0x24a
		cocos2d::CCSprite* m_colorSprite; // 0x24c
		bool m_ignoreScreenCheck; // 0x250
		float m_objectRadius; // 0x254
		bool m_isRotated; // 0x258
		float m_scaleModX; // 0x25c
		float m_scaleModY; // 0x260
		int m_ID; // 0x264
		GameObjectType m_objectType; // 0x268
		int m_sectionIdx; // 0x26c
		bool m_shouldSpawn; // 0x270
		bool m_touchTriggered; // 0x271
		cocos2d::CCPoint m_startPosition; // 0x274
		std::string m_textureName; // 0x27c // m_frame
		bool m_isDisabled; // 0x294
		bool m_useAudioScale; // 0x295
		bool m_isSleeping; // 0x296
		float m_startRotation; // 0x298
		float m_startScaleX; // 0x29c
		float m_startScaleY; // 0x2a0
		bool m_startFlipX; // 0x2a4
		bool m_startFlipY; // 0x2a5
		bool m_shouldHide; // 0x2a6
		float m_spawnXPos; // 0x2a8
		bool m_isInvisible; // 0x2ac
		float m_enterAngle; // 0x2b0
		int m_activeEnterEffect; // 0x2b4
		cocos2d::ccColor3B m_triggerColor; // 0x2b8
		float m_triggerDuration; // 0x2bc
		bool m_triggerTintGround; // 0x2c0
		int m_objectID; // 0x2c4
		bool m_dontEnter; // 0x2c8
		bool m_dontFade; // 0x2c9
		bool m_dontFadeTinted; // 0x2ca
		bool m_isTintObject; // 0x2cb
		bool m_stateVar; // 0x2cc
		int m_objectZ; // 0x2d0 // Z Order
		cocos2d::CCNode* m_objectParent; // 0x2d4
		bool m_customAudioScale; // 0x2d8
		float m_minAudioScale; // 0x2dc
		float m_maxAudioScale; // 0x2e0
		int m_secretCoinID; // 0x2e4 // m_uniqueID
		bool m_invisibleMode; // 0x2e8
		bool m_glowUseBGColor; // 0x2e9
		bool m_useSpecialLight; // 0x2ea
		bool m_glowUseReverseColor; // 0x2eb
		bool m_isColorObject; // 0x2ec
		float m_opacityMod; // 0x2f0 // object opacity
		float m_glowOpacityMod; // 0x2f4 // object glow opacity
		bool m_dontShow; // 0x2f8
		bool m_upSlope;  // 0x2f9
		int m_slopeType; // 0x2fc
		float m_slopeAngle; // 0x300
		bool m_hazardousSlope; // 0x304
		GJCustomColorMode m_customColorMode; // 0x308 // object color
		GJCustomColorMode m_defaultColorMode; // 0x30c
		bool m_forceBottomLayer; // 0x310
		bool m_shouldPreview; // 0x311 // m_editorSelected (why it named like this in .so)
		bool m_copyPlayerColor1; // 0x312
		bool m_copyPlayerColor2; // 0x313
		bool m_triggerBlending; // 0x314
		bool m_wasSelected; // 0x315
		bool m_isSelected; // 0x316 // in editor
		int m_editorSelectIdx; // 0x318
		cocos2d::CCPoint m_storedPosition; // 0x31c
		int m_editorGroup; // 0x324

		GJCustomColorMode getColorMode() {
			GJCustomColorMode customColor = this->m_customColorMode;
			if ((this->m_defaultColorMode == customColor) || this->m_canChangeCustomColor || (customColor == GJCustomColorMode::Default)) {
				customColor = this->m_defaultColorMode;
			}
			return customColor;
		}

		bool canRotateFree() {
			return m_objectType != GameObjectType::Solid && m_objectType != GameObjectType::Breakable && m_objectType != GameObjectType::Slope;
		}

		void setChildColor(cocos2d::ccColor3B color) {
			if (m_hasColor) {
				if (m_colorSprite) {
					m_colorSprite->setColor(color);
				}
			}
		}

		void setObjectColor(cocos2d::ccColor3B color) {
			reinterpret_cast<void(__thiscall*)(GameObject*, cocos2d::ccColor3B)>(base + 0x75560)(this, color);
		}

		void destroyObject() {
			reinterpret_cast<void(__fastcall*)(GameObject*)>(base + 0x72f90)(this);
		}

		void calculateOrientedBox() {
			reinterpret_cast<void(__fastcall*)(GameObject*)>(base + 0x75a20)(this);
		}
	};

	class RingObject : public GameObject {
	public:
	};
}

#endif