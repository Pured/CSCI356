#ifndef __DemoApp_h_
#define __DemoApp_h_
 
#include"stdafx.h"
#include "BaseApplication.h"
#include "PathFinding.h"
#include<math.h>
#include "PhysicsEngine.h"
#include<vector>
#include <irrKlang.h>
#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll
#define M_PI           3.14159265358979323846  /* pi */
//#define TANK_LIMIT           4  /* The number of tanks allowed in the game */

const int MAX_SHOTS = 10;
const double SHOT_VELOCITY = 100.0f;
const double SHOT_GRAVITY = 5.0f;
const int SHOT_RANGE = 15;
const int SHOT_DAMAGE = 10;
const int HP_SPAWNTIME = 60;
const int TROPHY_SPAWNTIME = 30;
const int NUMCOLLECTIBLES = 3;

class DemoApp : public BaseApplication
{
	public:
		DemoApp(void);
		virtual ~DemoApp(void);
 
	protected:
		virtual bool setup(void);
		virtual void createScene(void);
 
		// Ogre::FrameListener
		virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
		// OIS::KeyListener
		virtual bool keyPressed( const OIS::KeyEvent &arg );
		virtual bool keyReleased( const OIS::KeyEvent &arg );
		// OIS::MouseListener
		virtual bool mouseMoved( const OIS::MouseEvent &arg );
		virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
		virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

		// UI functions
		void createUI();
		void toggleOtherPanels();
		void updateScoreboard();
 
		Ogre::Real mRotate;          // The rotate constant
		Ogre::Real mMove;            // The movement constant
 
		Ogre::Vector3 mDirection;     // Value to move in the correct direction

		// Ground node for any objects to be attached to
		Ogre::SceneNode* mGroundNode;

		// For physics
		PhysicsEngine* mPhysicsEngine;

		// Function to shoot
		void shoot();

	private:
		// Projectile variables
		btCollisionObject* projectile(const btVector3& position, const btQuaternion& orientation, 
				const btVector3& linearVelocity, Ogre::String entityName);
		btCollisionObject* mProjectiles[MAX_SHOTS];
		btVector3 shotOrigin;
		void clearShots(btRigidBody* shot);
		int mTotalShots;

		// Shot variables
		double THETA;
		double DISTANCE;
		Ogre::Vector3 posVec;
		Ogre::Vector3 posVec2;
		Ogre::Vector3 oriVec;

		int TANK_LIMIT;

		virtual void createFrameListener(void);

		// Map Setup Functions
		void mapSetup();
	
		// CameraVariables
		double camX;
		double camY;
		double camZ;
		Ogre::Radian R;
		Ogre::SceneNode* camNode;
		Ogre::Vector3 lookAtDest;
		int selectionMode;
		Ogre::Degree mRotationX;
		Ogre::Degree mRotationY;

		// Camera Functions
		void frameRenderingCamera();
		void camInput(const OIS::KeyEvent &);
		void camMovement( const OIS::MouseEvent & );
		void camPressed( const OIS::MouseEvent &, OIS::MouseButtonID  );
		bool DemoApp::WithinBounds(Ogre::Vector3 , Ogre::Vector3 );
		bool DemoApp::camMouseReleased( const OIS::MouseEvent &, OIS::MouseButtonID );

		// Selection Box Variables
		Ogre::Vector2 pos1;
		Ogre::Vector2 pos2;
		bool mSelecting;
		Ogre::ManualObject* mSelectionBox;
		int boxTimeout;
		bool controlPressed;
		bool mousePressedVar;
		bool nextClickPath;

		// SelectionBox Functions
		bool DemoApp::selectionBox();
		Ogre::ManualObject* DemoApp::createSelectionBox(Ogre::String );
		Ogre::Vector2 DemoApp::worldToScreenPosition(const Ogre::Vector3&);
		Ogre::Vector2 DemoApp::GetScreenspaceCoords(const Ogre::Vector3& , const Ogre::Camera& );
		bool DemoApp::quickSelect();

		//Keyboard Inputs
		void keyInput(const OIS::KeyEvent &arg);
		char getInputChar(const OIS::KeyEvent &c);

		//UI Variables
		OgreBites::TextBox *controlsUI, *scoreUI, *chatBox;
		bool tankInfoWasOpen, controlsWasOpen, chatWasOpen;

