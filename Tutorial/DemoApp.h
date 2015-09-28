#ifndef __DemoApp_h_
#define __DemoApp_h_
 
#include "BaseApplication.h"
#include "PathFinding.h"
#include<math.h>
#define M_PI           3.14159265358979323846  /* pi */
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
 
	Ogre::Real mRotate;          // The rotate constant
	Ogre::Real mMove;            // The movement constant
 
	Ogre::Vector3 mDirection;     // Value to move in the correct direction

private:

	//Contains debug TextBox variables
	OgreBites::TextBox* tB;
	//SDKtrays functions
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

	//Camera Functions//
	void frameRenderingCamera();
	void camInput(const OIS::KeyEvent &);
	void camMovement( const OIS::MouseEvent & );
	void camPressed( const OIS::MouseEvent &, OIS::MouseButtonID  );
	bool DemoApp::WithinBounds(Ogre::Vector3 , Ogre::Vector3 );

	//TankMovementAI Variables//
	//Lab Variables
	int mCurrentState;
	int startNode;
	int goalNode;

	Graph* pathFindingGraph;
	PathFinding mPathFinder;
	Ogre::ManualObject* path1;
	Ogre::ManualObject* path2;

	//Assignment Variables
	Ogre::SceneNode* tankNode;
	std::vector<int> tankPath;
	int currentNode;

	//TankMovementAI Functions//
	void tankMovement();
	void createPath(Ogre::ManualObject* line, float height, std::vector<int>& path, Ogre::ColourValue& colour);
};
 
#endif // #ifndef __DemoApp_h_