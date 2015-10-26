#include"stdafx.h"
#include "DemoApp.h"
void DemoApp::playerControlsQueued()
{
	if((playerControlled != -1))
	{
		Ogre::Ray mouseRay = mCamera->getCameraToViewportRay(static_cast<float>(mMouse->getMouseState().X.abs)/mMouse->getMouseState().width,		static_cast<float>(mMouse->getMouseState().Y.abs)/mMouse->getMouseState().height);
		Ogre::RaySceneQuery * mRaySceneQuery = mSceneMgr->createRayQuery(Ogre::Ray());
		mRaySceneQuery->setRay(mouseRay);
		mRaySceneQuery->setQueryTypeMask(Ogre::SceneManager::ENTITY_TYPE_MASK);
		mRaySceneQuery->setSortByDistance(true);
		// Ray-cast and get first hit
		Ogre::RaySceneQueryResult &result = mRaySceneQuery->execute();
		Ogre::RaySceneQueryResult::iterator itr = result.begin();
		if(itr != result.end())
		{
			// Get hit location
			Ogre::Vector3 location = mouseRay.getPoint(itr->distance);
			tanks.at(playerControlled).turretNode->lookAt(Ogre::Vector3(location.x, 0, location.z), Ogre::Node::TS_WORLD);
			
			Ogre::Vector3 localY = tanks.at(playerControlled).turretNode->getOrientation() * Ogre::Vector3::UNIT_Y;
			Ogre::Quaternion quat = localY.getRotationTo(Ogre::Vector3::UNIT_Y);                        
			tanks.at(playerControlled).turretNode->rotate(quat, Ogre::Node::TS_PARENT);

			/*Ogre::Vector3 mDestination = mouseRay.getPoint(itr->distance); // mDestination is the next location
			Ogre::Vector3 mDirection = mDestination - tanks.at(playerControlled).turretNode->getPosition();     // B-A = A->B (see vector questions above)
			Ogre::Vector3 src = tanks.at(playerControlled).turretNode->getOrientation() * Ogre::Vector3::UNIT_X;      // Orientation from initial direction
			src.y = 0;                                                    // Ignore pitch difference angle
			mDirection.y = 0;
			src.normalise();
			Ogre::Real mDistance = mDirection.normalise( );                     // Both vectors modified so renormalize them
			Ogre::Quaternion quat = src.getRotationTo(mDirection);
			tanks.at(playerControlled).turretNode->rotate(quat);*/

		}
	}
	//ray cast
	//lookAt x,z pos (y not needed for turret)
	//Barrel auto adjusted(?)
}
