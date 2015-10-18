#include "stdafx.h"
#include "DemoApp.h"

void DemoApp::think(/*tank t*/)
{
//loop for all tanks?
	//do checks
	//then act
}

void DemoApp::shutDown(/*tank t*/)
{
//loop for all tanks?
	//changeState(-1, t);
}

void DemoApp::changeState(int x, tank t)
{
	switch(x)
	{
		case 0: //solve labrinth
			//...

			break;
		case 1: //move
			//...

			break;
		case 2: //follow
			//...

			break;
		case 3: //help
			//...

			break;
		case 4: //attack
			//...

			break;
		case 5: //shoot
			//...

			break;
		case 6: //escape
			//...

			break;
		case 7: //get power-up
			//...

			break;
		case -1: //force idle
			//...

			break;
		default:
			break;
	}
}