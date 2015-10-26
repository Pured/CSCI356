#include "stdafx.h"
#include "DemoApp.h"

void DemoApp::keyInput( const OIS::KeyEvent &arg )
{
	if(mTrayMgr->getTrayContainer(OgreBites::TL_BOTTOMLEFT)->isVisible() == false) //if chatbox is open, only typing is allowed
	{
		switch (arg.key)
		{
			case OIS::KC_A: //switch between AI and player control
				if(playerControlled == -1) //if no tanks are player-controlled
				{
					for(int i = 0; i < TANK_LIMIT; i++) //find selected tank
					{
						if(tanks.at(i).selected == true) //control the tank (if multiple selected, control the last one selected)
						{
							playerControlled = i; //the tank being controlled

							shutDown(tanks.at(playerControlled)); //turn off the tank's AI
						}
					}
				}
				else //a tank is already being controlled
				{
					prevPlayerControlled = playerControlled; //prevents the retoggle of the same tank

					tanks.at(playerControlled).currentState = -1; //set the tank to the idle state

					think(tanks.at(playerControlled)); //turn on the tank's AI

					playerControlled = -1; //all tanks are AI-controlled

					for(int i = 0; i < TANK_LIMIT; i++) //look for a selected tank if there is one
					{
						if(tanks.at(i).selected == true && prevPlayerControlled != i) //control the tank (if multiple selected, control the last one selected)
						{
							playerControlled = i; //the tank being controlled

							shutDown(tanks.at(playerControlled)); //turn off the tank's AI
						}
					}

					prevPlayerControlled = -1;
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
			case OIS::KC_H: //show/hide hp bars
				//show/hide hp bars

				break;
			case OIS::KC_P: //print map to console
				for(int j=0;j<TOTAL_NODES;j++)
				{
					printf("%i,",pathFindingGraph->getContent(j));
					if((j+1)% GRID_DIMENSION ==0 && j!=0)
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
			case OIS::KC_PGUP: //spawn another tank
				if(TANK_LIMIT < 10 && mTrayMgr->getTrayContainer(OgreBites::TL_CENTER)->isVisible() == false) //hard limit of tanks allowed
				{
					TANK_LIMIT++; //increases the amount of tanks in the program

					createTank(namesAllocated); //create another tank

					respawnTank(namesAllocated - 1); //spawns the tank that was just created based on its team
				}

				break;
			case OIS::KC_PGDOWN: //destroy the last tank
				if (TANK_LIMIT>0)
				{
					destroyTank(TANK_LIMIT - 1);
					printf("After destroy \n");
					TANK_LIMIT--;
					namesAllocated--;
				}
				/*
				TANK_LIMIT--; //decreases the amount of tanks in the program
				namesAllocated--; //frees up the name of the tank being deleted

				tanks.pop_back();//destroy tank
				*/
				break;
			case OIS::KC_TAB: //show/hide scoreboard
				if(mTrayMgr->getTrayContainer(OgreBites::TL_CENTER)->isVisible() == false)
				{
					updateScoreboard(); //update the scoreboard

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
				//edit score -1
				//respawn tank

				break;
	/*		case OIS::KC_LSHIFT: //hold for speed boost
				//give tank 2x speed
				//drain tank's nitros bar

				break;*/
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

			char temp = getInputChar(arg);
			std::stringstream ss;
			std::string s;

			ss << temp;

			Ogre::DisplayString tempChar = ss.str();//std::to_string((temp)); //stores a char

			if(tempChar != "`"); //an invalid key was pressed
				chatBox->appendText(tempChar); //put the char pressed into the chatbox
		}
	}
}

char DemoApp::getInputChar(const OIS::KeyEvent &arg)
{
	char c = '`';

	switch(arg.key)
	{
		case OIS::KC_0:
			c = '0';
			break;
		case OIS::KC_1:
			c = '1';
			break;
		case OIS::KC_2:
			c = '2';
			break;
		case OIS::KC_3:
			c = '3';
			break;
		case OIS::KC_4:
			c = '4';
			break;
		case OIS::KC_5:
			c = '5';
			break;
		case OIS::KC_6:
			c = '6';
			break;
		case OIS::KC_7:
			c = '7';
			break;
		case OIS::KC_8:
			c = '8';
			break;
		case OIS::KC_9:
			c = '9';
			break;
		case OIS::KC_A:
			c = 'a';
			break;
		case OIS::KC_B:
			c = 'b';
			break;
		case OIS::KC_C:
			c = 'c';
			break;
		case OIS::KC_D:
			c = 'd';
			break;
		case OIS::KC_E:
			c = 'e';
			break;
		case OIS::KC_F:
			c = 'f';
			break;
		case OIS::KC_G:
			c = 'g';
			break;
		case OIS::KC_H:
			c = 'h';
			break;
		case OIS::KC_I:
			c = 'i';
			break;
		case OIS::KC_J:
			c = 'j';
			break;
		case OIS::KC_K:
			c = 'k';
			break;
		case OIS::KC_L:
			c = 'l';
			break;
		case OIS::KC_M:
			c = 'm';
			break;
		case OIS::KC_N:
			c = 'n';
			break;
		case OIS::KC_O:
			c = 'o';
			break;
		case OIS::KC_P:
			c = 'p';
			break;
		case OIS::KC_Q:
			c = 'q';
			break;
		case OIS::KC_R:
			c = 'r';
			break;
		case OIS::KC_S:
			c = 's';
			break;
		case OIS::KC_T:
			c = 't';
			break;
		case OIS::KC_U:
			c = 'u';
			break;
		case OIS::KC_V:
			c = 'v';
			break;
		case OIS::KC_W:
			c = 'w';
			break;
		case OIS::KC_X:
			c = 'x';
			break;
		case OIS::KC_Y:
			c = 'y';
			break;
		case OIS::KC_Z:
			c = 'z';
			break;
		case OIS::KC_SPACE:
			c = ' ';
			break;
		case OIS::KC_PERIOD:
			c = '.';
			break;
		case OIS::KC_COMMA:
			c = ',';
			break;
		case OIS::KC_SLASH:
			c = '?';
			break;
	}

	return c;
}