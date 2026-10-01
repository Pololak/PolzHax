#pragma once
#include "pch.h"
#include <unordered_map>
#include "PolzBot.hpp"
#include <map>

struct PlayerCheckpointData {
    cocos2d::CCPoint m_position;
    float m_rotation;

    double m_slopeStartTime;

    int m_lastCollisionIdBottom;
    int m_lastCollisionIdTop;
    bool m_isRotating;
    bool m_checkCollisions;

    float m_slopeAngle;
    int m_collidingWithSlopeId;
    bool m_slopeFlipGravityRelated;

    double m_speed;
    double m_yStart;
    double m_gravity;
    float m_trailingParticleLife;
    float m_gameModeChangedTime;
    bool m_bUnknown4;
    bool m_bUnknown5;
    bool m_placedJumpCheckpoint;
    bool m_hasPlayLayer;
    bool m_bUnknown6;
    bool m_particlesActive;
    bool m_flyFallParticlesActive;
    bool m_inBallFall;
    bool m_bUnknown8;
    double m_lastCheckpointTime;
    double m_lastJumpTime;
    double m_lastFlipTime;
    bool m_bUnknown9;
    bool m_bUnknown10;
    double m_accelerationOrSpeed;
    double m_snapDistance;

    int m_onFlyCheckpointTries;
    cocos2d::CCPoint m_lastUpdatePos;

    bool m_useLandParticles;

    float m_landParticlesAngle;
    float m_landParticleRelatedY;
    int m_streakType;
    float m_slopeRotation;
    float m_currentSlopeYVelocity;
    float m_lastPlayerYPosOnSlope;
    bool m_inBoost;
    bool m_isCurrentSlopeTop;
    float m_updateCollideBottom;
    float m_updateCollideTop;
    bool m_hardStreakActive;
    bool m_canPlaceCheckpoint;
    bool m_tookDamage;

    double m_yVelocity;
    bool m_onSlope;
    bool m_wasOnSlope;
    float m_slopeYVel;
    bool m_flyMode;
    bool m_birdMode;
    bool m_rollMode;
    bool m_dartMode;
    bool m_gravityFlipped;
    bool m_isDead;
    bool m_canJump;
    float m_playerScale;
    float m_timeMod;
    cocos2d::CCPoint m_lastPos;
    cocos2d::CCPoint m_portalPos;

    bool m_onGround;
    bool m_isJumping;
    bool m_isLocked;
    bool m_allowRollJump;
    cocos2d::CCPoint m_lastGroundPos;

    gd::GameObject* m_portalObject;
    bool m_hasJumped;
    bool m_hasRingJumped;

    cocos2d::CCPoint m_realPlayerPos;
    bool m_isSecondPlayer;
    bool m_dualMode;
    double m_clkTimer;
    bool m_disableEffects;

    float m_audioScale;
    float m_groundHeight;

