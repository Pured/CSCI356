#include"stdafx.h"
#include "DemoApp.h"

void DemoApp::mapSetup()
{
	srand (time(NULL));

	TANK_LIMIT=4;
	// Create a selection box object
	mSelectionBox = createSelectionBox("Box");

	// Add object to scene manager
	mSceneMgr->getRootSceneNode()->attachObject(mSelectionBox);

	mSelecting = false;

	Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
	Ogre::MeshManager::getSingleton().createPlane("ground", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane, 
		320, 320, 20, 20, true, 1, 2.5, 2.5, Ogre::Vector3::UNIT_Z);

	Ogre::Entity* entGround = mSceneMgr->createEntity("GroundEntity", "ground");
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entGround);
	entGround->setMaterialName("Examples/GrassFloor");
	entGround->setCastShadows(false);


	// go through the graph
	// if a node is blocked, display a cube on that grid location
	for(int nodeNumber=0; nodeNumber<TOTAL_NODES; nodeNumber++)
	{
		int contents = pathFindingGraph->getContent(nodeNumber);

		if(contents)
		{
			// Create unique name
			std::ostringstream oss;
			oss << nodeNumber;
			std::string entityName = "Cube" + oss.str();

			// Create entity
			Ogre::Entity* cube = mSceneMgr->createEntity(entityName, "cube.mesh");
			cube->setMaterialName("Examples/BumpyMetal");

			// Attach entity to scene node
			Ogre::SceneNode* myNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
			myNode->attachObject(cube);
			myNode->scale(0.1, 0.1, 0.1);
			
			// Place object at appropriate position
			Ogre::Vector3 position = pathFindingGraph->getPosition(nodeNumber);
			position.y = 5;
			myNode->translate(position);
		}
	}
	for(int i=0;i<TANK_LIMIT;i++)
	{
		//Creates a new tank
		createTank(i);
		//Spawns the tank in the map based on team
		respawnTank(i);
	}
}
Ogre::ManualObject* DemoApp::createSelectionBox(Ogre::String name)
{
	// Create manual object
	Ogre::ManualObject* box2D = mSceneMgr->createManualObject(name);

	// Indicate that object is likely to be updated
	box2D->setDynamic(true);
	box2D->estimateVertexCount(5);

	// Set in render queue without depth checking
	box2D->setRenderQueueGroup(Ogre::RENDER_QUEUE_OVERLAY);

	// Specify the material and rendering type
	box2D->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_STRIP);
	
	// Points of the line strip
	box2D->position(0, 0, 0);
	box2D->position(0, 0, 0);
	box2D->position(0, 0, 0);
	box2D->position(0, 0, 0);
	box2D->position(0, 0, 0);

	// Finished defining the 2D line strip
	box2D->end();

	return box2D;
}

Ogre::Vector2 DemoApp::worldToScreenPosition(const Ogre::Vector3& position)
{
	// The origin of the screen position here is at the centre of screen
	Ogre::Vector3 screenPosition = mCamera->getProjectionMatrix() * mCamera->getViewMatrix() * position;

	// Compute the 2D screen position with the origin at the top left of screen
	return Ogre::Vector2(0.5 + 0.5 * screenPosition.x, 0.5 - 0.5 *screenPosition.y);
}