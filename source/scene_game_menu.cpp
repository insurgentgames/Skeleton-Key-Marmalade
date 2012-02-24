#include "scene_game_menu.h"
#include "IwResManager.h"
#include "settings.h"
#include "sounds.h"
#include "game_data.h"
#include "scene_game.h"
#include "scene_menu.h"
#include "scene_options.h"

// resume button
GameMenuButtonResume::GameMenuButtonResume() {
	imageNormalName = std::string("game_menu_resume");
	imageNormal = IGResourceManager::getInstance()->getImage(imageNormalName);
	imageSelectedName = std::string("game_menu_resume2");
	imageSelected = IGResourceManager::getInstance()->getImage(imageSelectedName);
	image = imageNormal;
	this->set(IGPoint(160,196));
	touchSize = IGRect(320,50);
	z = 2;
	tag = 0;
}
void GameMenuButtonResume::buttonPressed() {
	Sounds::getInstance()->playClick();
}
void GameMenuButtonResume::buttonReleased() {
	GameData::getInstance()->loadGame();
	parent->removeAllChildren();
	IGDirector::getInstance()->switchScene(new SceneGame());
}

// restart button
GameMenuButtonRestart::GameMenuButtonRestart() {
	imageNormalName = std::string("game_menu_restart");
	imageNormal = IGResourceManager::getInstance()->getImage(imageNormalName);
	imageSelectedName = std::string("game_menu_restart2");
	imageSelected = IGResourceManager::getInstance()->getImage(imageSelectedName);
	image = imageNormal;
	this->set(IGPoint(160,246));
	touchSize = IGRect(320,50);
	z = 2;
	tag = 0;
}
void GameMenuButtonRestart::buttonPressed() {
	Sounds::getInstance()->playClick();
}
void GameMenuButtonRestart::buttonReleased() {
	Sounds::getInstance()->playRestartLevel();
	GameData::getInstance()->loadLevel();
	GameData::getInstance()->saveGame();
	parent->removeAllChildren();
	IGDirector::getInstance()->switchScene(new SceneGame());
}

// next button
GameMenuButtonNext::GameMenuButtonNext() {
	imageNormalName = std::string("game_menu_next");
	imageNormal = IGResourceManager::getInstance()->getImage(imageNormalName);
	imageSelectedName = std::string("game_menu_next2");
	imageSelected = IGResourceManager::getInstance()->getImage(imageSelectedName);
	image = imageNormal;
	this->set(IGPoint(160,296));
	touchSize = IGRect(320,50);
	z = 2;
	tag = 0;
}
void GameMenuButtonNext::buttonPressed() {
	Sounds::getInstance()->playClick();
}
void GameMenuButtonNext::buttonReleased() {
	int level = GameData::getInstance()->level;
	int stage = GameData::getInstance()->stageForLevel(level);
	
	// last level?
	if(level == 120) {
		// give error about last level
		((SceneGameMenu*)parent)->errorMessageLastLevel();
		return;
	}

	if(stage == GameData::getInstance()->stageForLevel(level+1)) {
		// this level and next level are both in the same stage, so switch to next level
		GameData::getInstance()->level++;
	} else {
		// this level and next are in different stages
		if(GameData::getInstance()->stageLocked(stage+1)) {
			// give error about stage locked
			((SceneGameMenu*)parent)->errorMessageStageLocked();
			return;
		} else {
			// but the next stage is unlocked, so switch to next level
			level++;
		}
	}
	
	GameData::getInstance()->loadLevel();
	GameData::getInstance()->saveGame();
	parent->removeAllChildren();
	IGDirector::getInstance()->switchScene(new SceneGame());
}

// options button
GameMenuButtonOptions::GameMenuButtonOptions() {
	imageNormalName = std::string("game_menu_options");
	imageNormal = IGResourceManager::getInstance()->getImage(imageNormalName);
	imageSelectedName = std::string("game_menu_options2");
	imageSelected = IGResourceManager::getInstance()->getImage(imageSelectedName);
	image = imageNormal;
	this->set(IGPoint(160,376));
	touchSize = IGRect(320,50);
	z = 2;
	tag = 0;
}
void GameMenuButtonOptions::buttonPressed() {
	Sounds::getInstance()->playClick();
}
void GameMenuButtonOptions::buttonReleased() {
	GameData::getInstance()->optionsReturnsToGame = true;
	GameData::getInstance()->loadGame();
	parent->removeAllChildren();
	IGDirector::getInstance()->switchScene(new SceneOptions());
}

// abandon button
GameMenuButtonAbandon::GameMenuButtonAbandon() {
	imageNormalName = std::string("game_menu_abandon");
	imageNormal = IGResourceManager::getInstance()->getImage(imageNormalName);
	imageSelectedName = std::string("game_menu_abandon2");
	imageSelected = IGResourceManager::getInstance()->getImage(imageSelectedName);
	image = imageNormal;
	this->set(IGPoint(160,426));
	touchSize = IGRect(320,50);
	z = 2;
	tag = 0;
}
void GameMenuButtonAbandon::buttonPressed() {
	Sounds::getInstance()->playClick();
}
void GameMenuButtonAbandon::buttonReleased() {
	// delete saved game
	SQLite3Wrapper db("saved_game.db");
	db.exe(std::string("DELETE FROM saved_game;"));

	// change music
	Sounds::getInstance()->startMusicMenu();

	// go to menu
	parent->removeAllChildren();
	IGDirector::getInstance()->switchScene(new SceneMenu());
}

