#ifndef __GMANAGER_H__
#define __GMANAGER_H__

#include "../include/gd.h"

class GManager : public cocos2d::CCNode {
public:
    std::string m_fileName;
    bool m_setup;
    bool m_saved;
    bool m_quickSave;
};

#endif