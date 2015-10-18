#include "stdafx.h"
#include "DemoApp.h"

void DemoApp::createUI()
{
	Ogre::StringVector tankInfo;
	tankInfo.push_back("Tank ID");
	tankInfo.push_back("Team");
	tankInfo.push_back("HP");
	tankInfo.push_back("Armour");
	tankInfo.push_back("Power-up");
	tankInfo.push_back("State");
	
	tankPanel = mTrayMgr->createParamsPanel(OgreBites::TL_TOPLEFT, "TankPanel", 250, tankInfo);
	tankPanel->setParamValue(0, "???");
	tankPanel->setParamValue(1, "???");
	tankPanel->setParamValue(2, "???");
	tankPanel->setParamValue(3, "???");
	tankPanel->setParamValue(4, "???");
	tankPanel->setParamValue(5, "???");

	controlsUI = mTrayMgr->createTextBox(OgreBites::TL_TOPRIGHT, "ControlsPanel", "Controls", 400, 500);
	scoreUI = mTrayMgr->createTextBox(OgreBites::TL_CENTER, "ScorePanel", "Scoreboard", 600, 600);
	chatBox = mTrayMgr->createTextBox(OgreBites::TL_BOTTOMLEFT, "ChatPanel", "Player Chat", 600, 250);
	
	controlsUI->setText("Mouse:\n");
	controlsUI->appendText("Left click (on tank): Selects the tank.\n");
	controlsUI->appendText("Left click (on ground): Deselects all tanks.\n");
	controlsUI->appendText("Left click (hold + drag): Selects multiple tanks.\n");
	controlsUI->appendText("Right click: Moves the tank.\n");
	controlsUI->appendText("Move (edge of screen): Moves the camera.\n");
	controlsUI->appendText("\nKeyboard:\n");
	controlsUI->appendText("A: Toggles AI on/off.\n");
	controlsUI->appendText("C: Shows/hides the controls.\n");
	controlsUI->appendText("F: NO IDEA WHAT THIS IS FOR!\n");
	controlsUI->appendText("H: Show/hide tank HP bars.\n");
	controlsUI->appendText("P: Prints the map to the console.\n");
	controlsUI->appendText("Q: Toggles special bullet on/off.\n");
	controlsUI->appendText("R: Resets the camera to the middle of the field.\n");
	controlsUI->appendText("T: Show/hide tanks' information.\n");
	controlsUI->appendText("W: Use power-up.\n");
	controlsUI->appendText("Space (hold + tank selected): Charges tank's shooting power.\n");
	controlsUI->appendText("Tab: Show/hide scoreboard.\n");
	controlsUI->appendText("Backspace (tank selected): Force suicide.\n");
	controlsUI->appendText("LShift (hold + tank selected): Toggle nitros.\n");
	controlsUI->appendText("LControl: IS THIS NEEDED?.\n");
	controlsUI->appendText("LMenu: WHAT BUTTON IS THIS? IS IT NEEDED?.\n");
	controlsUI->appendText("Enter: Chat.\n");
	controlsUI->appendText("Escape: Exit the game.");

//	mTrayMgr->getTrayContainer(OgreBites::TL_TOPLEFT)->hide(); //hides the tank info panel by default
	mTrayMgr->getTrayContainer(OgreBites::TL_TOPRIGHT)->hide(); //hides the controls by default
	mTrayMgr->getTrayContainer(OgreBites::TL_CENTER)->hide(); //hides the scoreboard by default
	mTrayMgr->getTrayContainer(OgreBites::TL_BOTTOMLEFT)->hide(); //hides the chatbox by default
}

void DemoApp::toggleOtherPanels() //show/hide the other UI panels when toggling the scoreboard
{
	if(mTrayMgr->getTrayContainer(OgreBites::TL_CENTER)->isVisible() == false) //hide other panels
	{
		if(tankInfoWasOpen == true)
			tankPanel->hide();

		if(controlsWasOpen == true)
			mTrayMgr->getTrayContainer(OgreBites::TL_TOPRIGHT)->hide();
	}
	else //show other panels that were open before
	{
		if(tankInfoWasOpen == true)
			tankPanel->show();

		if(controlsWasOpen == true)
			mTrayMgr->getTrayContainer(OgreBites::TL_TOPRIGHT)->show();
	}
}