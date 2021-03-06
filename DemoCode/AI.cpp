#include "stdafx.h"
#include "DemoApp.h"
//-------------------------------------------------------------------------------------
void DemoApp::think(tank t)
{
	if(t.currentState == -1) //if idle, decide what to do
	{
		if(isEnemyVisible(t) == true) //look for enemy in range
			changeState(2, t); //attack enemy
		else if(visibleCollectibles > 0 /*&& distance < ???*/) //look for collectibles
			changeState(4, t); //pick up power-up
		else
			changeState(1, t); //move to a random location in the battlefield
	}
	else if(t.currentState == 1) //if moving, decide what to do
	{
		if(isEnemyVisible(t) == true) //look for enemy
			changeState(2, t); //attack enemy
		else if(visibleCollectibles > 0 /*&& distance < ???*/) //look for collectibles
			changeState(4, t); //pick up power-up
	}
	else if(t.currentState == 2) //if attacking, decide what to do
	{
		//search for other enemy tanks in range with less hp than the current enemy

		if(t.health <= 30 && t.enemy->health > 50) //if in a huge disadvantage, escape
			changeState(3, t); //escape from the enemy tank
		else
			fight(t); //fight the enemy
	}
	else if(t.currentState == 3) //if escaping, decide what to do
	{
		if(isEnemyVisible(t) == true) //look for enemy in range
		{
			t.enemy = &getVisibleEnemy(t); //get the enemy tank's information

			if(t.health <= 30 && t.enemy->health > 50) //if in a huge disadvantage, escape
				changeState(3, t); //escape from the enemy tank
			else //strong enough to fight the enemy tank
				changeState(2, t); //attack enemy
		}
		else if(visibleCollectibles > 0 /*&& distance < ???*/) //look for collectibles
			changeState(4, t); //pick up power-up
		else
			changeState(1, t); //move to a random location in the battlefield
	}
	else if(t.currentState == 4) //if collecting, decide what to do
	{
		if(isEnemyVisible(t) == true) //look for enemy in range
			changeState(2, t); //attack enemy
	}
}
//-------------------------------------------------------------------------------------
void DemoApp::shutDown(tank t)
{
	t.currentState = -2; //the tank is player controlled
}
//-------------------------------------------------------------------------------------
void DemoApp::changeState(int x, tank t)
{
	switch(x)
	{
		case 1: //move
			t.currentState = 1;

			generatePath(); //find a random place for the tanks to go to

			break;
		case 2: //attack
			t.currentState = 2;
			
			t.enemy = &getVisibleEnemy(t); //get the enemy tank's information

			if(t.health <= 30 && t.enemy->health > 50) //if in a huge disadvantage, escape
				changeState(3, t); //escape from the enemy tank
			else //strong enough to fight the enemy tank
				fight(t); //fight the enemy

			break;
		case 3: //escape
			t.currentState = 3;

			break;
		case 4: //get collectible
			t.currentState = 4;

			collectCollectible(t); //move towards a collectible

			break;
		case -1: //force idle
			t.currentState = -1;

			break;
		default:
			break;
	}
}
//-------------------------------------------------------------------------------------
bool DemoApp::isEnemyVisible(tank t)
{
	for (int i = 0; i < tanks.size(); i++)
	{
		if (t.selected == true)
		{
			if ((tanks.at(i).tankNode->getPosition().x - t.tankNode->getPosition().x) <= SHOT_RANGE)
			{
				if (tanks.at(i).team == 2)
				{
					return true;
				}
			}
		}
	}

	return false;
}
//-------------------------------------------------------------------------------------
DemoApp::tank DemoApp::getVisibleEnemy(tank t)
{
	tank e;

	for (int i = 0; i < tanks.size(); i++)
	{
		if (t.selected == true)
		{
			if ((tanks.at(i).tankNode->getPosition().x - t.tankNode->getPosition().x) <= SHOT_RANGE)
			{
				if (tanks.at(i).team == 2)
				{
					tanks.at(i).tankInVec = i;
					e = tanks.at(i);
					break;
				}
			}
		}
	}

	return e; //return the enemy tank found
}
//-------------------------------------------------------------------------------------
void DemoApp::fight(tank t)
{
	// Shoot enemy tank
	if (isEnemyVisible(t) == true)
	{
		tank e = getVisibleEnemy(t); 
		tanks.at(e.tankInVec).health -= SHOT_DAMAGE;
	}
}
//-------------------------------------------------------------------------------------
void DemoApp::collectCollectible(tank t)
{
	int item = -1;

	for(int x = 0; x < collectibles.size(); x++)
	{
		if(collectibles[x].isVisible == true)
		{
			item = x;

			x = collectibles.size(); //exit loop
		}
	}

	for(int j = 0; j < GRID_DIMENSION; j++)
	{
		if(pathFindingGraph->getContent(j) == 2)
			pathFindingGraph->setContent(j, 0);
	}

	if(item != -1) //a collectible was found
	{
		for(int i = 0; i < TANK_LIMIT; i++)
		{
			// if path already exists
			if(tanks.at(i).mCurrentState > 1)
			{
				// reset
				tanks.at(i).mCurrentState = 0;
				tanks.at(i).path2->clear();
			}
			// if no path yet
			else if(tanks.at(i).selected == true)
			{
				//Get hit location
				Ogre::Vector3 location = collectibles[item].position;

				// if hit the floor
				if((location.y < 0.001 && (selectionMode == 1 || selectionMode == 2)) || selectionMode == 0)
				{
					tanks.at(i).startNode = pathFindingGraph->getNode(tanks.at(i).tankNode->_getDerivedPosition());
					// set goal node
					if(selectionMode == 0)
					{
						do
						{
							tanks.at(i).goalNode = (int)rand() % 256;
						}while(pathFindingGraph->getContent(tanks.at(i).goalNode) != 0);
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

			pathFindingGraph->setContent(pathFindingGraph->getNode(tanks.at(i).tankNode->getPosition()), 2);
		}
	}

	//void collectibleCollected(collectible); //hide the collectible
}