// error continue button
GameMenuButtonErrorContinue::GameMenuButtonErrorContinue() {
	imageNormalName = std::string("game_menu_error_continue");
	imageNormal = IGResourceManager::getInstance()->getImage(imageNormalName);
	imageSelectedName = std::string("game_menu_error_continue2");
	imageSelected = IGResourceManager::getInstance()->getImage(imageSelectedName);
	image = imageNormal;
	this->set(IGPoint(160,332));
	touchSize = IGRect(320,50);
	z = 7;
	tag = GameMenuTagErrorContinue;
}
void GameMenuButtonErrorContinue::buttonPressed() {
	Sounds::getInstance()->playClick();
}
void GameMenuButtonErrorContinue::buttonReleased() {
	((SceneGameMenu*)parent)->errorContinue();
}

// scene game menu
SceneGameMenu::SceneGameMenu() {
	IGScene::IGScene();
	IGLog("SceneGameMenu init");
	errorUp = false;

	// load the resources
	IwGetResManager()->LoadGroup("game_menu.group");

	// the background
	IGSprite* spriteBackground = new IGSprite("background_forest_light", IGPoint(160,240), 0);
	this->addChild(spriteBackground);

	// header
	IGSprite* spriteHeader = new IGSprite("game_menu_header", IGPoint(160,60), 1);
	this->addChild(spriteHeader);

	// the menu items
	GameMenuButtonResume* buttonResume = new GameMenuButtonResume();
	GameMenuButtonRestart* buttonRestart = new GameMenuButtonRestart();
	GameMenuButtonNext* buttonNext = new GameMenuButtonNext();
	GameMenuButtonOptions* buttonOptions = new GameMenuButtonOptions();
	GameMenuButtonAbandon* buttonAbandon = new GameMenuButtonAbandon();
	this->addChild(buttonResume);
	this->addChild(buttonRestart);
	this->addChild(buttonNext);
	this->addChild(buttonOptions);
	this->addChild(buttonAbandon);

	// game info
	IGSprite* spriteInfoBackground = new IGSprite("game_menu_info_background", IGPoint(160,145), 2);
	this->addChild(spriteInfoBackground);
	char buffer1[50];
	char buffer2[100];
	switch(GameData::getInstance()->stage) {
		case GameStageForest: sprintf(buffer1, "Forest"); break;
		case GameStageCaves: sprintf(buffer1, "Caves"); break;
		case GameStageBeach: sprintf(buffer1, "Beach"); break;
		case GameStageShip: sprintf(buffer1, "Pirate Ship"); break;
	}
	sprintf(buffer2, "%s * Level: %i", buffer1, GameData::getInstance()->level);
	IGLabel* labelInfo = new IGLabel("font_algerian_16", std::string(buffer2), IGPoint(160,145), IGRect(300,50), 3);
	labelInfo->setColor(147,213,18,255);
	this->addChild(labelInfo);
}

SceneGameMenu::~SceneGameMenu() {
}

void SceneGameMenu::errorMessageStart() {
	IGSprite* spriteBlackBackground = new IGSprite("background_black", IGPoint(160,240), 5, GameMenuTagErrorBlackBackground);
	spriteBlackBackground->setOpacity(192);
	IGSprite* spriteErrorBackground = new IGSprite("game_menu_error_background", IGPoint(160,280), 6, GameMenuTagErrorBackground);
	GameMenuButtonErrorContinue* buttonContinue = new GameMenuButtonErrorContinue();
	this->addChild(spriteBlackBackground);
	this->addChild(spriteErrorBackground);
	this->addChild(buttonContinue);
}

void SceneGameMenu::errorMessageLastLevel() {
	if(errorUp)
		return;
	errorUp = true;
	
	// add error dialog
	errorMessageStart();
	IGSprite* spriteMessage = new IGSprite("game_menu_error_message_last_level", IGPoint(160,249), 7, GameMenuTagErrorMessage);
	this->addChild(spriteMessage);
}

void SceneGameMenu::errorMessageStageLocked() {
	if(errorUp)
		return;
	errorUp = true;
	
	// add error dialog
	errorMessageStart();
	IGSprite* spriteMessage = new IGSprite("game_menu_error_message_stage_locked", IGPoint(160,249), 7, GameMenuTagErrorMessage);
	this->addChild(spriteMessage);
}

void SceneGameMenu::errorContinue() {
	this->removeChildByTag(GameMenuTagErrorBlackBackground);
	this->removeChildByTag(GameMenuTagErrorBackground);
	this->removeChildByTag(GameMenuTagErrorMessage);
	this->removeChildByTag(GameMenuTagErrorContinue);
	errorUp = false;
}

bool SceneGameMenu::touch(s3ePointerTouchEvent* event) {
	// if error message is up, only handle that touch
	if(errorUp) {
		GameMenuButtonErrorContinue* buttonContinue = (GameMenuButtonErrorContinue*)this->getChildByTag(GameMenuTagErrorContinue);
		return buttonContinue->touch(event);
	}
	// otherwise handle all other touches
	else {
		return IGNode::touch(event);
	}
}
