#include "MMotionState.h"


MMotionState::MMotionState(Primitiva* object) : 
obj(object)
{
};
MMotionState::~MMotionState(void)
{
};

/*btMotionState interface*/
void MMotionState::getWorldTransform (btTransform &worldTrans) const
{
    worldTrans.setFromOpenGLMatrix((btScalar*) obj->getOpenglMat());
};
void MMotionState::setWorldTransform (const btTransform &worldTrans)
{
    worldTrans.getOpenGLMatrix((btScalar*) obj->getOpenglMat());
};