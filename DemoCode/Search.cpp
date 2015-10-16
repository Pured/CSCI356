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
				if(tanks.at(i).currentNode+1!=tanks.at(i).tankPath.size());
					goal=pathFindingGraph->getPosition(tanks.at(i).tankPath.at(tanks.at(i).currentNode+1));
				goal.y=0.5;

				if(tanks.at(i).tankNode->getOrientation()== tanks.at(i).orientDest && tanks.at(i).firstTime==false )
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
					tanks.at(i).tankNode->translate(tanks.at(i).tankNode->getOrientation().xAxis() * 0.1);
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
					tanks.at(i).mRotProgress+=1.0f;
					Ogre::Quaternion delta = Ogre::Quaternion::nlerp(tanks.at(i).mRotProgress*evt.timeSinceLastFrame, tanks.at(i).tankNode->getOrientation(), tanks.at(i).orientDest, true);
					tanks.at(i).tankNode->setOrientation(delta);
					
				}

				if(WithinBounds(current,goal))
				{
						tanks.at(i).currentNode++;
						tanks.at(i).mRotProgress=0;
						tanks.at(i).firstTime=true;
				}
			}
			else
			{
				resetPath(i);
			}
		}
	}
}
void DemoApp::findPath(int i)
{
	// try to find path from start to goal node
	std::vector<int> path;
	tanks.at(i).path2->clear();
	tanks.at(i).mCurrentState = 0;
	tanks.at(i).tankPath.clear();
	tanks.at(i).currentNode=-1;
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


		tanks.push_back(tmp);
}
void DemoApp::respawnTank(int i)
{
		// Place object at appropriate position
		
		Ogre::Vector3 position;
		if(tanks.at(i).team==2)
			position=pathFindingGraph->getPosition(((rand()%32)*32)+(32-(rand() % 3)));
		else
			position=pathFindingGraph->getPosition(((rand()%32)*32)+((rand() % 3)));
		position.y = 0.7;
		tanks.at(i).tankNode->setPosition(position);	
		tanks.at(i).health=100;
}