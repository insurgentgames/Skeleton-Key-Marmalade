#include "scene_nag.h"
#include "IwResManager.h"
#include "s3eExt_OSExec.h"
#include "IwResManager.h"
#include "scene_menu.h"
#include "scene_game_menu.h"
#include "game_data.h"
#include "settings.h"
#include "sounds.h"

// yes button
NagButtonYes::NagButtonYes() {
	imageNormalName = std::string("nag_yes");
	imageNormal = IGResourceManager::getInstance()->getImage(imageNormalName);
	imageSelectedName = std::string("nag_yes2");
	imageSelected = IGResourceManager::getInstance()->getImage(imageSelectedName);
	image = imageNormal;
	this->set(IGPoint(160,340));
	touchSize = IGRect(320,56);
	z = 2;
	tag = 0;
}
void NagButtonYes::buttonPressed() {
	Sounds::getInstance()->playClick();
}
void NagButtonYes::buttonReleased() {
	if(s3eExtOSExecAvailable() == S3E_TRUE) {
		s3eOSExecExecute("http://download.insurgentgames.com/skeleton-key/", false);
		/*// if iphone, redirect to app store
		if(s3eDeviceGetInt(S3E_DEVICE_OS) == S3E_OS_ID_IPHONE)
			s3eOSExecExecute("http://download.insurgentgames.com/skeleton-key/", false);
		// if android, redirect to market
		else
			s3eOSExecExecute("http://download.insurgentgames.com/skeleton-key/", false);*/
	}
	((SceneNag*)this->parent)->ontoTheGame();
}

// no button
NagButtonNo::NagButtonNo() {
	imageNormalName = std::string("nag_no");
	imageNormal = IGResourceManager::getInstance()->getImage(imageNormalName);
	imageSelectedName = std::string("nag_no2");
	imageSelected = IGResourceManager::getInstance()->getImage(imageSelectedName);
	image = imageNormal;
	this->set(IGPoint(160,396));
	touchSize = IGRect(320,56);
	z = 2;
	tag = 0;
}
void NagButtonNo::buttonPressed() {
	Sounds::getInstance()->playClick();
}
void NagButtonNo::buttonReleased() {
	((SceneNag*)this->parent)->ontoTheGame();
}

// scene nag
SceneNag::SceneNag() {
	IGScene::IGScene();
	IGLog("SceneNag init");

	// load the resources
	IwGetResManager()->LoadGroup("nag.group");
	
	// the background
	IGSprite* spriteBackground = new IGSprite("background_forest_dark", IGPoint(160,240), 0);
	this->addChild(spriteBackground);

	// header
	IGSprite* spriteHeader = new IGSprite("nag_header", IGPoint(160,128), 1);
	this->addChild(spriteHeader);

	// the menu items
	NagButtonYes* buttonYes = new NagButtonYes();
	NagButtonNo* buttonNo = new NagButtonNo();
	this->addChild(buttonYes);
	this->addChild(buttonNo);
}

SceneNag::~SceneNag() {
}

void SceneNag::ontoTheGame() {
	this->removeAllChildren();

	// if there's a game in progress, go to game menu
	if(GameData::getInstance()->loadGame()) {
		IGDirector::getInstance()->switchScene(new SceneGameMenu());
	}
	// otherwise go to menu
	else {
		IGDirector::getInstance()->switchScene(new SceneMenu());
	}
}