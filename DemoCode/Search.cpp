#include"stdafx.h"
#include"DemoApp.h"

void DemoApp::tankMovement()
{
	for(int i=0; i<TANK_LIMIT;i++)
	{
		if(tankPath[i].size()!=0)
		{
			Ogre::Vector3 current=tankNode[i]->_getDerivedPosition();
			Ogre::Vector3 goal;
			if(currentNode[i]+1!=tankPath[i].size())
			{
				if(currentNode[i]+1!=tankPath[i].size());
					goal=pathFindingGraph->getPosition(tankPath[i].at(currentNode[i]+1));
				goal.y=0.5;
				tankNode[i]->lookAt(goal,Ogre::Node::TS_WORLD, Ogre::Vector3::UNIT_X);
				if(WithinBounds(current,goal))
				{
						currentNode[i]++;

				}
				tankNode[i]->translate(tankNode[i]->_getDerivedOrientation().xAxis() * 0.1);
			}
			else
			{
				// reset
				mCurrentState[i] = 0;
				//path1->clear();
				path2[i]->clear();
				tankPath[i].clear();
				currentNode[i]=-1;
			}

			//tB->appendText("CURRENT NODE: "+std::to_string(currentNode)+"\n");
			//tB->appendText("CURRENT POS: "+std::to_string(current.x)+","+std::to_string(current.y)+","+std::to_string(current.z)+"\n");
			//tB->appendText("GOAL POS: "+std::to_string(goal.x)+","+std::to_string(goal.y)+","+std::to_string(goal.z)+"\n");
			//tB->appendText("SIZE: "+std::to_string(tankPath.size()));
		}
	}
}