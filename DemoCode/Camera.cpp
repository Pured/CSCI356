#include"stdafx.h"
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
		//camY = arg.state.Y.rel*0.7;
		//else if(mMouse->getMouseState().X.rel > 0)
		//camX = arg.state.X.rel*0.7;

		mRotationX = Ogre::Degree(-mMouse->getMouseState().X.rel * 0.13);
		mRotationY = Ogre::Degree(-mMouse->getMouseState().Y.rel * 0.13);
	}
	camZ = arg.state.Z.rel*-0.05;
}
bool DemoApp::camMouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	if (mTrayMgr->injectMouseUp(arg, id)) return true;

	switch (id)
	{
		case OIS::MB_Left:
			{
				// If visible set to invisible
				if(mSelectionBox->isVisible())
				{
					mSelectionBox->setVisible(false);
					mSelecting = false;
				}
				if(boxTimeout<6)
				{
					//if(quickSelect()==false)
					quickSelect();
						nextClickPath=false;
					//else
					//	nextClickPath=true;
				}
				/*else
				{
					if(nextClickPath==true)
					{
						nextClickPath=false;
					}
					else
						nextClickPath=true;
				}*/
				boxTimeout=0;
				mousePressedVar=false;
			}
			break;
			case OIS::MB_Right:
			{
				//if(nextClickPath==true)
				if(selectionMode==2)
				{
					//tankPath[0]=
					//currentNode[0]=0;
					//firstTime[0]=true;
					//selected[0]=true;
				}
				generatePath();
			}
			break;
		default:
			break;
	}

	return true;
}
bool DemoApp::quickSelect()
{
	tB->setText("In QUICK SELECTED \n");
	tB->appendText("Selection Mode: "+std::to_string(selectionMode)+"\n");
	// Create RaySceneQuery
	Ogre::Ray mouseRay = mCamera->getCameraToViewportRay(static_cast<float>(mMouse->getMouseState().X.abs)/mMouse->getMouseState().width,		static_cast<float>(mMouse->getMouseState().Y.abs)/mMouse->getMouseState().height);

	Ogre::RaySceneQuery * mRaySceneQuery = mSceneMgr->createRayQuery(Ogre::Ray());

	// Set ray
	mRaySceneQuery->setRay(mouseRay);

	mRaySceneQuery->setQueryTypeMask(Ogre::SceneManager::ENTITY_TYPE_MASK);
			
	mRaySceneQuery->setSortByDistance(true);

	// Ray-cast and get first hit
	Ogre::RaySceneQueryResult &result = mRaySceneQuery->execute();
	Ogre::RaySceneQueryResult::iterator itr = result.begin();

	// if hit an object
	//&& selectionMode==1) || selectionMode==0
	if((itr != result.end() && itr->movable))
	{
		
		//Get hit location
		//Ogre::Vector3 location = mouseRay.getPoint(itr->distance);

		Ogre::String name = itr->movable->getName();
		bool found=false;
		if(name=="Box")
		{
			itr++;
			name = itr->movable->getName();
		}

		for(int i=1;i<TANK_LIMIT;i++)
		{
			tB->appendText("RAY NAME: "+name+"\n");
			if(name=="Tank"+std::to_string(i))
			{
				tB->appendText("TANK QUICKSELECTED: "+std::to_string(i)+"\n");
				tanks.at(i).selected=true;
				if(mSelectionCircle[i]->getParentSceneNode()!=tanks.at(i).tankNode)
					tanks.at(i).tankNode->attachObject(mSelectionCircle[i]);
				if(mHealthBar[i]->getParentSceneNode()!=tanks.at(i).tankNode)
					tanks.at(i).tankNode->attachObject(mHealthBar[i]);
				found=true;
				tanks.at(i).path2->setVisible(true);
			}
			else if(controlPressed==false)
			{
				tanks.at(i).selected=false;
				if(mSelectionCircle[i]->getParentSceneNode()==tanks.at(i).tankNode)
					tanks.at(i).tankNode->detachObject(mSelectionCircle[i]);
				if(mHealthBar[i]->getParentSceneNode()==tanks.at(i).tankNode)
					tanks.at(i).tankNode->detachObject(mHealthBar[i]);
				tanks.at(i).path2->setVisible(false);
			}
		}
		if(found==false)
			return false;
		else return true;
	}
	return false;
}
void DemoApp::generatePath()
{
	tB->setText("");
	for(int i=0;i<TANK_LIMIT;i++)
	{
		//tB->appendText("START OF FOR : "+std::to_string(i)+"\n");
		// if path already exists
		if(tanks.at(i).mCurrentState > 1)
		{
			// reset
			tanks.at(i).mCurrentState = 0;
			//path1->clear();
			tanks.at(i).path2->clear();
		}
		// if no path yet
		else if(tanks.at(i).selected==true)
		{
			tB->appendText("TANK SELECTED: "+std::to_string(i)+"\n");
			tB->appendText("Selection Mode: "+std::to_string(selectionMode)+"\n");
			// Create RaySceneQuery
			Ogre::Ray mouseRay = mCamera->getCameraToViewportRay(static_cast<float>(mMouse->getMouseState().X.abs)/mMouse->getMouseState().width,		static_cast<float>(mMouse->getMouseState().Y.abs)/mMouse->getMouseState().height);

			Ogre::RaySceneQuery * mRaySceneQuery = mSceneMgr->createRayQuery(Ogre::Ray());

			// Set ray
			mRaySceneQuery->setRay(mouseRay);

			// Ray-cast and get first hit
			Ogre::RaySceneQueryResult &result = mRaySceneQuery->execute();
			Ogre::RaySceneQueryResult::iterator itr = result.begin();

			// if hit an object
			//&& selectionMode==1) || selectionMode==0
			if((itr != result.end()))
			{
				//Get hit location
				Ogre::Vector3 location = mouseRay.getPoint(itr->distance);

				// if hit the floor
				if((location.y < 0.001 && selectionMode==1) || selectionMode==0)
				{
					// if no start node yet
					tanks.at(i).mCurrentState=1;
					//Start node is always the current position of the object
					//if(mCurrentState[i] == 1)
					{
						tanks.at(i).startNode = pathFindingGraph->getNode(tanks.at(i).tankNode->_getDerivedPosition());
						// set goal node
						if(selectionMode==0)
						{
							do
							{
								tanks.at(i).goalNode = (int)rand()%256;
							}while(pathFindingGraph->getContent(tanks.at(i).goalNode)!=0);
						}
						else
						{
							tanks.at(i).goalNode=pathFindingGraph->getNode(location);
						}
						//tB->appendText("GOAL NODE: "+std::to_string(goalNode[i])+" \n");
						//tB->appendText("START NODE: "+std::to_string(startNode[i])+" \n");
						// check that goal node is not the same as start node
						if(tanks.at(i).goalNode != tanks.at(i).startNode)
						{
							// try to find path from start to goal node
							std::vector<int> path;
							tanks.at(i).path2->clear();
							tanks.at(i).mCurrentState = 0;
							tanks.at(i).tankPath.clear();
							tanks.at(i).currentNode=-1;
							// if path exists
							//if(mPathFinder.Dijkstra(startNode, goalNode, *pathFindingGraph, path))
							if(mPathFinder.AStar(tanks.at(i).startNode, tanks.at(i).goalNode, *pathFindingGraph, path))
							{
								// draw path
								//createPath(path1, 0.5, path, Ogre::ColourValue(1, 0, 0));
						
								//mPathFinder.AStar(startNode, goalNode, *pathFindingGraph, path);
								createPath(tanks.at(i).path2, 1.0, path, Ogre::ColourValue(0, 0, 1));

								// set state to path found
								tanks.at(i).mCurrentState++;

								//WIP TANK
								//std::copy(path.begin(), path.end(), tankPath.begin());
								for (std::vector<int>::iterator it = path.begin(); it!=path.end(); ++it)
								{
									tanks.at(i).tankPath.push_back(*it);
								}
								tanks.at(i).currentNode=0;
								tanks.at(i).firstTime=true;
								if(selectionMode==2)
								{
									tanks.at(0).tankPath=tanks.at(i).tankPath;
									tanks.at(0).currentNode=0;
									tanks.at(0).firstTime=true;
								}
							}
							else
							{
								tB->appendText("COULD NOT FIND PATH \n");
								// no path so set state to no start node
								tanks.at(i).mCurrentState = 0;
							}
						}
						else
						{
							tanks.at(i).path2->clear();
							tanks.at(i).mCurrentState = 0;
							tanks.at(i).tankPath.clear();
							tanks.at(i).currentNode=-1;
							tanks.at(i).firstTime=true;
						}
					}
				}
			}
		}
	}
}
void DemoApp::camPressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	switch (id)
	{
		case OIS::MB_Left:
			{
				mousePressedVar=true;
				// Store starting position
				pos1.x = mMouse->getMouseState().X.abs;
				pos1.y = mMouse->getMouseState().Y.abs;
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
			mTrayMgr->hideCursor();
			break;
		case OIS::KC_R:
			//lookAtDest=Ogre::Vector3(0,0,0);
			camNode->setPosition(Ogre::Vector3(0,10,-10));
			//mCamera->setPosition(camNode->getPosition());
			
			//mCamera->setOrientation(Ogre::Quaternion());
			camNode->setOrientation(Ogre::Quaternion());
			//camNode->lookAt(Ogre::Vector3(0,10,0),Ogre::Node::TS_PARENT,Ogre::Vector3::NEGATIVE_UNIT_Z);
			break;
		case OIS::KC_F:
				if(selectionMode==0)
					selectionMode++;
				else if(selectionMode==1)
					selectionMode++;
				else if(selectionMode==2)
					selectionMode=0;
			break;
		case OIS::KC_LCONTROL:
				controlPressed=true;
			break;
		default:
			break;
	}
}
bool DemoApp::selectionBox()
{
	//tB->setText("In selection box");
	// If left mouse button is down and a starting position is already stored
	if(mMouse->getMouseState().buttonDown(OIS::MouseButtonID::MB_Left) && mSelecting)
	{
		//tB->setText("In selection box if");
		float left, right, top, bottom;
		// Get screen width and height
		int screenWidth = mWindow->getWidth();
		int screenHeight = mWindow->getHeight();

		// Get end position
		pos2.x = mMouse->getMouseState().X.abs;
		pos2.y = mMouse->getMouseState().Y.abs;
				
		// Find left and right boundaries
		if(pos1.x < pos2.x)
		{
			left = static_cast<float>(pos1.x) / screenWidth;
			right = static_cast<float>(pos2.x) / screenWidth;
		}
		else
		{
			left = static_cast<float>(pos2.x) / screenWidth;
			right = static_cast<float>(pos1.x) / screenWidth;
		}

		// Find top and bottom boundaries
		if(pos1.y < pos2.y)
		{
			top = static_cast<float>(pos1.y) / screenHeight;
			bottom = static_cast<float>(pos2.y) / screenHeight;
		}
		else
		{
			top = static_cast<float>(pos2.y) / screenHeight;
			bottom = static_cast<float>(pos1.y) / screenHeight;
		}

		// Find selection box corners in 3D coordinates in front of the camera
		Ogre::Ray topLeft = mCamera->getCameraToViewportRay(left, top); 
		Ogre::Ray topRight = mCamera->getCameraToViewportRay(right, top); 
		Ogre::Ray bottomRight = mCamera->getCameraToViewportRay(right, bottom); 
		Ogre::Ray bottomLeft = mCamera->getCameraToViewportRay(left, bottom); 

		// Update the vertices of the selection box
		mSelectionBox->beginUpdate(0);
		mSelectionBox->position(topLeft.getPoint(1));
		mSelectionBox->position(topRight.getPoint(1));
		mSelectionBox->position(bottomRight.getPoint(1));
		mSelectionBox->position(bottomLeft.getPoint(1));
		mSelectionBox->position(topLeft.getPoint(1));

		// Finished defining the 2D line strip
		mSelectionBox->end();

		// If not visible set to visible
		if(!mSelectionBox->isVisible())
			mSelectionBox->setVisible(true);
		if(controlPressed==false)
		{
			for(int i=1;i<TANK_LIMIT;i++)
			{
				tanks.at(i).selected=false;
				tanks.at(i).tankNode->detachObject(mSelectionCircle[i]);
				tanks.at(i).tankNode->detachObject(mHealthBar[i]);
				tanks.at(i).path2->setVisible(false);
			}
		}
		tB->setText(std::to_string(left)+" , "+std::to_string(right)+" , "+std::to_string(1-top)+" , "+std::to_string(1-bottom)+"\n");
		bool found=false;
		for(int i=0;i<TANK_LIMIT;i++)
		{
			
			tB->appendText("TANK"+std::to_string(i)+": "+std::to_string(GetScreenspaceCoords(tanks.at(i).tankNode->getPosition(),*mCamera).x)+" , "+std::to_string(GetScreenspaceCoords(tanks.at(i).tankNode->getPosition(),*mCamera).y)+"\n");
			//if(GetScreenspaceCoords(tankNode[i]->getPosition(),*mCamera).x > left && GetScreenspaceCoords(tankNode[i]->getPosition(),*mCamera).x < right)
			//	tB->appendText("TANK: "+std::to_string(i)+"  RIGHT+LEFT CORRECT\n");
			//if(GetScreenspaceCoords(tankNode[i]->getPosition(),*mCamera).y > 1-top)
			//	tB->appendText("TANK: "+std::to_string(i)+"  TOP CORRECT\n");
			if(GetScreenspaceCoords(tanks.at(i).tankNode->getPosition(),*mCamera).x > left && GetScreenspaceCoords(tanks.at(i).tankNode->getPosition(),*mCamera).x < right && GetScreenspaceCoords(tanks.at(i).tankNode->getPosition(),*mCamera).y < 1-top && GetScreenspaceCoords(tanks.at(i).tankNode->getPosition(),*mCamera).y > 1-bottom)
			{
				tB->appendText("TANK SELECTED: "+std::to_string(i)+"\n");
				tanks.at(i).selected=true;
				if(mSelectionCircle[i]->getParentSceneNode()!=tanks.at(i).tankNode)
					tanks.at(i).tankNode->attachObject(mSelectionCircle[i]);
				if(mHealthBar[i]->getParentSceneNode()!=tanks.at(i).tankNode)
					tanks.at(i).tankNode->attachObject(mHealthBar[i]);
				found=true;
				tanks.at(i).path2->setVisible(true);
			}	
		}	
		if(found==false)
			return false;
		else 
			return true;
	}
}
Ogre::Vector2 DemoApp::GetScreenspaceCoords(const Ogre::Vector3& iPoint, const Ogre::Camera& iCamera)
{
    Ogre:: Vector3 point = iCamera.getProjectionMatrix() * (iCamera.getViewMatrix() * iPoint);

    Ogre::Vector2 screenSpacePoint = Ogre::Vector2::ZERO;
    screenSpacePoint.x = (point.x / 2.f) + 0.5f;// * mWindow->getWidth();
    screenSpacePoint.y = (point.y / 2.f) + 0.5f;// * mWindow->getHeight();

    return screenSpacePoint;
}
void DemoApp::frameRenderingCamera()
{
	Ogre::Vector3 camPos;
	/*if(cMode==0)
		tB->setText("CMODE: "+std::to_string(cMode)+"\n");
	else if(cMode==1)
		tB->setText("CMODE: "+std::to_string(cMode)+"\n");*/
						// Start position stored, move to next state
	// && nextClickPath==false
	if(mousePressedVar==true && boxTimeout>=6)
	{
		mSelecting = true;
		selectionBox();
	}
	else if(mousePressedVar==true )
	{
		boxTimeout++;
	}
	

	if(cMode==0)
	{
		Ogre::Vector3 height=camNode->getPosition();
		camNode->translate((camNode->getOrientation()*Ogre::Vector3::UNIT_X)  * camX);
		camNode->translate((camNode->getOrientation()*Ogre::Vector3::UNIT_Z)  * camY);
		camNode->translate((camNode->getOrientation()*Ogre::Vector3::NEGATIVE_UNIT_Y)  * -camZ);
		camPos=camNode->getPosition();
		if(camZ==0)
			camNode->setPosition(camPos.x,height.y,camPos.z);
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
			//camNode->translate((camNode->getOrientation()*Ogre::Vector3::UNIT_X)  * camX);
			//camNode->translate((camNode->getOrientation()*Ogre::Vector3::UNIT_Y)  * camY);
			
			camNode->translate((camNode->getOrientation()*Ogre::Vector3::NEGATIVE_UNIT_Z)  * -camZ);
			//camNode->lookAt(Ogre::Vector3(0,10,0),Ogre::Node::TS_PARENT,Ogre::Vector3::NEGATIVE_UNIT_Z);

			camNode->yaw(mRotationX);
			camNode->pitch(mRotationY);
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
		mCamera->setOrientation(Ogre::Quaternion());
		
		//0
		//camNode->setOrientation(Ogre::Quaternion(0,0,0,1));
	}

	//tB->appendText("Look At Dest: ("+std::to_string(lookAtDest.x)+")-("+std::to_string(lookAtDest.y)+")-("+std::to_string(lookAtDest.z)+")\n");
	//tB->appendText("Cam Pos Dest: ("+std::to_string(camPos.x)+")-("+std::to_string(camPos.y)+")-("+std::to_string(camPos.z)+")\n");
	//R=camZ*(M_PI/(double)180);
	//mCamera->setFOVy(R);
	//tB->setText("Selection Mode: "+std::to_string(selectionMode)+"\n");
	if(cMode==1)
	{
		camX=0;
		camY=0;	
	}
	camZ=0;
	mRotationX=0;
	mRotationY=0;

}