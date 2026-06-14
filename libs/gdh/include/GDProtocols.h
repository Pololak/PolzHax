#ifndef __GDPROTOCOLS_H__
#define __GDPROTOCOLS_H__

#include "gd.h"

class FLAlertLayer;

class FLAlertLayerProtocol {
public:
    virtual void FLAlert_Clicked(FLAlertLayer*, bool) {};
};

#endif