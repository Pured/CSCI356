#include "DemoApp.h"

void DemoApp::mapSetup()
{
	Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
	Ogre::MeshManager::getSingleton().createPlane("ground", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane, 
		256, 256, 20, 20, true, 1, 2.5, 2.5, Ogre::Vector3::UNIT_Z);

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
	path1 = mSceneMgr->createManualObject("DijkstraPath");
	path1->clear();
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(path1);

	path2 = mSceneMgr->createManualObject("AStarPath");
	path2->clear();
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(path2);

	/*Tank Stuff*/
	std::string entityName = "Tank";

	// Create entity
	Ogre::Entity* cube = mSceneMgr->createEntity(entityName, "cube.mesh");
	//cube->setMaterialName("Examples/BumpyMetal");

	// Attach entity to scene node
	tankNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	tankNode->attachObject(cube);
	tankNode->scale(0.1, 0.01, 0.1);
			
	// Place object at appropriate position
	Ogre::Vector3 position = pathFindingGraph->getPosition(0);
	position.y = 0.7;
	tankNode->translate(position);
}