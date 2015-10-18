#ifndef __DemoApp_h_
#define __DemoApp_h_
 
#include "BaseApplication.h"
#include "PathFinding.h"
#include<math.h>
#include "PhysicsEngine.h"
#include"stdafx.h"
#include<vector>;
#define M_PI           3.14159265358979323846  /* pi */
//#define TANK_LIMIT           4  /* The number of tanks allowed in the game */

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

	//UI functions
	void createUI();
	void toggleOtherPanels();
 
	Ogre::Real mRotate;          // The rotate constant
	Ogre::Real mMove;            // The movement constant
 
	Ogre::Vector3 mDirection;     // Value to move in the correct direction

private:
	//SDKtrays functions
	
	int TANK_LIMIT;

	virtual void createFrameListener(void);

	//Map Setup Functions//
	void mapSetup();
	
	//CameraVariables//
	double camX;
	double camY;
	double camZ;
	Ogre::Radian R;
	Ogre::SceneNode* camNode;
	int cMode;
	Ogre::Vector3 lookAtDest;
	int selectionMode;
	Ogre::Degree mRotationX;
	Ogre::Degree mRotationY;

	//Camera Functions//
	void frameRenderingCamera();
	void camMovement( const OIS::MouseEvent & );
	void camPressed( const OIS::MouseEvent &, OIS::MouseButtonID  );
	bool DemoApp::WithinBounds(Ogre::Vector3 , Ogre::Vector3 );
	bool DemoApp::camMouseReleased( const OIS::MouseEvent &, OIS::MouseButtonID );

	//Selection Box Variables
	Ogre::Vector2 pos1;
	Ogre::Vector2 pos2;
	bool mSelecting;
	Ogre::ManualObject* mSelectionBox;
	int boxTimeout;
	bool controlPressed;
	bool mousePressedVar;
	bool nextClickPath;

	//SelectionBox Functions
	bool DemoApp::selectionBox();
	Ogre::ManualObject* DemoApp::createSelectionBox(Ogre::String );
	Ogre::Vector2 DemoApp::worldToScreenPosition(const Ogre::Vector3&);
	Ogre::Vector2 DemoApp::GetScreenspaceCoords(const Ogre::Vector3& , const Ogre::Camera& );
	bool DemoApp::quickSelect();

	//Keyboard Inputs
	void keyInput(const OIS::KeyEvent &);

	//UI Variables
	OgreBites::TextBox *controlsUI, *scoreUI, *chatBox;
	bool tankInfoWasOpen, controlsWasOpen, chatWasOpen;

	//AI Variables
	bool playerControl;

	//Lab Variables
	class tank {
	public:
		int health;
		int team;
		int mCurrentState;
		int startNode;
		int goalNode;
		bool selected;

		Ogre::ManualObject* path2;
		Ogre::SceneNode* tankNode;
		std::vector<int> tankPath;
		int currentNode;
		Ogre::Real mRotProgress; 
		bool firstTime;
		Ogre::Quaternion orientDest;
		Ogre::BillboardSet* mHealthBar;
		Ogre::BillboardSet* mSelectionCircle;
		Ogre::Radian originalRoll;
		tank()
		{
			selected=false;
			firstTime=true;
			mRotProgress=0;
			currentNode=-1;
			mCurrentState=0;
			team=-1;
			health=-1;
		}
	};

	std::vector<tank> tanks;
	/*int mCurrentState[TANK_LIMIT];
	int startNode[TANK_LIMIT];
	int goalNode[TANK_LIMIT];
	bool selected[TANK_LIMIT];*/

	Graph* pathFindingGraph;
	PathFinding mPathFinder;
	/*Ogre::ManualObject* path2[TANK_LIMIT];

	//Assignment Variables
	Ogre::SceneNode* tankNode[TANK_LIMIT];
	std::vector<int> tankPath[TANK_LIMIT];
	int currentNode[TANK_LIMIT];
	Ogre::Real mRotProgress[TANK_LIMIT]; 
	bool firstTime[TANK_LIMIT];
	Ogre::Quaternion orientDest[TANK_LIMIT];*/

	//TankMovementAI Functions//
	void createTank(int);
	void respawnTank(int);
	void findPath(int);
	void resetPath(int);
	void tankMovement(const Ogre::FrameEvent&);
	void createPath(Ogre::ManualObject* line, float height, std::vector<int>& path, Ogre::ColourValue& colour);
	void generatePath();
	bool WithinRotationBounds(tank );

	//AI functions
	void think(/*tank t*/); //AI does its own things
	void shutDown(/*tank t*/); //AI stops
	void changeState(int x, tank t); //x = state to change to

	//Selection BillBoard variables
	//Ogre::BillboardSet* mHealthBar[TANK_LIMIT];
	Ogre::Billboard* mHealthBarBB;
	//Ogre::BillboardSet* mSelectionCircle[TANK_LIMIT];
	Ogre::Billboard* mSelectionCircleBB;
};
 
#endif // #ifndef __DemoApp_h_