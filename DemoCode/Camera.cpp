#include"stdafx.h"
#include "DemoApp.h"
void DemoApp::camMovement( const OIS::MouseEvent &arg )
{
	//Rotation and movement variables
	if(cMode==0)
	{
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
					quickSelect();
				}
				boxTimeout=0;
				mousePressedVar=false;
			}
			break;
			case OIS::MB_Right:
			{
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

	if((itr != result.end() && itr->movable))
	{
		
		//Get hit location
		Ogre::String name = itr->movable->getName();
		bool found=false;
		if(name=="Box")
		{
			itr++;
			name = itr->movable->getName();
		}

		for(int i=0;i<TANK_LIMIT;i++)
		{
			tB->appendText("RAY NAME: "+name+"\n");
			if(name=="Tank"+std::to_string(i))
			{
				tB->appendText("TANK QUICKSELECTED: "+std::to_string(i)+"\n");
				tanks.at(i).selected=true;
				if(tanks.at(i).mSelectionCircle->getParentSceneNode()!=tanks.at(i).tankNode)
					tanks.at(i).tankNode->attachObject(tanks.at(i).mSelectionCircle);
				if(tanks.at(i).mHealthBar->getParentSceneNode()!=tanks.at(i).tankNode)
					tanks.at(i).tankNode->attachObject(tanks.at(i).mHealthBar);
				found=true;	
				tanks.at(i).path2->setVisible(true);
			}
			else if(controlPressed==false)
			{
				tanks.at(i).selected=false;
				if(tanks.at(i).mSelectionCircle->getParentSceneNode()==tanks.at(i).tankNode)
					tanks.at(i).tankNode->detachObject(tanks.at(i).mSelectionCircle);
				if(tanks.at(i).mHealthBar->getParentSceneNode()==tanks.at(i).tankNode)
					tanks.at(i).tankNode->detachObject(tanks.at(i).mHealthBar);
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
		// if path already exists
		if(tanks.at(i).mCurrentState > 1)
		{
			// reset
			tanks.at(i).mCurrentState = 0;
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
			if((itr != result.end()))
			{
				//Get hit location
				Ogre::Vector3 location = mouseRay.getPoint(itr->distance);

				// if hit the floor
				if((location.y < 0.001 && selectionMode==1) || selectionMode==0)
				{
					// if no start node yet
					//tanks.at(i).mCurrentState=1;
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
						// check that goal node is not the same as start node
						if(tanks.at(i).goalNode != tanks.at(i).startNode)
						{
							findPath(i);
						}
						else
						{
							resetPath(i);
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
			for(int i=0;i<TANK_LIMIT;i++)
			{
				tanks.at(i).selected=false;
				tanks.at(i).tankNode->detachObject(tanks.at(i).mSelectionCircle);
				tanks.at(i).tankNode->detachObject(tanks.at(i).mHealthBar);
				tanks.at(i).path2->setVisible(false);
			}
		}
		tB->setText(std::to_string(left)+" , "+std::to_string(right)+" , "+std::to_string(1-top)+" , "+std::to_string(1-bottom)+"\n");
		bool found=false;
		for(int i=0;i<TANK_LIMIT;i++)
		{
			
			tB->appendText("TANK"+std::to_string(i)+": "+std::to_string(GetScreenspaceCoords(tanks.at(i).tankNode->getPosition(),*mCamera).x)+" , "+std::to_string(GetScreenspaceCoords(tanks.at(i).tankNode->getPosition(),*mCamera).y)+"\n");

			if(GetScreenspaceCoords(tanks.at(i).tankNode->getPosition(),*mCamera).x > left && GetScreenspaceCoords(tanks.at(i).tankNode->getPosition(),*mCamera).x < right && GetScreenspaceCoords(tanks.at(i).tankNode->getPosition(),*mCamera).y < 1-top && GetScreenspaceCoords(tanks.at(i).tankNode->getPosition(),*mCamera).y > 1-bottom)
			{
				tB->appendText("TANK SELECTED: "+std::to_string(i)+"\n");
				tanks.at(i).selected=true;
				if(tanks.at(i).mSelectionCircle->getParentSceneNode()!=tanks.at(i).tankNode)
					tanks.at(i).tankNode->attachObject(tanks.at(i).mSelectionCircle);
				if(tanks.at(i).mHealthBar->getParentSceneNode()!=tanks.at(i).tankNode)
					tanks.at(i).tankNode->attachObject(tanks.at(i).mHealthBar);
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
    screenSpacePoint.x = (point.x / 2.f) + 0.5f;
    screenSpacePoint.y = (point.y / 2.f) + 0.5f;

    return screenSpacePoint;
}
void DemoApp::frameRenderingCamera()
{
	Ogre::Vector3 camPos;
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
		camNode->translate((camNode->getOrientation()*Ogre::Vector3::NEGATIVE_UNIT_Z)  * -camZ);
		camPos=camNode->getPosition();
		if(camZ==0)
			camNode->setPosition(camPos.x,height.y,camPos.z);
	}
	else if(cMode==1)
	{
			
		camNode->translate((camNode->getOrientation()*Ogre::Vector3::NEGATIVE_UNIT_Z)  * -camZ);

		camNode->yaw(mRotationX);
		camNode->pitch(mRotationY);
		camPos=camNode->getPosition();
		mCamera->setOrientation(Ogre::Quaternion());		
	}
	if(cMode==1)
	{
		camX=0;
		camY=0;	
	}
	camZ=0;
	mRotationX=0;
	mRotationY=0;

}