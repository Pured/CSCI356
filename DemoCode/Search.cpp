#include"stdafx.h"
#include"DemoApp.h"

void DemoApp::tankMovement(const Ogre::FrameEvent& evt)
{
	for(int i = 0; i < TANK_LIMIT; i++)
	{
		if(tanks.at(i).tankPath.size()!=0)
		{
			Ogre::Vector3 current = tanks.at(i).tankNode->_getDerivedPosition();
			Ogre::Vector3 goal;
	
			if(tanks.at(i).currentNode+2 != tanks.at(i).tankPath.size())
			{
				//printf("TANK %i : next node %i \n",i, pathFindingGraph->getContent(tanks.at(i).currentNode+1));
				//printf("tank moving before wait %i \n", i);
				//printf("Current Node: %i \n", tanks.at(i).currentNode + 1);
				
				/*if (pathFindingGraph->getContent(tanks.at(i).tankPath.at(tanks.at(i).currentNode + 1)) != 2)
				{
					printf("%i , %i\n", pathFindingGraph->getContent(tanks.at(i).tankPath.at(tanks.at(i).currentNode+1)), i);
					//printf("Found = 2 \n");

					//printf("%i \n", pathFindingGraph->getContent(tanks.at(i).tankPath.at(tanks.at(i).currentNode)));
					once = true;
				}*/
				//if( pathFindingGraph->getContent(tanks.at(i).tankPath.at(tanks.at(i).currentNode+1))!=2)
				{
					
					
					if(tanks.at(i).currentNode+1!=tanks.at(i).tankPath.size());
						goal=pathFindingGraph->getPosition(tanks.at(i).tankPath.at(tanks.at(i).currentNode+1));

					goal.y = 0.5;

					//printf("TANK YAW %i : GOAL YAW %i \n",(int)tanks.at(i).tankNode->getOrientation().getYaw().valueDegrees(), (int)tanks.at(i).orientDest.getYaw().valueDegrees());
					printf("PATH SIZE : %i CURRENT : %i \n",tanks.at(i).tankPath.size(), tanks.at(i).currentNode);
					if(WithinRotationBounds(tanks.at(i)))
					{
						//printf("TANKS NAME %i \n", tanks.at(i).name);
						//printf("TANKS ORIRENT DEST %i \n", tanks.at(i).orientDest);
						if(i<tanks.size())
							tanks.at(i).tankNode->setOrientation(tanks.at(i).orientDest);
					}
					/*if(WithinRotationBounds(tanks.at(i)) && tanks.at(i).firstTime==true)
						printf("VALID BOUNDS AND FIRST TIME \n");
					else if(WithinRotationBounds(tanks.at(i)) && tanks.at(i).firstTime==false)
						printf("VALID BOUNDS AND NOT FIRST TIME \n");*/
					if(WithinRotationBounds(tanks.at(i)) && tanks.at(i).firstTime==false )
					{
						//printf("FINISHED TANK YAW %f : GOAL YAW %f \n",tanks.at(i).tankNode->getOrientation().getYaw(), tanks.at(i).orientDest.getYaw());
						if(tanks.at(i).firstTime==true)
						{
							tanks.at(i).firstTime=false;
							tanks.at(i).mRotProgress=0;
							Ogre::Quaternion orig=tanks.at(i).tankNode->_getDerivedOrientation();
							tanks.at(i).tankNode->lookAt(goal,Ogre::Node::TS_WORLD, Ogre::Vector3::UNIT_X);
							tanks.at(i).orientDest=tanks.at(i).tankNode->_getDerivedOrientation();
							tanks.at(i).tankNode->setOrientation(orig);
						}

						tanks.at(i).tankNode->translate(tanks.at(i).tankNode->getOrientation().xAxis() * 0.2);
						//printf("Moving \n");
						//printf("Moving \n");
					}
					else
					{
						if(tanks.at(i).firstTime==true)
						{
						
							tanks.at(i).firstTime=false;
							tanks.at(i).mRotProgress=0;
							Ogre::Quaternion orig=tanks.at(i).tankNode->_getDerivedOrientation();
							tanks.at(i).tankNode->lookAt(goal,Ogre::Node::TS_WORLD, Ogre::Vector3::UNIT_X);
							tanks.at(i).orientDest=tanks.at(i).tankNode->_getDerivedOrientation();
							tanks.at(i).tankNode->setOrientation(orig);
						}
						//printf("Rotating \n");
						tanks.at(i).mRotProgress+=2.0f;
						Ogre::Quaternion delta = Ogre::Quaternion::nlerp(tanks.at(i).mRotProgress*evt.timeSinceLastFrame, tanks.at(i).tankNode->getOrientation(), tanks.at(i).orientDest, true);
						tanks.at(i).tankNode->setOrientation(delta);
						tanks.at(i).tankNode->roll(tanks.at(i).originalRoll);

						//printf("rotating \n");
					}

					if(WithinBounds(current,goal))
					{
							pathFindingGraph->setContent(tanks.at(i).tankPath.at(tanks.at(i).currentNode),0);
							
							tanks.at(i).currentNode++;
							pathFindingGraph->setContent(tanks.at(i).tankPath.at(tanks.at(i).currentNode + 1), 2);
							tanks.at(i).mRotProgress=0;
							tanks.at(i).firstTime=true;
							//printf("\n \n WITHIN BOUDS &i \n \n", i);
					}
					//printf("\n \ nPast withinBounds check \n \n");
				}
			}
			else
			{
				//pathFindingGraph->setContent(pathFindingGraph->getNode(tanks.at(i).tankNode->getPosition()), 2);
				printf("RESET PATH \n");
				resetPath(i);
			}
		}
	}
}

