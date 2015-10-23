#include "stdafx.h"
#include "PhysicsEngine.h"

PhysicsEngine::PhysicsEngine() :
	mCollisionConfiguration(0),
	mDispatcher(0),
	mBroadphase(0),
	mSolver(0),
	mDynamicsWorld(0)
{}

PhysicsEngine::~PhysicsEngine()
{
	exitPhysics();
}

void PhysicsEngine::exitPhysics()
{
	//cleanup in the reverse order of creation/initialization

	//remove the rigidbodies from the dynamics world and delete them
	int i;
	for (i=mDynamicsWorld->getNumCollisionObjects()-1; i>=0 ;i--)
	{
		btCollisionObject* obj = mDynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		mDynamicsWorld->removeCollisionObject( obj );
		delete obj;
	}

	//delete collision shapes
	for (int j=0;j<mCollisionShapes.size();j++)
	{
		btCollisionShape* shape = mCollisionShapes[j];
		delete shape;
	}
	mCollisionShapes.clear();

	delete mDynamicsWorld;
	delete mSolver;
	delete mBroadphase;
	delete mDispatcher;
	delete mCollisionConfiguration;
}

void PhysicsEngine::initPhysics()
{
	///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	mCollisionConfiguration = new btDefaultCollisionConfiguration();

	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	mBroadphase = new btDbvtBroadphase();
	
	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	mDispatcher = new btCollisionDispatcher(mCollisionConfiguration);

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	mSolver = new btSequentialImpulseConstraintSolver;

	// Create a dynamic simulation environment
	mDynamicsWorld = new btDiscreteDynamicsWorld(mDispatcher, mBroadphase, mSolver, mCollisionConfiguration);

	// Set the gravity for the simulation
	mDynamicsWorld->setGravity(btVector3(0, -10, 0));
}

void PhysicsEngine::update(float deltaTime)
{
	// Simulate dynamics
	mDynamicsWorld->stepSimulation(deltaTime);
}

btRigidBody* PhysicsEngine::createRigidBody(float mass, const btTransform& startTransform, btCollisionShape* collisionShape, Ogre::SceneNode* node)
{
	// Check validity of collision shape
	btAssert((!collisionShape || collisionShape->getShapeType() != INVALID_SHAPE_PROXYTYPE));

	// Push the collision shape into the collision shapes array
	mCollisionShapes.push_back(collisionShape);

	// Rigidbody is dynamic if and only if mass is non zero, otherwise it's static
	bool isDynamic = (mass != 0.f);

	// Calculate moment of inertia
	btVector3 inertia(0, 0, 0);
	if(isDynamic)
		collisionShape->calculateLocalInertia(mass, inertia);

	// Create rigid body
	btMotionState* motionState = new MyMotionState(startTransform, node);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, collisionShape, inertia);
	btRigidBody* rigidBody = new btRigidBody(rbInfo);

	// Add rigid body to dynamic simulation
	mDynamicsWorld->addRigidBody(rigidBody);

	return rigidBody;
}

void PhysicsEngine::shootRay(const btVector3 &rayFromWorld, const btVector3 &rayToWorld, const btVector3& impulse)
{
	// Create a closest ray result callback
	btCollisionWorld::ClosestRayResultCallback rayCallback(rayFromWorld, rayToWorld);

	// Shoot a ray given a start and end position and fills in the ray callback
	mDynamicsWorld->rayTest(rayFromWorld, rayToWorld, rayCallback);
	
	// Check whether the ray hit anything
	if(rayCallback.hasHit())
	{
		// Get the object that the ray hit
		btRigidBody* body = const_cast<btRigidBody*>(btRigidBody::upcast(rayCallback.m_collisionObject));

		// If the rigid body exists
		if(body)
		{
			// Activate the object dynamics
			body->setActivationState(ACTIVE_TAG);

			// Compute the direction to apply the impulse
			btVector3 relPos = rayCallback.m_hitPointWorld - body->getCenterOfMassPosition();

			// Apply the impulse
			body->applyImpulse(impulse, relPos);
		}
	}
}

void PhysicsEngine::destroyRigidBody(btRigidBody* rigidBody)
{
	// Get the collision shape
	btCollisionShape* shape = rigidBody->getCollisionShape();

	// Remove the collision shape from the list of collision objects
	mCollisionShapes.remove(shape);

	// Remove the rigid body from collision and rigidbody computations
	mDynamicsWorld->removeCollisionObject(rigidBody);
	mDynamicsWorld->removeRigidBody(rigidBody);

	// Delete the objects
	delete shape;
	delete static_cast<MyMotionState*>(rigidBody->getMotionState());
	delete rigidBody;
}
