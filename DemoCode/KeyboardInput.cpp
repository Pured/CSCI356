#include "stdafx.h"
#include "DemoApp.h"

void DemoApp::keyInput( const OIS::KeyEvent &arg )
{
	if(mTrayMgr->getTrayContainer(OgreBites::TL_BOTTOMLEFT)->isVisible() == false) //if chatbox is open, only typing is allowed
	{
		switch (arg.key)
		{
			case OIS::KC_A: //switch between AI and player control
				if(playerControl == true)
				{
					playerControl = false;
					think(); //turn on AI
				}
				else
				{
					playerControl = true;
					shutDown(); //turn off AI
				}

				break;
			case OIS::KC_C: //show/hide controls
				if(mTrayMgr->getTrayContainer(OgreBites::TL_CENTER)->isVisible() == false)
				{
					if(mTrayMgr->getTrayContainer(OgreBites::TL_TOPRIGHT)->isVisible() == false)
					{
						mTrayMgr->getTrayContainer(OgreBites::TL_TOPRIGHT)->show();
						controlsWasOpen = true;
					}
					else
					{
						mTrayMgr->getTrayContainer(OgreBites::TL_TOPRIGHT)->hide();
						controlsWasOpen = false;
					}
				}

				break;
			/*case OIS::KC_F:
				if(selectionMode==0)
					selectionMode++;
				else if(selectionMode==1)
				{
					follower.tankNode->setVisible(true);
					selectionMode++;
				}
				else if(selectionMode==2)
				{
					follower.tankNode->setVisible(false);
					selectionMode=1;
				}

				break;*/
			case OIS::KC_H: //show/hide hp bars
				//show/hide hp bars

				break;
			case OIS::KC_P: //print map to console
				for(int j=0;j<1024;j++)
				{
					printf("%i,",pathFindingGraph->getContent(j));
					if((j+1)%32==0 && j!=0)
						printf("\n");
				}
				printf("\n");
				printf("\n");

				break;
			case OIS::KC_Q: //click to activate special bullet
				//2x damage or 2x splash radius

				break;
			case OIS::KC_R: //reset camera
				//lookAtDest=Ogre::Vector3(0,0,0);
				camNode->setPosition(Ogre::Vector3(0,10,-10));
				//mCamera->setPosition(camNode->getPosition());
			
				//mCamera->setOrientation(Ogre::Quaternion());
				camNode->setOrientation(Ogre::Quaternion());
				//camNode->lookAt(Ogre::Vector3(0,10,0),Ogre::Node::TS_PARENT,Ogre::Vector3::NEGATIVE_UNIT_Z);

				break;
			case OIS::KC_T: //show/hide tank information
				if(mTrayMgr->getTrayContainer(OgreBites::TL_CENTER)->isVisible() == false)
				{
					if(mTrayMgr->getTrayContainer(OgreBites::TL_TOPLEFT)->isVisible() == false)
					{
						mTrayMgr->getTrayContainer(OgreBites::TL_TOPLEFT)->show();
						tankInfoWasOpen = true;
					}
					else
					{
						mTrayMgr->getTrayContainer(OgreBites::TL_TOPLEFT)->hide();
						tankInfoWasOpen = false;
					}
				}

				break;
			case OIS::KC_W: //use powerup
				//use powerup

				break;
			case OIS::KC_SPACE: //hold to charge shooting power
				//tank shooting code

				break;
			case OIS::KC_TAB: //show/hide scoreboard
				if(mTrayMgr->getTrayContainer(OgreBites::TL_CENTER)->isVisible() == false)
				{
					toggleOtherPanels();
					mTrayMgr->getTrayContainer(OgreBites::TL_CENTER)->show();
				}
				else
				{
					toggleOtherPanels();
					mTrayMgr->getTrayContainer(OgreBites::TL_CENTER)->hide();
				}

				break;
			case OIS::KC_BACK: //force tank respawn
				//kill the tank
				//edit score
				//respawn tank

				break;
	/*		case OIS::KC_LSHIFT: //hold for speed boost
				//give tank 2x speed
				//drain tank's nitros bar

				break;*/
			case OIS::KC_LCONTROL:
				controlPressed = true;

				break;
			case OIS::KC_LMENU:
				cMode=1;
				mTrayMgr->hideCursor();

				break;
			case OIS::KC_RETURN: //open chatbox
				if(mTrayMgr->getTrayContainer(OgreBites::TL_CENTER)->isVisible() == false)
				{
						mTrayMgr->getTrayContainer(OgreBites::TL_BOTTOMLEFT)->show();
				}

				break;
			case OIS::KC_ESCAPE: //exit program
				mShutDown = true;

				break;
			default:
				break;
		}
	}
	else //chat
	{
		if(arg.key == OIS::KC_RETURN) //close chat.
		{
			mTrayMgr->getTrayContainer(OgreBites::TL_BOTTOMLEFT)->hide();
			chatWasOpen = false;
		}
		else
		{
			if(chatWasOpen == false) //start a new chat line
			{
				chatBox->appendText("\nPlayer: ");

				chatWasOpen = true;
			}

			chatBox->appendText(std::to_string(arg.key));
		}
	}
}