#ifndef _MPHYSICSWORLD_H_
#define _MPHYSICSWORLD_H_

#include <btBulletDynamicsCommon.h>
#include <vector>

#include <GL\glew.h> //test purposes only
#include "MMotionState.h" //our motionstate

//supported objects

class MPhysicsWorld {

public:
    MPhysicsWorld();
    ~MPhysicsWorld();

private:

    btDynamicsWorld *m_pDynamicsWorld;
    std::vector <btRigidBody*> m_RigidBodies;
    
    //debug rigid bodies
    btRigidBody* fallRigidBody;

public:

    void init();
    void update(float timeStep);
    void addBody(Primitiva* cub);
    btDynamicsWorld * getPhysicsWorld() {return m_pDynamicsWorld;} ;
};

#endif