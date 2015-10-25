#include "stdafx.h"
#include "DemoApp.h"
//-------------------------------------------------------------------------------------
DemoApp::DemoApp(void)
{
	camX = 0;
	camY = 0;
	camZ = 45;
	cMode = 0;
	lookAtDest = (0, 0, 0);
	srand(time(NULL));
	selectionMode = 1;
	boxTimeout = 0;
	controlPressed = false;
	mousePressedVar = false;
	nextClickPath = false;

	playerControlled = -1; //-1 = AI, 0+ = tankID
	prevPlayerControlled = -1;

	tankInfoWasOpen = true;
	controlsWasOpen = true;
	chatWasOpen = false;

	visibleCollectibles = NUMCOLLECTIBLES;

	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
}
//-------------------------------------------------------------------------------------
DemoApp::~DemoApp(void)
{
	if (pathFindingGraph)
		delete pathFindingGraph;
}
//-------------------------------------------------------------------------------------
bool DemoApp::setup(void)
{
	// Initialise the mPhysicsEngine
	mPhysicsEngine = new PhysicsEngine();
	mPhysicsEngine->initPhysics();

	BaseApplication::setup();

	//set initial mouse position
	OIS::MouseState &mutableMouseState = const_cast<OIS::MouseState &>(mMouse->getMouseState());
	mutableMouseState.X.abs = mCamera->getViewport()->getActualWidth() / 2;
	mutableMouseState.Y.abs = mCamera->getViewport()->getActualHeight() / 2;

	mTrayMgr->showCursor();

	pathFindingGraph = new Graph;

	mDirection = Ogre::Vector3::ZERO;

	mTotalShots = 0;

	return true;
};
//-------------------------------------------------------------------------------------
void DemoApp::createScene(void)
{
	mCamera->setOrientation(Ogre::Quaternion());
	camNode=mSceneMgr->getRootSceneNode()->createChildSceneNode("Camera");
	camNode->setOrientation(Ogre::Quaternion());
	camNode->setPosition(Ogre::Vector3(0, 10, -10));
	camNode->lookAt(Ogre::Vector3(0, 10, 0),Ogre::Node::TS_PARENT,Ogre::Vector3::UNIT_Z);

	camNode->attachObject(mCamera);

	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.25, 0.25, 0.25));
 
    // create the light
    Ogre::Light *light = mSceneMgr->createLight("Light1");
    light->setType(Ogre::Light::LT_POINT);
    light->setPosition(Ogre::Vector3(250, 150, 250));
    light->setDiffuseColour(Ogre::ColourValue::White);
    light->setSpecularColour(Ogre::ColourValue::White);
 
	mSceneMgr->setSkyDome(true, "Examples/CloudySky", 5, 8); //sky dome

	mapSetup();

	mWindow->setFullscreen(false,1280,720);
	mWindow->reposition(150, 50);
}
//-------------------------------------------------------------------------------------
bool DemoApp::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    if(mWindow->isClosed())
        return false;
 
	if(mShutDown)
		return false;
 
	//need to capture/update each device
	mKeyboard->capture();
	mMouse->capture();
	tankMovement(evt);
	frameRenderingCamera();
	playerControlsQueued();

	// Delete the shots once they go more than MAX_SHOTS
	for(int i = 0; i < MAX_SHOTS; i++)
	{
		// If there's an existing shot
		if(mProjectiles[i] != NULL)
		{
			if(mTotalShots >= MAX_SHOTS)
			{
				// Delete all shots in the array
				for (int j = 0; j < MAX_SHOTS; j++)
				{
					clearShots(static_cast<btRigidBody*>(mProjectiles[j]));
					mProjectiles[j] = NULL;
					mTotalShots--;
				}
			}
		}
	}

	for(int i = 0; i < tanks.size(); i++) //loop through all tanks
		if(i != playerControlled) //if the tank isn't player-controlled
			think(tanks.at(i)); //run AI for tank

	mTrayMgr->frameRenderingQueued(evt);

	return true;
}
//-------------------------------------------------------------------------------------
 void DemoApp::createFrameListener(void)
{
	BaseApplication::createFrameListener();

	createUI();
}
 //-------------------------------------------------------------------------------------