bool DemoApp::WithinRotationBounds(tank one)
{
	if(one.tankNode->getOrientation().getYaw().valueDegrees()>one.orientDest.getYaw().valueDegrees()-2 && one.tankNode->getOrientation().getYaw().valueDegrees()<one.orientDest.getYaw().valueDegrees()+2)
		return true;
	else
		return false;
}

void DemoApp::findPath(int i)
{
	//Try to find a new path
	//Resets everything to default state
	std::vector<int> path;

	tanks.at(i).path2->clear();
	tanks.at(i).mCurrentState = 0;
	tanks.at(i).tankPath.clear();
	tanks.at(i).currentNode=-1;

	pathFindingGraph->setContent(pathFindingGraph->getNode(tanks.at(i).tankNode->getPosition()),0);

	//if path exists
	if(mPathFinder.AStar(tanks.at(i).startNode, tanks.at(i).goalNode, *pathFindingGraph, path))
	{
		createPath(tanks.at(i).path2, 1.0, path, Ogre::ColourValue(0, 0, 1));

		// set state to path found
		tanks.at(i).mCurrentState++;

		for (std::vector<int>::iterator it = path.begin(); it!=path.end(); ++it)
		{
			tanks.at(i).tankPath.push_back(*it);
		}

		tanks.at(i).currentNode = 0;
		tanks.at(i).firstTime = true;
	}
	else
	{
		//printf("No Path \n");
		pathFindingGraph->setContent(pathFindingGraph->getNode(tanks.at(i).tankNode->getPosition()),2);
		//no path so set state to no start node
		tanks.at(i).mCurrentState = 0;
	}
}

void DemoApp::resetPath(int i)
{
	tanks.at(i).path2->clear();
	tanks.at(i).mCurrentState = 0;
	tanks.at(i).tankPath.clear();

	tanks.at(i).currentNode = -1;
	tanks.at(i).firstTime = true;	

	once = false;
}

