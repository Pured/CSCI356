#include"stdafx.h"
#include "DemoApp.h"
void DemoApp::playerControlsQueued()
{
	if((playerControlled != -1))
	{
		//TURRET FOLLOW MOUSE (PLAYER CONTROLLED) works sometimes
		/*Ogre::Ray mouseRay = mCamera->getCameraToViewportRay(static_cast<float>(mMouse->getMouseState().X.abs)/mMouse->getMouseState().width,		static_cast<float>(mMouse->getMouseState().Y.abs)/mMouse->getMouseState().height);
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
			tanks.at(playerControlled).turretNode->yaw(Ogre::Radian(-90));
		}*/
		//DIRECT CONTROL OVER BARREL/TURRET
		// Rotate the tank turret
		tanks.at(playerControlled).turretNode->yaw(Ogre::Degree(tanks.at(playerControlled).mTurretRotate));
		// Calculate the tank barrel's current pitch
		tanks.at(playerControlled).mBarrelPitch += tanks.at(playerControlled).mBarrelRotate;
		// Clamp tank barrel rotation between 0 and 30 degrees
		if(tanks.at(playerControlled).mBarrelPitch > 30)
			tanks.at(playerControlled).mBarrelPitch = 30;
		else if(tanks.at(playerControlled).mBarrelPitch < 0)
			tanks.at(playerControlled).mBarrelPitch = 0;
		else
			tanks.at(playerControlled).barrelNode->roll(Ogre::Degree(-tanks.at(playerControlled).mBarrelRotate));
	}
}
