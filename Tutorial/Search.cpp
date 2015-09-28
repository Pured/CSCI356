#include"DemoApp.h"

void DemoApp::tankMovement()
{	
	if(tankPath.size()!=0)
	{
		Ogre::Vector3 current=tankNode->_getDerivedPosition();
		Ogre::Vector3 goal;
		if(currentNode+1!=tankPath.size())
		{
			if(currentNode+1!=tankPath.size());
				goal=pathFindingGraph->getPosition(tankPath.at(currentNode+1));
			goal.y=0.5;
			tankNode->lookAt(goal,Ogre::Node::TS_WORLD, Ogre::Vector3::UNIT_X);
			if(WithinBounds(current,goal))
			{
					currentNode++;

			}
			tankNode->translate(tankNode->_getDerivedOrientation().xAxis() * 0.1);
		}
		else
		{
			// reset
			mCurrentState = 0;
			path1->clear();
			path2->clear();
			tankPath.clear();
			currentNode=-1;
		}

		tB->appendText("CURRENT NODE: "+std::to_string(currentNode)+"\n");
		tB->appendText("CURRENT POS: "+std::to_string(current.x)+","+std::to_string(current.y)+","+std::to_string(current.z)+"\n");
		tB->appendText("GOAL POS: "+std::to_string(goal.x)+","+std::to_string(goal.y)+","+std::to_string(goal.z)+"\n");
		tB->appendText("SIZE: "+std::to_string(tankPath.size()));
	}
}