#include"stdafx.h"
#include"DemoApp.h"

void DemoApp::tankMovement(const Ogre::FrameEvent& evt)
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

				//Ogre::Vector3 src=tankNode[i]->getPosition();
				Ogre::Vector2 v1;
				v1.x=tankNode[i]->getPosition().x;
				v1.y=tankNode[i]->getPosition().z;

				Ogre::Quaternion q1=tankNode[i]->getOrientation();
				//tankNode[i]->lookAt(goal,Ogre::Node::TS_WORLD, Ogre::Vector3::UNIT_X);
				Ogre::Quaternion q2=tankNode[i]->_getDerivedOrientation();

				//Ogre::Quaternion mRotDest = src.getRotationTo(goal) * q1; 
				
				Ogre::Vector2 v2;
				v2.x=goal.x;
				v2.y=goal.z;
				//v2.x=goalNode[i]->getPosition().x;

				//bool mRotating=true;
				//double tanVal=atan2(goal.y - v1.y,goal.x - v1.x) * 180 / M_PI;
			

				Ogre::Radian yRad;
				Ogre::Radian pRad;
				Ogre::Radian rRad;

				Ogre::Matrix3 mat;
				q1.ToRotationMatrix(mat);
				mat.ToEulerAnglesYXZ(yRad, pRad, rRad);
				Ogre::Degree yDeg = yRad;
				Ogre::Degree pDeg = pRad;
				Ogre::Degree rDeg = rRad;

				
				float dot = v1.x*v2.x + v1.y*v2.y;//      # dot product
				float det = v1.x*v2.y - v1.y*v2.x;//      # determinant
				//int angle = (atan2(v1.x, v1.y) - atan2(v2.x, v2.y)) * (180/M_PI);
				int angle = (atan2(v1.y, v2.y) - atan2(v1.x, v2.x)) * (180/M_PI);
				//tB->setText("Angle: "+std::to_string(angle)+" \n v2.x: "+std::to_string(v2.x)+"\n V2.y: "+std::to_string(v2.y)+"\n TankPath[i] "+std::to_string(tankPath[i].at(currentNode[i]+1))+ "\n");
				//tB->setText(std::to_string(q1.w)+" , "+std::to_string(q1.x)+" , "+std::to_string(q1.y)+" , "+std::to_string(q1.z)+" \n"+std::to_string(q2.w)+" , "+std::to_string(q2.x)+" , "+std::to_string(q2.y)+" , "+std::to_string(q2.z)+"\n");
				//tB->appendText("atan: "+std::to_string(tanVal)+"\n");
				yDeg +=Ogre::Degree(1);
				Ogre::Quaternion rot;

				//Ogre::Vector3 lookat = m_nodes[0]->convertWorldToLocalPosition(m_camera->getPosition()) - Vector3(0, 1.9, 0);
				Ogre::Quaternion temp = tankNode[i]->getPosition().getRotationTo(goal);   // temp is how much you need to rotate to get from the current orientation to the new orientation
				//temp.limitYaw(Ogre::Radian(evt.timeSinceLastFrame*3.0));   // limit the offset so the head turns at a maximum of 3.0 radians per second
				//temp.limitPitch(Ogre::Radian(evt.timeSinceLastFrame*3.0));
 
				rot = rot + temp;   // add the offset to the current orientation
				//e2.limitYaw(Ogre::Degree(60));   // make sure the head doesn't turn too far. Clamp it to +/- 60 degrees of yaw and pitch.
				//e2.limitPitch(Ogre::Degree(60));
				//tankNode[i]->setOrientation(rot);
			  // mat.FromEulerAnglesYXZ(yDeg, pDeg, rDeg);
			 //  quat.FromRotationMatrix(mat);
				if(tankNode[i]->getOrientation()== orientDest[i] && firstTime[i]==false )
				{
					//mRotating = false;
					if(firstTime[i]==true)
					{
						firstTime[i]=false;
						mRotProgress[i]=0;
						Ogre::Quaternion orig=tankNode[i]->_getDerivedOrientation();
						tankNode[i]->lookAt(goal,Ogre::Node::TS_WORLD, Ogre::Vector3::UNIT_X);
						orientDest[i]=tankNode[i]->_getDerivedOrientation();
						tankNode[i]->setOrientation(orig);
					}
					tankNode[i]->translate(tankNode[i]->getOrientation().xAxis() * 0.1);
					//tB->appendText("Moving");
					//mRotProgress[i]=0;
				}
				else
				{
					if(firstTime[i]==true)
					{
						
						firstTime[i]=false;
						mRotProgress[i]=0;
						Ogre::Quaternion orig=tankNode[i]->_getDerivedOrientation();
						tankNode[i]->lookAt(goal,Ogre::Node::TS_WORLD, Ogre::Vector3::UNIT_X);
						orientDest[i]=tankNode[i]->_getDerivedOrientation();
						tankNode[i]->setOrientation(orig);

						//orientDest[i]=tankNode[i]->_getDerivedPosition().getRotationTo(goal) * tankNode[i]->_getDerivedOrientation();//   getOrientation(); 
					}
					if(tankNode[i]->getOrientation()== orientDest[i])
						tB->setText("Same Orientation \n");
					else
						tB->setText("Different Orientation \n");
					mRotProgress[i]+=1.0f;
					//Ogre::Quaternion delta = Ogre::Quaternion::Slerp(mRotProgress[i], q1, q2, true);
					//tankNode[i]->setOrientation(Ogre::Quaternion::Slerp(evt.timeSinceLastFrame*2, q1, mRotDest));
					Ogre::Quaternion delta = Ogre::Quaternion::nlerp(mRotProgress[i]*evt.timeSinceLastFrame, tankNode[i]->getOrientation(), orientDest[i], true);
					tankNode[i]->setOrientation(delta);
					//tB->setText(std::to_string(q1.w)+" , "+std::to_string(q1.x)+" , "+std::to_string(q1.y)+" , "+std::to_string(q1.z)+" \n"+std::to_string(orientDest[i].w)+" , "+std::to_string(orientDest[i].x)+" , "+std::to_string(orientDest[i].y)+" , "+std::to_string(orientDest[i].z)+"\n");
					//tB->appendText("Rotating");
					
				}

				if(WithinBounds(current,goal))
				{
						tB->appendText("Reset");
						currentNode[i]++;
						mRotProgress[i]=0;
						//tankNode[i]->lookAt(goal,Ogre::Node::TS_WORLD, Ogre::Vector3::UNIT_X);
						firstTime[i]=true;
						//orientDest[i];
				}
				
				//tankNode[i]->translate(tankNode[i]->_getDerivedOrientation().xAxis() * 0.1);
			}
			else
			{
				// reset
				mCurrentState[i] = 0;
				//path1->clear();
				path2[i]->clear();
				tankPath[i].clear();
				currentNode[i]=-1;
				firstTime[i]=true;
			}

			//tB->appendText("CURRENT NODE: "+std::to_string(currentNode)+"\n");
			//tB->appendText("CURRENT POS: "+std::to_string(current.x)+","+std::to_string(current.y)+","+std::to_string(current.z)+"\n");
			//tB->appendText("GOAL POS: "+std::to_string(goal.x)+","+std::to_string(goal.y)+","+std::to_string(goal.z)+"\n");
			//tB->appendText("SIZE: "+std::to_string(tankPath.size()));
		}
	}
}