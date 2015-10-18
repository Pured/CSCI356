#include "stdafx.h"
#include "DemoApp.h"
//-------------------------------------------------------------------------------------
DemoApp::DemoApp(void)
{
	camX=0;
	camY=0;
	camZ=45;
	cMode=0;
	lookAtDest=(0,0,0);
	srand(time(NULL));
	selectionMode=1;
	boxTimeout=0;
	controlPressed=false;
	mousePressedVar=false;
	nextClickPath=false;

	playerControl = true;

	tankInfoWasOpen = false;
	controlsWasOpen = false;
	chatWasOpen = false;

	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
}
//-------------------------------------------------------------------------------------
DemoApp::~DemoApp(void)
{
	if(pathFindingGraph)
		delete pathFindingGraph;
}
 
bool DemoApp::setup(void)
{
	BaseApplication::setup();

	//set initial mouse position
	OIS::MouseState &mutableMouseState = const_cast<OIS::MouseState &>(mMouse->getMouseState());
	mutableMouseState.X.abs = mCamera->getViewport()->getActualWidth() / 2;
	mutableMouseState.Y.abs = mCamera->getViewport()->getActualHeight() / 2;

	mTrayMgr->showCursor();

	pathFindingGraph = new Graph;

	mDirection = Ogre::Vector3::ZERO;

	return true;
};

//-------------------------------------------------------------------------------------
void DemoApp::createScene(void)
{
	mCamera->setOrientation(Ogre::Quaternion());
	camNode=mSceneMgr->getRootSceneNode()->createChildSceneNode("Camera");
	camNode->setOrientation(Ogre::Quaternion());
	camNode->setPosition(Ogre::Vector3(0, 10, -10));
	camNode->lookAt(Ogre::Vector3(0, 10, 0),Ogre::Node::TS_PARENT,Ogre::Vector3::UNIT_Z);

	camNode->attachObject(mCamera);

	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.25, 0.25, 0.25));
 
    // create the light
    Ogre::Light *light = mSceneMgr->createLight("Light1");
    light->setType(Ogre::Light::LT_POINT);
    light->setPosition(Ogre::Vector3(250, 150, 250));
    light->setDiffuseColour(Ogre::ColourValue::White);
    light->setSpecularColour(Ogre::ColourValue::White);
 
	mSceneMgr->setSkyDome(true, "Examples/CloudySky", 5, 8); //sky dome

	mapSetup();

	mWindow->setFullscreen(false,1280,720);
	mWindow->reposition(150, 50);
}
bool DemoApp::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    if(mWindow->isClosed())
        return false;
 
	if(mShutDown)
		return false;
 
	//Need to capture/update each device
	mKeyboard->capture();
	mMouse->capture();
	tankMovement(evt);
	frameRenderingCamera();

	mTrayMgr->frameRenderingQueued(evt);

	return true;
}
 void DemoApp::createFrameListener(void)
{
	BaseApplication::createFrameListener();

	createUI();
}
// OIS::KeyListener
bool DemoApp::keyPressed( const OIS::KeyEvent &arg )
{
	keyInput(arg);

	return true;
}
bool DemoApp::keyReleased( const OIS::KeyEvent &arg )
{
	BaseApplication::keyReleased(arg);

	switch (arg.key)
	{
		case OIS::KC_LMENU: 
			cMode=0;
			lookAtDest=(0,0,0);
			mTrayMgr->showCursor();
			break;
		case::OIS::KC_LCONTROL:
			controlPressed=false;
			break;
	}
	return true;
}

// OIS::MouseListener
bool DemoApp::mouseMoved( const OIS::MouseEvent &arg )
{
	if (mTrayMgr->injectMouseMove(arg)) return true;
	camMovement(arg);
	return true;
}

bool DemoApp::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	if (mTrayMgr->injectMouseDown(arg, id)) return true;

	camPressed(arg,id);

	return true;
}
bool DemoApp::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	if (mTrayMgr->injectMouseDown(arg, id)) return true;

	camMouseReleased(arg,id);

	return true;
}

void DemoApp::createPath(Ogre::ManualObject* line, float height, std::vector<int>& path, Ogre::ColourValue& colour)
{
	line->clear();

	// Specify the material and rendering type
	line->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_STRIP);

	// Specify the vertices and vertex colour for the line
	Ogre::Vector3 position;

	for(std::vector<int>::iterator it=path.begin(); it!=path.end(); it++)
	{
		position = pathFindingGraph->getPosition(*it);
		line->position(Ogre::Vector3(position.x, height, position.z));
		line->colour(colour);
	}

	// Finished defining line
	line->end();
}

 
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif
 
#ifdef __cplusplus
extern "C" {
#endif
 
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
    int main(int argc, char *argv[])
#endif
    {
        // Create application object
        DemoApp app;
 
        try {
            app.go();
        } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occured: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }
 
        return 0;
    }
 
#ifdef __cplusplus
}
#endif