// OIS::KeyListener
bool DemoApp::keyPressed( const OIS::KeyEvent &arg )
{
	keyInput(arg);

	return true;
}
//-------------------------------------------------------------------------------------
bool DemoApp::keyReleased( const OIS::KeyEvent &arg )
{
	BaseApplication::keyReleased(arg);

	switch (arg.key)
	{
		case OIS::KC_LMENU: 
			cMode = 0;
			lookAtDest = (0, 0, 0);
			mTrayMgr->showCursor();
		break;

		case::OIS::KC_LCONTROL:
			controlPressed = false;
		break;
	}
	return true;
}
//-------------------------------------------------------------------------------------
// OIS::MouseListener
bool DemoApp::mouseMoved( const OIS::MouseEvent &arg )
{
	if (mTrayMgr->injectMouseMove(arg)) return true;

	camMovement(arg);

	return true;
}
//-------------------------------------------------------------------------------------
bool DemoApp::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	if (mTrayMgr->injectMouseDown(arg, id)) return true;

	camPressed(arg,id);

	return true;
}
//-------------------------------------------------------------------------------------
bool DemoApp::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	if (mTrayMgr->injectMouseDown(arg, id)) return true;

	switch (id)
	{
		case OIS::MB_Left:

			// shoot();

		break;
	}

	camMouseReleased(arg,id);

	return true;
}
//-------------------------------------------------------------------------------------
void DemoApp::createPath(Ogre::ManualObject* line, float height, std::vector<int>& path, Ogre::ColourValue& colour)
{
	line->clear();

	// Specify the material and rendering type
	line->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_STRIP);

	// Specify the vertices and vertex colour for the line
	Ogre::Vector3 position;

	for(std::vector<int>::iterator it=path.begin(); it!=path.end(); it++)
	{
		position = pathFindingGraph->getPosition(*it);
		line->position(Ogre::Vector3(position.x, height, position.z));
		line->colour(colour);
	}

	// Finished defining line
	line->end();
}
//-------------------------------------------------------------------------------------
void DemoApp::shoot()
{
	// The btVector3 'shotOrgin' is assumed to be the location of the tank shooting the gun

	int i = 0;

	// Move 'i' to the latest shot count
	for (i = 0; i < MAX_SHOTS; i++)
		if (mProjectiles[i] == NULL)
			break;

	if (i < MAX_SHOTS)
	{			
		// Calculate the direction for the linear velocity
		btVector3 linearVelocity(shotOrigin.x(), shotOrigin.y(), shotOrigin.z());
				
		// Convert vector3 to a float
		linearVelocity.normalize();

		// Scale to appropriate velocity multiplied with the charge
		linearVelocity *= SHOT_VELOCITY;

		char name[8];
		sprintf(name, "box%d", i);

		// Create and shoot the box
		mProjectiles[i] = projectile(shotOrigin, btQuaternion(0, 0, 0, 1), linearVelocity, name);
		mTotalShots++;
	}
}
//-------------------------------------------------------------------------------------
btCollisionObject* DemoApp::projectile(const btVector3& position, const btQuaternion& orientation, 
								   const btVector3& linearVelocity, Ogre::String entityName)
{
	// 'position' is the location in which the ball begins it's journey
	// 'orientation' is idealy 'btQuaternion(0, 0, 0, 1); its for the rotation value of the object
	// 'linearVelocity' is the velocity of the balls. Will be a global variable
	// 'entityName' is the name of the entity; to ensure it doesn't crash the program

	// Create the projectile entity
	Ogre::Entity* projectile;
	projectile = mSceneMgr->createEntity(entityName, "penguin.mesh");

	// Attach the object into the world
	Ogre::SceneNode* node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	node->attachObject(projectile);
	node->scale(0.5, 0.5, 0.5);

	// Create a collision shape
	btCollisionShape* collisionShape = new btSphereShape(btScalar(1));

	// The object's starting transformation
	btTransform startingTrans;
	startingTrans.setIdentity();
	startingTrans.setOrigin(position);
	startingTrans.setRotation(orientation);

	// Create the rigid body
	btRigidBody* rigidBody = mPhysicsEngine->createRigidBody(1.0f, startingTrans, collisionShape, node);

	// Give the rigid body an initial velocity
	rigidBody->setLinearVelocity(linearVelocity);

	return rigidBody;
}
//-------------------------------------------------------------------------------------
void DemoApp::clearShots(btRigidBody* shot)
{
	// Get the scene node from the motion state of the rigidbody
	Ogre::SceneNode* node = static_cast<MyMotionState*>(shot->getMotionState())->getNode();

	// Get the entity from the node 
	// Assumes that you only have one entity, if you have more just loop through to get them all in order to delete them
	Ogre::Entity* entity = static_cast<Ogre::Entity*>(node->getAttachedObject(0));

	// Detach the entity from the scene node
	node->detachObject(entity);

	// Delete the entity and the scene node
	mSceneMgr->destroyEntity(entity);
	mSceneMgr->destroySceneNode(node);

	// Destroy the rigidbody from the physics system
	mPhysicsEngine->destroyRigidBody(shot);
}
//-------------------------------------------------------------------------------------
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif
 
#ifdef __cplusplus
extern "C" {
#endif
 
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
    int main(int argc, char *argv[])
#endif
    {
        // Create application object
        DemoApp app;
 
        try {
            app.go();
        } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occured: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }
 
        return 0;
    }
 
#ifdef __cplusplus
}
#endif