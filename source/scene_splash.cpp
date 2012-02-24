#include "scene_splash.h"
#include "IwResManager.h"
#include "scene_menu.h"
#include "scene_game_menu.h"
#include "scene_nag.h"
#include "game_data.h"
#include "settings.h"

// splash image
SplashImage::SplashImage() {
	imageName = std::string("splash");
	image = IGResourceManager::getInstance()->getImage(imageName);
	this->set(IGPoint(160,240));
	z = 0;
	tag = 0;
}

// splash scene
SceneSplash::SceneSplash() {
	IGScene::IGScene();
	IGLog("SceneSplash init");

	// load the resources
	IwGetResManager()->LoadGroup("splash.group");

	// if using an iphone, skip the wait and just fade out
	if(s3eDeviceGetInt(S3E_DEVICE_OS) == S3E_OS_ID_IPHONE)
		stage = 1;
	else
		stage = 0;
	start = s3eTimerGetMs();

	// background
	IGSprite* spriteBackground = new IGSprite("background_black", IGPoint(160,240));
	this->addChild(spriteBackground);
	
	// slash image
	splashImage = new SplashImage();
	this->addChild(splashImage);

	// increment the load counter
	Settings::getInstance()->loadCount++;
	Settings::getInstance()->save();
}

SceneSplash::~SceneSplash() {
}

void SceneSplash::update() {
	switch(stage) {
		case 0: // waiting
			if(s3eTimerGetMs() - start >= 2000) { // 2 second
				stage = 1;
				start = s3eTimerGetMs();
				fadeCount = 0;
			}
			break;
		case 1: // fading
			if(s3eTimerGetMs() - start >= 10) { // .01 seconds
				start = s3eTimerGetMs();
				fadeCount++;
				splashImage->setOpacity((uint8)((10-fadeCount)*(float)(255.0/10.0)));
			}
			if(fadeCount >= 10)
				stage = 2;
			break;
		case 2: // switching scenes
			removeAllChildren();
			
			// if this is the 5th time opening the game, pop up the nag screen
			if(Settings::getInstance()->loadCount == 7) {
				// go to the nag scene
				IGDirector::getInstance()->switchScene(new SceneNag());
			} else {
				// if there's a game in progress, go to game menu
				if(GameData::getInstance()->loadGame()) {
					IGDirector::getInstance()->switchScene(new SceneGameMenu());
				}
				// otherwise go to menu
				else {
					IGDirector::getInstance()->switchScene(new SceneMenu());
				}
			}
			break;
	}
}