void DemoApp::createTank(int i)
{
		tank tmp;
		tmp.name = nameList[namesAllocated];
		tmp.path2 = mSceneMgr->createManualObject("AStarPath"+ tmp.name);
		tmp.path2->clear();
		mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(tmp.path2);

		if(i % 2 == 0)
		{
			tmp.team = 1;
		}
		else
		{
			tmp.team = 2;
		}

		 //give the tank a name
		namesAllocated++; //point to the next name

		/*Tank Stuff*/
		std::string entityName = "Tank"+std::to_string(i);

		std::string mesh;
		std::string mat;
		double size;

		if(tmp.team == 1) //change mesh/material depending on team (challenger vs leopard)
		{
			mesh = "ch";
			mat = "ch_";
			size = 0.055;
		}
		else
		{
			mesh = "lp";
			mat = "lp_";
			size = 0.07;
		}

		// Create tank body entity
		Ogre::Entity* tankBody = mSceneMgr->createEntity(tmp.name, mesh+"body.mesh");
		tankBody->setMaterialName(mat+"tank_material");

		// Create tank turret entity
		Ogre::Entity* tankTurret = mSceneMgr->createEntity(tmp.name +"Turret", mesh+"turret.mesh");
		tankTurret->setMaterialName(mat+"tank_material");

		// Create tank barrel entity
		Ogre::Entity* tankBarrel = mSceneMgr->createEntity(tmp.name +"Barrel", mesh+"barrel.mesh");
		tankBarrel->setMaterialName(mat+"tank_material");

		// Attach tank body to scene node
		tmp.tankNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
		Ogre::SceneNode * rePos=tmp.tankNode->createChildSceneNode();
		rePos->yaw(Ogre::Degree(180));
		rePos->setScale(size, size, size);
		rePos->attachObject(tankBody);

		// Attach tank turret to tank body
		tmp.turretNode = rePos->createChildSceneNode();
		tmp.turretNode->attachObject(tankTurret);
		// Move it above tank body
		tmp.turretNode->translate(0, 3, 0);

		// Attach tank barrel to tank turret
		tmp.barrelNode = tmp.turretNode->createChildSceneNode();
		tmp.barrelNode->attachObject(tankBarrel);
		// Move it to the appropriate position on the turret
		tmp.barrelNode->translate(-30, 10, 0);

		// Create a BillboardSet to represent a health bar and set its properties
		tmp.mHealthBar = mSceneMgr->createBillboardSet("Healthbar1"+std::to_string(i));
		tmp.mHealthBar->setCastShadows(false);
		tmp.mHealthBar->setDefaultDimensions(5, 1);
		tmp.mHealthBar->setMaterialName("myMaterial/HealthBar");

		// Create a billboard for the health bar BillboardSet
		mHealthBarBB = tmp.mHealthBar->createBillboard(Ogre::Vector3(0, 5, 0));

		// Create a BillboardSet for a selection circle and set its properties
		tmp.mSelectionCircle = mSceneMgr->createBillboardSet("SelectionCircle"+std::to_string(i));
		tmp.mSelectionCircle->setCastShadows(false);
		tmp.mSelectionCircle->setDefaultDimensions(12, 10);
		tmp.mSelectionCircle->setMaterialName("myMaterial/SelectionCircle");
		tmp.mSelectionCircle->setBillboardType(Ogre::BillboardType::BBT_PERPENDICULAR_COMMON);
		tmp.mSelectionCircle->setCommonDirection(Ogre::Vector3(0, 1, 0));
		tmp.mSelectionCircle->setCommonUpVector(Ogre::Vector3(0, 0, -1));

		// Create a billboard for the selection circle BillboardSet
		mSelectionCircleBB = tmp.mSelectionCircle->createBillboard(Ogre::Vector3(0, 0.01, 0));
		mSelectionCircleBB->setTexcoordRect(0.0, 0.0, 1.0, 1.0);

		tmp.originalRoll=tmp.tankNode->getOrientation().getRoll();
		tanks.push_back(tmp);

}
void DemoApp::respawnTank(int i)
{
	//Place object at appropriate position
	Ogre::Vector3 position;
	int node = 0;

	do
	{	
		if(tanks.at(i).team == 2)
		{
			//printf("%i team 2 \n",i);
			node = ((rand() % 40 + 1)  *42) + 40; //(32-(rand() % 3+1));
			position=pathFindingGraph->getPosition(node);
		}
		else
		{
			//printf("%i team 1 \n",i);
			node = ((rand() % 40 + 1) * 42) + 1; //+((rand() % 3));
			position=pathFindingGraph->getPosition(node);
		}

		position.y = 0.7;
	}while(pathFindingGraph->getContent(node) == 2);

	if(tanks.at(i).team == 2)
		tanks.at(i).tankNode->yaw(Ogre::Degree(180));

	pathFindingGraph->setContent(node, 2);
	tanks.at(i).tankNode->setPosition(position);	
	tanks.at(i).health = 100;
}
void DemoApp::destroyTank(int i)
{
	printf("In destroy \n");
	tanks.at(i).barrelNode->detachAllObjects();
	tanks.at(i).tankNode->detachAllObjects();
	tanks.at(i).turretNode->detachAllObjects();
	//
	printf("After detach \n");
	mSceneMgr->destroyEntity(tanks.at(i).name + "Barrel");
	printf("After barrel \n");
	mSceneMgr->destroyEntity(tanks.at(i).name + "Turret");
	printf("After turret \n");
	mSceneMgr->destroyEntity(tanks.at(i).name);
	printf("After body \n");
	tanks.at(i).mHealthBar->detachFromParent();
	tanks.at(i).mSelectionCircle->detachFromParent();
	mSceneMgr->destroyManualObject(tanks.at(i).path2);
	mSceneMgr->destroyBillboardSet(tanks.at(i).mHealthBar);
	mSceneMgr->destroyBillboardSet(tanks.at(i).mSelectionCircle);
	tanks.erase(tanks.begin() + i);
}