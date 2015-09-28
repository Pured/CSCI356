#include "DemoApp.h"
void DemoApp::camMovement( const OIS::MouseEvent &arg )
{
	//Rotation and movement variables
	//mRotationX = Ogre::Degree(-mMouse->getMouseState().X.rel * 0.13);
	//mRotationY = Ogre::Degree(-mMouse->getMouseState().Y.rel * 0.13);
	if(cMode==0)
	{
		/*if(arg.state.Z.rel>0)
			camZ-=0.1;
		else if(arg.state.Z.rel<0 )
			camZ+=0.1;*/
		if(mMouse->getMouseState().X.abs > vp->getActualWidth()-50 )
			camX+=0.1;
		else if(mMouse->getMouseState().X.abs < 50 )
			camX-=0.1;
		else
			camX=0;

		if(mMouse->getMouseState().Y.abs > vp->getActualHeight()-50 )
			camY+=0.1;
		else if(mMouse->getMouseState().Y.abs < 50 )
			camY-=0.1;
		else
			camY=0;
	}
	else if(cMode==1)
	{
		/*if(mMouse->getMouseState().Y.rel > 0)
		{
			camY+=0.003;
		}
		else if(mMouse->getMouseState().X.rel > 0)
		{
			camY+=0.003;
		}*/
		//if(mMouse->getMouseState().Y.rel > 0)
		camY = arg.state.Y.rel*0.7;
		//else if(mMouse->getMouseState().X.rel > 0)
		camX = arg.state.X.rel*0.7;
	}
	camZ = arg.state.Z.rel*-0.7;
}
void DemoApp::camPressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	switch (id)
	{
		case OIS::MB_Left:
			{
				// if path already exists
				if(mCurrentState > 1)
				{
					// reset
					mCurrentState = 0;
					path1->clear();
					path2->clear();
				}
				// if no path yet
				else
				{
					// Create RaySceneQuery
					Ogre::Ray mouseRay = mCamera->getCameraToViewportRay(
						static_cast<float>(mMouse->getMouseState().X.abs)/mMouse->getMouseState().width, 
						static_cast<float>(mMouse->getMouseState().Y.abs)/mMouse->getMouseState().height);

					Ogre::RaySceneQuery * mRaySceneQuery = mSceneMgr->createRayQuery(Ogre::Ray());

					// Set ray
					mRaySceneQuery->setRay(mouseRay);

					// Ray-cast and get first hit
					Ogre::RaySceneQueryResult &result = mRaySceneQuery->execute();
					Ogre::RaySceneQueryResult::iterator itr = result.begin();

					// if hit an object
					if(itr != result.end())
					{
						// Get hit location
						Ogre::Vector3 location = mouseRay.getPoint(itr->distance);

						// if hit the floor
						if(location.y < 0.001)
						{
							// if no start node yet
							mCurrentState=1;
							if(mCurrentState == 0)
							{
								// set start node
								startNode = pathFindingGraph->getNode(tankNode->_getDerivedPosition());
								// set state to goal node state
								mCurrentState++;
							}
							// if start node already assigned
							else if(mCurrentState == 1)
							{
								startNode = pathFindingGraph->getNode(tankNode->_getDerivedPosition());
								// set goal node
								goalNode = pathFindingGraph->getNode(location);

								// check that goal node is not the same as start node
								if(goalNode != startNode)
								{
									// try to find path from start to goal node
									std::vector<int> path;

									// if path exists
									//if(mPathFinder.Dijkstra(startNode, goalNode, *pathFindingGraph, path))
									if(mPathFinder.AStar(startNode, goalNode, *pathFindingGraph, path))
									{
										// draw path
										//createPath(path1, 0.5, path, Ogre::ColourValue(1, 0, 0));
						
										//mPathFinder.AStar(startNode, goalNode, *pathFindingGraph, path);
										createPath(path2, 1.0, path, Ogre::ColourValue(0, 0, 1));

										// set state to path found
										mCurrentState++;

										//WIP TANK
										//std::copy(path.begin(), path.end(), tankPath.begin());
										for (std::vector<int>::iterator it = path.begin(); it!=path.end(); ++it)
										{
											tankPath.push_back(*it);
										}
										currentNode=0;
									}
									else
									{
										// no path so set state to no start node
										mCurrentState = 0;
									}
								}
							}
						}
					}
				}
			}
			break;
		default:
			break;
	}
}
bool DemoApp::WithinBounds(Ogre::Vector3 current, Ogre::Vector3 goal)
{
	if(current.x < goal.x+0.1 && current.x > goal.x-0.1 && current.z < goal.z+0.1 && current.z > goal.z-0.1)
		return true;
	else
		return false;
}
void DemoApp::camInput( const OIS::KeyEvent &arg )
{
	 switch (arg.key)
	{
		case OIS::KC_ESCAPE: 
			mShutDown = true;
			break;
		case OIS::KC_LMENU: 
			cMode=1;
			break;
		case OIS::KC_R:
			//lookAtDest=Ogre::Vector3(0,0,0);
			camNode->setPosition(Ogre::Vector3(0,10,-10));
			//mCamera->setPosition(camNode->getPosition());
			
			//mCamera->setOrientation(Ogre::Quaternion());
			camNode->setOrientation(Ogre::Quaternion());


			break;
		default:
			break;
	}
}

