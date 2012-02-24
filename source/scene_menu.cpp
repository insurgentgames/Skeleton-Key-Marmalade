#include "scene_menu.h"
#include "IwResManager.h"
#include "s3eExt_OSExec.h"
#include "settings.h"
#include "sounds.h"
#include "scene_map.h"
#include "scene_instructions.h"
#include "scene_achievements.h"
#include "scene_options.h"

// play game button
MenuButtonPlayGame::MenuButtonPlayGame() {
	imageNormalName = std::string("menu_play_game");
	imageNormal = IGResourceManager::getInstance()->getImage(imageNormalName);
	imageSelectedName = std::string("menu_play_game2");
	imageSelected = IGResourceManager::getInstance()->getImage(imageSelectedName);
	image = imageNormal;
	this->set(IGPoint(160,228));
	touchSize = IGRect(320,56);
	z = 2;
	tag = 0;
}
void MenuButtonPlayGame::buttonPressed() {
	Sounds::getInstance()->playClick();
}
void MenuButtonPlayGame::buttonReleased() {
	parent->removeAllChildren();
	IGDirector::getInstance()->switchScene(new SceneMap());
}

// instructions button
MenuButtonInstructions::MenuButtonInstructions() {
	imageNormalName = std::string("menu_instructions");
	imageNormal = IGResourceManager::getInstance()->getImage(imageNormalName);
	imageSelectedName = std::string("menu_instructions2");
	imageSelected = IGResourceManager::getInstance()->getImage(imageSelectedName);
	image = imageNormal;
	this->set(IGPoint(160,284));
	touchSize = IGRect(320,56);
	z = 2;
	tag = 0;
}
void MenuButtonInstructions::buttonPressed() {
	Sounds::getInstance()->playClick();
}
void MenuButtonInstructions::buttonReleased() {
	parent->removeAllChildren();
	IGDirector::getInstance()->switchScene(new SceneInstructions());
}

// achievements button
MenuButtonAchievements::MenuButtonAchievements() {
	imageNormalName = std::string("menu_achievements");
	imageNormal = IGResourceManager::getInstance()->getImage(imageNormalName);
	imageSelectedName = std::string("menu_achievements2");
	imageSelected = IGResourceManager::getInstance()->getImage(imageSelectedName);
	image = imageNormal;
	this->set(IGPoint(160,340));
	touchSize = IGRect(320,56);
	z = 2;
	tag = 0;
}
void MenuButtonAchievements::buttonPressed() {
	Sounds::getInstance()->playClick();
}
void MenuButtonAchievements::buttonReleased() {
	parent->removeAllChildren();
	IGDirector::getInstance()->switchScene(new SceneAchievements());
}

// options button
MenuButtonOptions::MenuButtonOptions() {
	imageNormalName = std::string("menu_options");
	imageNormal = IGResourceManager::getInstance()->getImage(imageNormalName);
	imageSelectedName = std::string("menu_options2");
	imageSelected = IGResourceManager::getInstance()->getImage(imageSelectedName);
	image = imageNormal;
	this->set(IGPoint(160,396));
	touchSize = IGRect(320,56);
	z = 2;
	tag = 0;
}
void MenuButtonOptions::buttonPressed() {
	Sounds::getInstance()->playClick();
}
void MenuButtonOptions::buttonReleased() {
	GameData::getInstance()->optionsReturnsToGame = false;
	parent->removeAllChildren();
	IGDirector::getInstance()->switchScene(new SceneOptions());
}

// other games button
MenuButtonOtherGames::MenuButtonOtherGames() {
	imageNormalName = std::string("menu_other_games");
	imageNormal = IGResourceManager::getInstance()->getImage(imageNormalName);
	imageSelectedName = std::string("menu_other_games2");
	imageSelected = IGResourceManager::getInstance()->getImage(imageSelectedName);
	image = imageNormal;
	this->set(IGPoint(160,452));
	touchSize = IGRect(320,56);
	z = 2;
	tag = 0;
}
void MenuButtonOtherGames::buttonPressed() {
	Sounds::getInstance()->playClick();
}
void MenuButtonOtherGames::buttonReleased() {
	if(s3eExtOSExecAvailable() == S3E_TRUE)
		s3eOSExecExecute("http://othergames.insurgentgames.com/skeleton-key/", false);
}

// scene menu
SceneMenu::SceneMenu() {
	IGScene::IGScene();
	IGLog("SceneMenu init");

	// load the resources
	IwGetResManager()->LoadGroup("menu.group");
	
	// the background
	IGSprite* spriteBackground = new IGSprite("background_forest_light", IGPoint(160,240), 0);
	this->addChild(spriteBackground);

	// header
	IGSprite* spriteHeader = new IGSprite("menu_header", IGPoint(160,100), 1);
	this->addChild(spriteHeader);

	// the menu items
	MenuButtonPlayGame* buttonPlayGame = new MenuButtonPlayGame();
	MenuButtonInstructions* buttonInstructions = new MenuButtonInstructions();
	MenuButtonOptions* buttonOptions = new MenuButtonOptions();
	MenuButtonAchievements* buttonAchievements = new MenuButtonAchievements();
	MenuButtonOtherGames* buttonOtherGames = new MenuButtonOtherGames();
	this->addChild(buttonPlayGame);
	this->addChild(buttonInstructions);
	this->addChild(buttonOptions);
	this->addChild(buttonAchievements);
	this->addChild(buttonOtherGames);
}

SceneMenu::~SceneMenu() {
}
