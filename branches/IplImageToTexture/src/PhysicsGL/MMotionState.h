#ifndef _MMOTIONSTATE_H_
#define _MMOTIONSTATE_H_

#include <btBulletDynamicsCommon.h>
#include "Cube.h"

class MMotionState :
    public btMotionState
{
private:
    Primitiva* obj;

public:
    MMotionState(Primitiva* object);
    ~MMotionState(void);

    /*btMotionState interface*/
    void getWorldTransform (btTransform &worldTrans) const;
    void setWorldTransform (const btTransform &worldTrans);
};


#endif