void DemoApp::frameRenderingCamera()
{
	Ogre::Vector3 camPos;
	if(cMode==0)
		tB->setText("CMODE: "+std::to_string(cMode)+"\n");
	else if(cMode==1)
		tB->setText("CMODE: "+std::to_string(cMode)+"\n");
	if(cMode==0)
	{

		camNode->translate((camNode->getOrientation()*Ogre::Vector3::UNIT_X)  * camX);
		camNode->translate((camNode->getOrientation()*Ogre::Vector3::UNIT_Z)  * camY);
		camNode->translate((camNode->getOrientation()*Ogre::Vector3::NEGATIVE_UNIT_Z)  * -camZ);
		camPos=camNode->getPosition();
		//mCamera->getRealOrientation();
		//mCamera->moveRelative( (Ogre::Vector3(0,1,1) ) * camY );
		//mCamera->moveRelative(  );
	}
	else if(cMode==1)
	{
		//if(lookAtDest==Ogre::Vector3(0,0,0))
		{
			//camNode->setOrientation(Ogre::Quaternion());
			//mCamera->setOrientation(Ogre::Quaternion());
			//camNode->detachAllObjects();
			//Ogre::Ray camRay;	
			//camRay=mCamera->getCameraToViewportRay(0.5,0.5);
			//lookAtDest = camRay.getPoint(50);
			//lookAtDest.y=5;
			//mCamera->lookAt((lookAtDest));
			//camNode->lookAt(lookAtDest,Ogre::Node::TS_PARENT,Ogre::Vector3::NEGATIVE_UNIT_Z);
		}
		//else
		{
			camNode->translate((camNode->getOrientation()*Ogre::Vector3::UNIT_X)  * camX);
			camNode->translate((camNode->getOrientation()*Ogre::Vector3::UNIT_Y)  * camY);
			
			camNode->translate((camNode->getOrientation()*Ogre::Vector3::NEGATIVE_UNIT_Z)  * -camZ);
			camNode->lookAt(Ogre::Vector3(0,10,0),Ogre::Node::TS_PARENT,Ogre::Vector3::NEGATIVE_UNIT_Z);
			//camNode->translate((camNode->getOrientation()*Ogre::Vector3::UNIT_Z)  * camZ);
			//camPos=camNode->getPosition();
			//lookAtDest.z=camNode->getPosition().z;
			//mCamera->moveRelative(Ogre::Vector3::UNIT_X * camX);
			//mCamera->moveRelative(Ogre::Vector3::UNIT_Y * camY);
			//mCamera->moveRelative(Ogre::Vector3::UNIT_Z * camZ);
			//mCamera->lookAt((lookAtDest));
		
			//camNode->lookAt(lookAtDest,Ogre::Node::TS_WORLD,Ogre::Vector3::NEGATIVE_UNIT_Z);
		}
		camPos=camNode->getPosition();
		//
		//camNode->setOrientation(Ogre::Quaternion(0,0,0,1));
	}

	tB->appendText("Look At Dest: ("+std::to_string(lookAtDest.x)+")-("+std::to_string(lookAtDest.y)+")-("+std::to_string(lookAtDest.z)+")\n");
	tB->appendText("Cam Pos Dest: ("+std::to_string(camPos.x)+")-("+std::to_string(camPos.y)+")-("+std::to_string(camPos.z)+")\n");
	//R=camZ*(M_PI/(double)180);
	//mCamera->setFOVy(R);
	if(cMode==1)
	{
		camX=0;
		camY=0;	
	}
	camZ=0;
}