    void store(gd::PlayerObject* player) {
        m_position = player->getPosition();
        m_rotation = player->getRotation();

        m_slopeStartTime =             player->m_slopeStartTime;

        m_lastCollisionIdBottom =      player->m_lastCollisionIdBottom;
        m_lastCollisionIdTop =         player->m_lastCollisionIdTop;
        m_isRotating =                 player->m_isRotating;
        m_checkCollisions =            player->m_checkCollisions;

        m_slopeAngle =                 player->m_slopeAngle;
        m_collidingWithSlopeId =       player->m_collidingWithSlopeId;
        m_slopeFlipGravityRelated =    player->m_slopeFlipGravityRelated;

        m_speed =                      player->m_speed;
        m_yStart =                     player->m_yStart;
        m_gravity =                    player->m_gravity;
        m_trailingParticleLife =       player->m_trailingParticleLife;
        m_gameModeChangedTime =        player->m_gameModeChangedTime;
        m_bUnknown4 =                  player->m_bUnknown4;
        m_bUnknown5 =                  player->m_bUnknown5;
        m_placedJumpCheckpoint =       player->m_placedJumpCheckpoint;
        m_hasPlayLayer =               player->m_hasPlayLayer;
        m_bUnknown6 =                  player->m_bUnknown6;
        m_particlesActive =            player->m_particlesActive;
        m_flyFallParticlesActive =     player->m_flyFallParticlesActive;
        m_inBallFall =                 player->m_inBallFall;
        m_bUnknown8 =                  player->m_bUnknown8;
        m_lastCheckpointTime =         player->m_lastCheckpointTime;
        m_lastJumpTime =               player->m_lastJumpTime;
        m_lastFlipTime =               player->m_lastFlipTime;
        m_bUnknown9 =                  player->m_bUnknown9;
        m_bUnknown10 =                 player->m_bUnknown10;
        m_accelerationOrSpeed =        player->m_accelerationOrSpeed;
        m_snapDistance =               player->m_snapDistance;

        m_onFlyCheckpointTries =       player->m_onFlyCheckpointTries;
        m_lastUpdatePos =              player->m_lastUpdatePos;

        m_landParticlesAngle =         player->m_landParticlesAngle;
        m_landParticleRelatedY =       player->m_landParticleRelatedY;
        m_streakType =                 player->m_streakType;
        m_slopeRotation =              player->m_slopeRotation;
        m_currentSlopeYVelocity =      player->m_currentSlopeYVelocity;
        m_lastPlayerYPosOnSlope =      player->m_lastPlayerYPosOnSlope;
        m_inBoost =                    player->m_inBoost;
        m_isCurrentSlopeTop =          player->m_isCurrentSlopeTop;
        m_updateCollideBottom =        player->m_updateCollideBottom;
        m_updateCollideTop =           player->m_updateCollideTop;
        m_hardStreakActive =           player->m_hardStreakActive;
        m_canPlaceCheckpoint =         player->m_canPlaceCheckpoint;
        m_tookDamage =                 player->m_tookDamage;

        m_yVelocity =                  player->m_yVelocity;
        m_onSlope =                    player->m_onSlope;
        m_wasOnSlope =                 player->m_wasOnSlope;
        m_slopeYVel =                  player->m_slopeYVel;
        m_flyMode =                    player->m_flyMode;
        m_birdMode =                   player->m_birdMode;
        m_rollMode =                   player->m_rollMode;
        m_dartMode =                   player->m_dartMode;
        m_gravityFlipped =             player->m_gravityFlipped;
        m_isDead =                     player->m_isDead;
        m_canJump =                    player->m_canJump;
        m_playerScale =                player->m_playerScale;
        m_timeMod =                    player->m_timeMod;
        m_lastPos =                    player->m_lastPos;
        m_portalPos =                  player->m_portalPos;

        m_onGround =                   player->m_onGround;
        m_isJumping =                  player->m_isJumping;
        m_isLocked =                   player->m_isLocked;
        m_allowRollJump =              player->m_allowRollJump;
        m_lastGroundPos =              player->m_lastGroundPos;

        m_portalObject =               player->m_portalObject;
        m_hasJumped =                  player->m_hasJumped;
        m_hasRingJumped =              player->m_hasRingJumped;

        m_realPlayerPos =              player->m_realPlayerPos;
        m_isSecondPlayer =             player->m_isSecondPlayer;
        m_dualMode =                   player->m_dualMode;
        m_clkTimer =                   player->m_clkTimer;
        m_disableEffects =             player->m_disableEffects;

        m_audioScale =                 player->m_audioScale;
        m_groundHeight =               player->m_groundHeight;
    }

