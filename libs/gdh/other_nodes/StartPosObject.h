#ifndef __STARTPOSOBJECT_H__
#define __STARTPOSOBJECT_H__

#include "../include/gd.h"

class LevelSettingsObject;

class StartPosObject : public GameObject {
public:
    LevelSettingsObject* m_settings;
};

#endif
