#include "MPhysicsWorld.h"


MPhysicsWorld::MPhysicsWorld() :
m_pDynamicsWorld(0)
{
};

MPhysicsWorld::~MPhysicsWorld()
{
    delete(m_pDynamicsWorld);
};

void MPhysicsWorld::init()
{
    //initialize physics world
    if(!m_pDynamicsWorld) delete m_pDynamicsWorld; //check for consistency errors

    btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
    btCollisionDispatcher* dispatcher = new	btCollisionDispatcher (collisionConfiguration);
    btBroadphaseInterface* broadphase = new btDbvtBroadphase();
    btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

    //build world
    m_pDynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);
    m_pDynamicsWorld->setGravity(btVector3(0,-10,0));

    //setup shapes (debug only)
    btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0,1,0),1);

    //static body
    btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,-4,0)));
    btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));
    btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
    m_pDynamicsWorld->addRigidBody(groundRigidBody);


};
void MPhysicsWorld::update(float timeStep)
{
    m_pDynamicsWorld->stepSimulation(timeStep,10);
};
void MPhysicsWorld::addBody(Primitiva* cub)
{
    // get dynamic body shape 

    btCollisionShape* fallShape;
    switch(cub->type)
    {
    case CUBE:
        {
            Cube* cubo =(Cube*) cub;
            fallShape = new btBoxShape(btVector3(cubo->halfExtention->x,cubo->halfExtention->y,cubo->halfExtention->z));
            break;
        }
    case PARALELEPIPEDO:
        {
            Paralelepipedo* para = (Paralelepipedo*) cub;
            fallShape = new btBoxShape(btVector3(para->halfExtention->x,para->halfExtention->y,para->halfExtention->z));
            break;
        }
    case ESFERA:
        {
        }
    };

    //btDefaultMotionState* fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(3,0,-10)));
    MMotionState* fallMotionState = new MMotionState(cub);

    btScalar mass = 1;
    btVector3 fallInertia(0,0,0);
    fallShape->calculateLocalInertia(mass,fallInertia);
    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass,fallMotionState,fallShape,fallInertia);
    fallRigidBody = new btRigidBody(fallRigidBodyCI);
    m_pDynamicsWorld->addRigidBody(fallRigidBody);

};