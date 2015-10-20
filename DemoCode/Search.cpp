#include"stdafx.h"
#include"DemoApp.h"

void DemoApp::tankMovement(const Ogre::FrameEvent& evt)
{
	for(int i=0; i<TANK_LIMIT;i++)
	{
		if(tanks.at(i).tankPath.size()!=0)
		{
			Ogre::Vector3 current=tanks.at(i).tankNode->_getDerivedPosition();
			Ogre::Vector3 goal;
	
			if(tanks.at(i).currentNode+1!=tanks.at(i).tankPath.size())
			{
				//printf("TANK %i : next node %i \n",i, pathFindingGraph->getContent(tanks.at(i).currentNode+1));
				if( pathFindingGraph->getContent(tanks.at(i).tankPath.at(tanks.at(i).currentNode+1))!=2)
				{
					
					if(tanks.at(i).currentNode+1!=tanks.at(i).tankPath.size());
						goal=pathFindingGraph->getPosition(tanks.at(i).tankPath.at(tanks.at(i).currentNode+1));
					goal.y=0.5;

					//printf("TANK YAW %i : GOAL YAW %i \n",(int)tanks.at(i).tankNode->getOrientation().getYaw().valueDegrees(), (int)tanks.at(i).orientDest.getYaw().valueDegrees());
					if(WithinRotationBounds(tanks.at(i)))
					{
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
					
					}

					if(WithinBounds(current,goal))
					{
							pathFindingGraph->setContent(tanks.at(i).tankPath.at(tanks.at(i).currentNode),0);
							pathFindingGraph->setContent(tanks.at(i).tankPath.at(tanks.at(i).currentNode+1),2);
							tanks.at(i).currentNode++;
							tanks.at(i).mRotProgress=0;
							tanks.at(i).firstTime=true;
							
							/*for(int j=0;j<1024;j++)
							{
								printf("%i,",pathFindingGraph->getContent(j));
								if((j+1)%32==0 && j!=0)
									printf("\n");
							}
							printf("\n");
							printf("\n");*/
					}
				}
			}
			else
			{
				//pathFindingGraph->setContent(pathFindingGraph->getNode(tanks.at(i).tankNode->getPosition()),2);
				resetPath(i);
			}
		}
	}
}
bool DemoApp::WithinRotationBounds(tank one)
{
	if(one.tankNode->getOrientation().getYaw().valueDegrees()>one.orientDest.getYaw().valueDegrees()-2 && one.tankNode->getOrientation().getYaw().valueDegrees()<one.orientDest.getYaw().valueDegrees()+2)
		return true;
	else return false;
}
void DemoApp::findPath(int i)
{
	// Try to find a new path
	//Resets everything to default state
	std::vector<int> path;
	tanks.at(i).path2->clear();
	tanks.at(i).mCurrentState = 0;
	tanks.at(i).tankPath.clear();
	tanks.at(i).currentNode=-1;
	pathFindingGraph->setContent(pathFindingGraph->getNode(tanks.at(i).tankNode->getPosition()),0);
	// if path exists
	if(mPathFinder.AStar(tanks.at(i).startNode, tanks.at(i).goalNode, *pathFindingGraph, path))
	{
		createPath(tanks.at(i).path2, 1.0, path, Ogre::ColourValue(0, 0, 1));

		// set state to path found
		tanks.at(i).mCurrentState++;
		for (std::vector<int>::iterator it = path.begin(); it!=path.end(); ++it)
		{
			tanks.at(i).tankPath.push_back(*it);
		}
		tanks.at(i).currentNode=0;
		tanks.at(i).firstTime=true;
	}
	else
	{
		tB->appendText("COULD NOT FIND PATH \n");
		pathFindingGraph->setContent(pathFindingGraph->getNode(tanks.at(i).tankNode->getPosition()),2);
		// no path so set state to no start node
		tanks.at(i).mCurrentState = 0;
	}
}
void DemoApp::resetPath(int i)
{
	tanks.at(i).path2->clear();
	tanks.at(i).mCurrentState = 0;
	tanks.at(i).tankPath.clear();

	
	tanks.at(i).currentNode=-1;
	tanks.at(i).firstTime=true;	
}
void DemoApp::createTank(int i)
{
		tank tmp;
		
		tmp.path2 = mSceneMgr->createManualObject("AStarPath"+std::to_string(i));
		tmp.path2->clear();
		mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(tmp.path2);
		if(i<TANK_LIMIT/2)
		{
			tmp.team=1;
		}
		else
		{
			tmp.team=2;
		}
		/*Tank Stuff*/
		std::string entityName = "Tank"+std::to_string(i);

		// Create entity
		Ogre::Entity* tankEntity = mSceneMgr->createEntity(entityName, "fish.mesh");

		// Attach entity to scene node
		tmp.tankNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
		Ogre::SceneNode * rePos=tmp.tankNode->createChildSceneNode();
		rePos->yaw(Ogre::Degree(180));
		rePos->attachObject(tankEntity);


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
		tmp.mSelectionCircle->setDefaultDimensions(10, 10);
		tmp.mSelectionCircle->setMaterialName("myMaterial/SelectionCircle");
		tmp.mSelectionCircle->setBillboardType(Ogre::BillboardType::BBT_PERPENDICULAR_COMMON);
		tmp.mSelectionCircle->setCommonDirection(Ogre::Vector3(0, 1, 0));
		tmp.mSelectionCircle->setCommonUpVector(Ogre::Vector3(0, 0, -1));

		// Create a billboard for the selection circle BillboardSet
		mSelectionCircleBB = tmp.mSelectionCircle->createBillboard(Ogre::Vector3(0, 0.05, 0));
		mSelectionCircleBB->setTexcoordRect(0.0, 0.0, 1.0, 1.0);

		tmp.originalRoll=tmp.tankNode->getOrientation().getRoll();
		tanks.push_back(tmp);
}
void DemoApp::respawnTank(int i)
{
		// Place object at appropriate position
		
		Ogre::Vector3 position;
		int node=0;
		do
		{
			
			if(tanks.at(i).team==2)
			{
				//printf("%i team 2 \n",i);
				node=((rand()%40 + 1)*42)+40;//(32-(rand() % 3+1));
				position=pathFindingGraph->getPosition(node);
			}
			else
			{
				//printf("%i team 1 \n",i);
				node=((rand()%40+1)*42)+1;//+((rand() % 3));
				position=pathFindingGraph->getPosition(node);
			}
			position.y = 0.7;
		}while(pathFindingGraph->getContent(node)==2);

		if(tanks.at(i).team==2)
			tanks.at(i).tankNode->yaw(Ogre::Degree(180));

		pathFindingGraph->setContent(node,2);
		tanks.at(i).tankNode->setPosition(position);	
		tanks.at(i).health=100;
}