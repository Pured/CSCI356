#include"stdafx.h"
#include "DemoApp.h"

void DemoApp::mapSetup()
{
	// Create a selection box object
	mSelectionBox = createSelectionBox("Box");

	// Add object to scene manager
	mSceneMgr->getRootSceneNode()->attachObject(mSelectionBox);

	mSelecting = false;

	Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
	Ogre::MeshManager::getSingleton().createPlane("ground", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane, 
		160, 160, 20, 20, true, 1, 2.5, 2.5, Ogre::Vector3::UNIT_Z);

	Ogre::Entity* entGround = mSceneMgr->createEntity("GroundEntity", "ground");
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entGround);
	entGround->setMaterialName("Ground");
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
			myNode->scale(0.1, 0.01, 0.1);
			
			// Place object at appropriate position
			Ogre::Vector3 position = pathFindingGraph->getPosition(nodeNumber);
			position.y = 0.5;
			myNode->translate(position);
		}
	}

	// create the path objects, and clear them to start off
	//path1 = mSceneMgr->createManualObject("DijkstraPath");
	//path1->clear();
	//mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(path1);
	for(int i=0;i<TANK_LIMIT;i++)
	{
		path2[i] = mSceneMgr->createManualObject("AStarPath"+std::to_string(i));
		path2[i]->clear();
		mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(path2[i]);
	}
	for(int i=0; i<TANK_LIMIT; i++)
	{
		/*Tank Stuff*/
		std::string entityName = "Tank"+std::to_string(i);

		// Create entity
		Ogre::Entity* fish = mSceneMgr->createEntity(entityName, "fish.mesh");
		//cube->setMaterialName("Examples/BumpyMetal");

		// Attach entity to scene node
		tankNode[i] = mSceneMgr->getRootSceneNode()->createChildSceneNode();
		Ogre::SceneNode * rePos=tankNode[i]->createChildSceneNode();
		rePos->yaw(Ogre::Degree(180));
		rePos->attachObject(fish);
		//tankNode[i]->scale(0.1, 0.01, 0.1);
			
		// Place object at appropriate position
		Ogre::Vector3 position = pathFindingGraph->getPosition((i*16));
		position.y = 0.7;
		tankNode[i]->translate(position);
		
	}
	for(int i=0;i<TANK_LIMIT;i++)
	{
		// Create a BillboardSet to represent a health bar and set its properties
		mHealthBar[i] = mSceneMgr->createBillboardSet("Healthbar1"+std::to_string(i));
		mHealthBar[i]->setCastShadows(false);
		mHealthBar[i]->setDefaultDimensions(5, 1);
		mHealthBar[i]->setMaterialName("myMaterial/HealthBar");

		// Create a billboard for the health bar BillboardSet
		mHealthBarBB = mHealthBar[i]->createBillboard(Ogre::Vector3(0, 5, 0));

		//tankNode[i]->attachObject(mHealthBar[i]);

		// Create a BillboardSet for a selection circle and set its properties
		mSelectionCircle[i] = mSceneMgr->createBillboardSet("SelectionCircle"+std::to_string(i));
		mSelectionCircle[i]->setCastShadows(false);
		mSelectionCircle[i]->setDefaultDimensions(10, 10);
		mSelectionCircle[i]->setMaterialName("myMaterial/SelectionCircle");
		mSelectionCircle[i]->setBillboardType(Ogre::BillboardType::BBT_PERPENDICULAR_COMMON);
		mSelectionCircle[i]->setCommonDirection(Ogre::Vector3(0, 1, 0));
		mSelectionCircle[i]->setCommonUpVector(Ogre::Vector3(0, 0, -1));

		// Create a billboard for the selection circle BillboardSet
		mSelectionCircleBB = mSelectionCircle[i]->createBillboard(Ogre::Vector3(0, 0.05, 0));
		mSelectionCircleBB->setTexcoordRect(0.0, 0.0, 1.0, 1.0);

		//tankNode[i]->attachObject(mSelectionCircle[i]);
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