#include "stdafx.h"
#include "DemoApp.h"

DemoApp::Collectible::Collectible(int index, int type, Ogre::SceneManager *mSceneMgr, Graph *pathFindingGraph)
{
	collectibleItem = mSceneMgr->createEntity("CollectibleItem" + std::to_string(index), "uv_sphere.mesh");

	if(type == 3) //hp ball
		collectibleItem->setMaterialName("HPBALL");
	else //trophy ball
		collectibleItem->setMaterialName("Examples/Hilite/Yellow");

	collectibleNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	collectibleNode->attachObject(collectibleItem);
	collectibleNode->scale(0.02, 0.02, 0.02);
	
	if(index == 0)
		position = pathFindingGraph->getPosition(193);
	else if(index == 1)
		position = pathFindingGraph->getPosition(861);
	else if(index == 2)
		position = pathFindingGraph->getPosition(1570);

	position.y = 5;
	collectibleNode->translate(position);

	isVisible = true;
	timeTillSpawn = 0;
	collectibleType = type;
}

void DemoApp::collectibleCollected(Collectible collectible)
{
	if(collectible.collectibleType == 3) //hp respawn timer
		collectible.timeTillSpawn = 60;
	else if(collectible.collectibleType == 4) //trophy respawn timer
		collectible.timeTillSpawn = 30;

	collectible.isVisible = false;
}

void DemoApp::spawnCollectible(Collectible collectible)
{
	collectible.isVisible = true;
}