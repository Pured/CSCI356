#include "stdafx.h"
#include "DemoApp.h"
//-------------------------------------------------------------------------------------
DemoApp::DemoApp(void)
{
	for(int i=0;i<TANK_LIMIT;i++)
		currentNode[i]=-1;
	camX=0;
	camY=0;
	camZ=45;
	cMode=0;
	lookAtDest=(0,0,0);
	srand(time(NULL));
	selectionMode=0;
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

	mTrayMgr->showCursor();

	pathFindingGraph = new Graph;
	for(int i=0;i<TANK_LIMIT;i++)
		mCurrentState[i] = 0;

	mDirection = Ogre::Vector3::ZERO;

	return true;
};

//-------------------------------------------------------------------------------------
void DemoApp::createScene(void)
{
	//mCamera->setPosition(Ogre::Vector3(0, 90, 100));
	//mCamera->lookAt(Ogre::Vector3(0, 0, 10));

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
 
	mapSetup();

	mWindow->setFullscreen(false,1280,720);
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
	tankMovement();
	frameRenderingCamera();


	mTrayMgr->frameRenderingQueued(evt);

	return true;
}
 void DemoApp::createFrameListener(void)
{
	BaseApplication::createFrameListener();
	tB = mTrayMgr->createTextBox(OgreBites::TL_TOPRIGHT, "InfoPanel", "Information", 500, 400);

}
// OIS::KeyListener
bool DemoApp::keyPressed( const OIS::KeyEvent &arg )
{
	camInput(arg);
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
			//camNode->setOrientation(Ogre::Quaternion());
			//mCamera->setOrientation(Ogre::Quaternion());
			//camNode->setOrientation(mCamera->getOrientation());
			//camNode->setPosition(mCamera->getPosition());
			//mCamera->setPosition(camNode->getPosition());
			//camNode->attachObject(mCamera);
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