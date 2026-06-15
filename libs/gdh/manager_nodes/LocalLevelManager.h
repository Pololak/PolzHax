#ifndef __LOCALLEVELMANAGER_H__
#define __LOCALLEVELMANAGER_H__

#include "../include/gd.h"

class LocalLevelManager : public GManager {
public:
    cocos2d::CCDictionary* m_mainLevels; // 0x104
    cocos2d::CCArray* m_localLevels; // 0x108

    static LocalLevelManager* sharedState();

    void updateLevelOrder();

    cocos2d::CCArray* getAllLevelsWithName(std::string);
};

#endif // !__LOCALLEVELMANAGER_H__