    void apply(gd::PlayerObject* player) {
        player->setPosition(m_position);
        player->setRotation(m_rotation);

        player->m_slopeStartTime =             m_slopeStartTime;

        player->m_lastCollisionIdBottom =      m_lastCollisionIdBottom;
        player->m_lastCollisionIdTop =         m_lastCollisionIdTop;
        player->m_isRotating =                 m_isRotating;
        player->m_checkCollisions =            m_checkCollisions;

        player->m_slopeAngle =                 m_slopeAngle;
        player->m_collidingWithSlopeId =       m_collidingWithSlopeId;
        player->m_slopeFlipGravityRelated =    m_slopeFlipGravityRelated;

        player->m_speed =                      m_speed;
        player->m_yStart =                     m_yStart;
        player->m_gravity =                    m_gravity;
        player->m_trailingParticleLife =       m_trailingParticleLife;
        player->m_gameModeChangedTime =        m_gameModeChangedTime;
        player->m_bUnknown4 =                  m_bUnknown4;
        player->m_bUnknown5 =                  m_bUnknown5;
        player->m_placedJumpCheckpoint =       m_placedJumpCheckpoint;
        player->m_hasPlayLayer =               m_hasPlayLayer;
        player->m_bUnknown6 =                  m_bUnknown6;
        player->m_particlesActive =            m_particlesActive;
        player->m_flyFallParticlesActive =     m_flyFallParticlesActive;
        player->m_inBallFall =                 m_inBallFall;
        player->m_bUnknown8 =                  m_bUnknown8;
        player->m_lastCheckpointTime =         m_lastCheckpointTime;
        player->m_lastJumpTime =               m_lastJumpTime;
        player->m_lastFlipTime =               m_lastFlipTime;
        player->m_bUnknown9 =                  m_bUnknown9;
        player->m_bUnknown10 =                 m_bUnknown10;
        player->m_accelerationOrSpeed =        m_accelerationOrSpeed;
        player->m_snapDistance =               m_snapDistance;

        player->m_onFlyCheckpointTries =       m_onFlyCheckpointTries;
        player->m_lastUpdatePos =              m_lastUpdatePos;

        player->m_landParticlesAngle =         m_landParticlesAngle;
        player->m_landParticleRelatedY =       m_landParticleRelatedY;
        player->m_streakType =                 m_streakType;
        player->m_slopeRotation =              m_slopeRotation;
        player->m_currentSlopeYVelocity =      m_currentSlopeYVelocity;
        player->m_lastPlayerYPosOnSlope =      m_lastPlayerYPosOnSlope;
        player->m_inBoost =                    m_inBoost;
        player->m_isCurrentSlopeTop =          m_isCurrentSlopeTop;
        player->m_updateCollideBottom =        m_updateCollideBottom;
        player->m_updateCollideTop =           m_updateCollideTop;
        player->m_hardStreakActive =           m_hardStreakActive;
        player->m_canPlaceCheckpoint =         m_canPlaceCheckpoint;
        player->m_tookDamage =                 m_tookDamage;

        player->m_yVelocity =                  m_yVelocity;
        player->m_onSlope =                    m_onSlope;
        player->m_wasOnSlope =                 m_wasOnSlope;
        player->m_slopeYVel =                  m_slopeYVel;
        player->m_flyMode =                    m_flyMode;
        player->m_birdMode =                   m_birdMode;
        player->m_rollMode =                   m_rollMode;
        player->m_dartMode =                   m_dartMode;
        player->m_gravityFlipped =             m_gravityFlipped;
        player->m_isDead =                     m_isDead;
        player->m_canJump =                    m_canJump;
        player->m_playerScale =                m_playerScale;
        player->m_timeMod =                    m_timeMod;
        player->m_lastPos =                    m_lastPos;
        player->m_portalPos =                  m_portalPos;

        player->m_onGround =                   m_onGround;
        player->m_isJumping =                  m_isJumping;
        player->m_isLocked =                   m_isLocked;
        player->m_allowRollJump =              m_allowRollJump;
        player->m_lastGroundPos =              m_lastGroundPos;

        player->m_portalObject =               m_portalObject;
        player->m_hasJumped =                  m_hasJumped;
        player->m_hasRingJumped =              m_hasRingJumped;

        player->m_realPlayerPos =              m_realPlayerPos;
        player->m_isSecondPlayer =             m_isSecondPlayer;
        player->m_dualMode =                   m_dualMode;
        player->m_clkTimer =                   m_clkTimer;
        player->m_disableEffects =             m_disableEffects;

        player->m_audioScale =                 m_audioScale;
        player->m_groundHeight =               m_groundHeight;
    }
};

struct CheckpointStorage {
    PlayerCheckpointData m_playerCheckpointData;
    PlayerCheckpointData m_player2CheckpointData;

    std::unordered_map<gd::GameObject*, std::pair<bool, bool>> m_activatedObjects;

    int m_currentFrame;
    unsigned int m_frameOffset;
    std::vector<PolzBot::Event> m_replayEventsVec;
};