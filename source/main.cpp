#include <stdlib.h>
#include <time.h>
#include "s3e.h"
#include "IwGx.h"
#include "Iw2D.h"
#include "ig2d/ig.h"
#include "scene_nag.h"
#include "scene_game.h"
#include "scene_game_menu.h"
#include "scene_menu.h"
#include "derbh.h"
#include "settings.h"
#include "sounds.h"
#include "game_data.h"
#include "achievements.h"
#include "config.h"

// attempt to lock frame rate to 30 fps
#define	MS_PER_FRAME (1000 / 30)

// airplay callbacks
int32 callbackPause(void* systemData, void* userData) {
	IGLog("PAUSED");
	/*// if the game is active on pause
	if(GameData::getInstance()->activeGame) {
		// switch scene to NULL and unload everything
		IGDirector::getInstance()->switchScene(NULL);
		IGScene::unloadResources();
		GameData::getInstance()->activeGame = true;
	}*/
	return 0;
}
int32 callbackUnpause(void* systemData, void* userData) {
	IGLog("UNPAUSED");
	/*// if the game is active on resume
	if(GameData::getInstance()->activeGame) {
		// switch back to game menu
		GameData::getInstance()->activeGame = false;
		IGDirector::getInstance()->switchScene(new SceneGameMenu());
	}*/
	return 0;
}

void gameInit() {
	// seed the random number generator
	srand(time(NULL));
	
	// init
	Iw2DInit();
	IwResManagerInit();
	//IwMemBucketDebugSetBreakpoint(11160);
	touchesInit();
	IGDistorter::getInstance()->set(320, 480);
	IGResourceManager::getInstance();

	// load archives
	if(dzArchiveAttach("sound.dz") == S3E_RESULT_ERROR)
		IGLog("Error attaching derbh archive sound.dz");
	else
		IGLog("Attached derbh archive sound.dz");
	
	// iphone, bada only need gles1
	if(AIRPLAY_DEVICE == AIRPLAY_DEVICE_BADA || AIRPLAY_DEVICE == AIRPLAY_DEVICE_IPHONE) {
		if(dzArchiveAttach("gles1.dz") == S3E_RESULT_ERROR)
			IGLog("Error attaching derbh archive gles1.dz");
	}
	// android needs gles1 and gles1-atitc
	else if(AIRPLAY_DEVICE == AIRPLAY_DEVICE_ANDROID) {
		char buildStyle[128];
		if(s3eConfigGetString("RESMANAGER", "ResBuildStyle", buildStyle) == S3E_RESULT_SUCCESS) {
			if((strcmp(buildStyle, "gles1") != 0) && strcmp(buildStyle, "gles1-atitc") != 0)
				sprintf(buildStyle, "gles1");
			
			char buffer[128];
			sprintf(buffer, "%s.dz", buildStyle);
			if(dzArchiveAttach(buffer) == S3E_RESULT_ERROR) {
				sprintf(buffer, "Error attaching derbh archive %s.dz", buildStyle);
				IGLog(buffer);
			} else {
				sprintf(buffer, "Attached derbh archive %s.dz", buildStyle);
				IGLog(buffer);
			}
		} else {
			IGLog("Error, no buildStyle");
		}
	}
	
	// load the rest of the game
	Settings::getInstance();
	GameData::getInstance();
	Achievements::getInstance();
	AchievementData::getInstance();
	Sounds::getInstance();

	// initialize callback functions
	s3eDeviceRegister(S3E_DEVICE_PAUSE, callbackPause, NULL);
	s3eDeviceRegister(S3E_DEVICE_UNPAUSE, callbackUnpause, NULL);
}

void gameStart() {
	// if this is the 7th time opening the game, pop up the nag screen
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
}

void gameShutdown() {
	// unload all extra resource groups
	IGScene::unloadResources();

	// stop the callbacks
	s3eDeviceUnRegister(S3E_DEVICE_PAUSE, callbackPause);
	s3eDeviceUnRegister(S3E_DEVICE_UNPAUSE, callbackUnpause);

	// shutdown everything else
	touchesShutdown();
	IGDirector::shutdown();
	IGDistorter::shutdown();
	IGResourceManager::shutdown();
	Settings::shutdown();
	Sounds::shutdown();
	GameData::shutdown();
	Achievements::shutdown();
	AchievementData::shutdown();
	IwResManagerTerminate();
	Iw2DTerminate();
}

void gameJustGenerateTextures() {
	// load all the textures and exit (to generate compressed ones)
	IwGetResManager()->LoadGroup("achievements.group");
	IwGetResManager()->LoadGroup("game.group");
	IwGetResManager()->LoadGroup("game_menu.group");
	IwGetResManager()->LoadGroup("instructions.group");
	IwGetResManager()->LoadGroup("map.group");
	IwGetResManager()->LoadGroup("menu.group");
	IwGetResManager()->LoadGroup("nag.group");
	IwGetResManager()->LoadGroup("options.group");
	IwGetResManager()->LoadGroup("select_level.group");
}

int main(int argc, char* argv[]) {
	// init
	gameInit();

	// uncomment to just generate textures and not load the game
	//gameJustGenerateTextures(); return 0;

	gameStart();
	
	// game loop
	int32 frame = 0;
	while(1) { 
		// let airplay do it's thing each frame
		s3eDeviceYield(0); 
		s3eKeyboardUpdate();
		s3ePointerUpdate();
		int64 start = s3eTimerGetMs();
		
		// update
		frame++;
		IGDirector::getInstance()->update();
		if(s3eDeviceCheckQuitRequest())
			break; 
		
		// render graphics
		IGDirector::getInstance()->display();
		Iw2DSurfaceShow();
		
		// attempt frame rate
		while((s3eTimerGetMs() - start) < MS_PER_FRAME) {
			int32 yield = (int32) (MS_PER_FRAME - (s3eTimerGetMs() - start));
			if(yield < 0)
				break;
			s3eDeviceYield(yield);
		}
	}
	
	// shutdown
	gameShutdown();
	return 0;
} 