		//AI Variables
		int playerControlled, prevPlayerControlled; //-1 = AI, 0+ = tank ID

		int namesAllocated; //what name to give out next
		std::string nameList[10]; //list of names for tanks

		//Collectible Class
		class Collectible
		{
			public:
				Collectible(int index, int type, Ogre::SceneManager *mSceneMgr, Graph *pathFindingGraph); //sets default values
				
				Ogre::Entity *collectibleItem;
				Ogre::SceneNode *collectibleNode;
				Ogre::Vector3 position;

				bool isVisible;
				float timeTillSpawn;
				int collectibleType; //3 = hp, 4 = trophy
		};

		int visibleCollectibles;

		std::vector<Collectible> collectibles;

		void collectibleCollected(Collectible collectible);
		void spawnCollectible(Collectible collectible);

		//Tank Class
		class tank 
		{
			public:
				int health;
				int team;
				int mCurrentState;
				int startNode;
				int goalNode;
				bool selected;
				int currentState; //AI state
				int score; //how many points obtained
				std::string name;

				//Player Controlled tank aiming
				float mTurretRotate;
				float mBarrelRotate;
				float mBarrelPitch;

				Ogre::ManualObject* path2;
				Ogre::SceneNode* tankNode;
				Ogre::SceneNode* turretNode;
				Ogre::SceneNode* barrelNode;
				std::vector<int> tankPath;
				int currentNode;
				Ogre::Real mRotProgress; 
				bool firstTime;
				Ogre::Quaternion orientDest;
				Ogre::BillboardSet* mHealthBar;
				Ogre::BillboardSet* mSelectionCircle;
				Ogre::Radian originalRoll;

				int tankInVec;

				tank *enemy;

				tank()
				{
					score = 0;
					currentState = -1; //AI state
					selected = false;
					firstTime = true;
					mRotProgress = 0;
					currentNode = -1;
					mCurrentState = 0;
					team = -1;
					health = -1;
					enemy = NULL;
					mTurretRotate = 0;
					mBarrelRotate = 0;
					mBarrelPitch = 0;
					tankInVec = 0;
				}
		};

		std::vector<tank> tanks;

		void fire(tank t, tank ta, const Ogre::FrameEvent& evt);
		void findXY(Ogre::Real& x, Ogre::Real& y, const Ogre::FrameEvent& evt, double theta);

		/*int mCurrentState[TANK_LIMIT];
		int startNode[TANK_LIMIT];
		int goalNode[TANK_LIMIT];
		bool selected[TANK_LIMIT];*/

		Graph* pathFindingGraph;
		PathFinding mPathFinder;
		/*Ogre::ManualObject* path2[TANK_LIMIT];

		// Assignment Variables
		Ogre::SceneNode* tankNode[TANK_LIMIT];
		std::vector<int> tankPath[TANK_LIMIT];
		int currentNode[TANK_LIMIT];
		Ogre::Real mRotProgress[TANK_LIMIT]; 
		bool firstTime[TANK_LIMIT];
		Ogre::Quaternion orientDest[TANK_LIMIT];*/

		// TankMovementAI Functions
		void createTank(int);
		void respawnTank(int);
		void findPath(int);
		void resetPath(int);
		void tankMovement(const Ogre::FrameEvent&);
		void createPath(Ogre::ManualObject* line, float height, std::vector<int>& path, Ogre::ColourValue& colour);
		void generatePath();
		bool WithinRotationBounds(tank );
		void destroyTank(int );

		// AI functions
		void think(tank t); //AI does its own things
		void shutDown(tank t); //AI stops
		void changeState(int x, tank t); //x = state to change to

		bool isEnemyVisible(tank t); //check if there are any visible enemies
		tank getVisibleEnemy(tank t); //sets t's enemy as the enemy tank found
		void fight(tank t);

		void collectCollectible(tank t);

		// Selection BillBoard variables
		//Ogre::BillboardSet* mHealthBar[TANK_LIMIT];
		Ogre::Billboard* mHealthBarBB;
		//Ogre::BillboardSet* mSelectionCircle[TANK_LIMIT];
		Ogre::Billboard* mSelectionCircleBB;

		//Controls Functions//
		void playerControlsQueued();

		bool once;

		irrklang::ISoundEngine* soundEngine;
};
 
#endif // #ifndef __DemoApp_h_