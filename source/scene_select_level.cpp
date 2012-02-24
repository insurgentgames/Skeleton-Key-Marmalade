#include "scene_select_level.h"
#include "IwResManager.h"
#include "settings.h"
#include "sounds.h"
#include "scene_menu.h"
#include "scene_game.h"
#include "game_data.h"

// back button
SelectLevelButtonBack::SelectLevelButtonBack() {
	imageNormalName = std::string("select_level_back");
	imageNormal = IGResourceManager::getInstance()->getImage(imageNormalName);
	imageSelectedName = std::string("select_level_back2");
	imageSelected = IGResourceManager::getInstance()->getImage(imageSelectedName);
	image = imageNormal;
	this->set(IGPoint(32,35));
	touchSize = IGRect(63,70);
	z = 2;
	tag = 0;
}
void SelectLevelButtonBack::buttonPressed() {
	Sounds::getInstance()->playClick();
}
void SelectLevelButtonBack::buttonReleased() {
	parent->removeAllChildren();
	IGDirector::getInstance()->switchScene(new SceneMenu());
}

// level number label
SelectLevelLabelNumber::SelectLevelLabelNumber(int _level, float x, float y) {
	level = _level;
	fontName = std::string("font_deutsch_26");
	font = IGResourceManager::getInstance()->getFont(fontName);
	set(IGPoint(x, y), IGRect(126, 120));
	char buffer[200];
	sprintf(buffer, "%i", level);
	setString(std::string(buffer));
	z = 2;
	tag = SelectLevelTagLevels+level;

	SQLite3Wrapper db("levels.db");
	sprintf(buffer, "SELECT complete_easy,complete_medium,complete_hard,perfect FROM level WHERE num='%i';", level);
	db.exe(std::string(buffer));
	std::vector<std::string>::iterator i = db.vdata.begin();
	std::string data = *i;
	completedEasy = (data == "1" ? true : false);
	++i; data = *i;
	completedMedium = (data == "1" ? true : false);
	++i; data = *i;
	completedHard = (data == "1" ? true : false);
	++i; data = *i;
	perfect = (data == "1" ? true : false);
	
	updateOpacity();
}
void SelectLevelLabelNumber::updateOpacity() {
	bool completed = false;
	switch(Settings::getInstance()->difficulty) {
		case GameDifficultyEasy:
			if(completedEasy)
				completed = true;
			break;
		case GameDifficultyMedium:
			if(completedMedium)
				completed = true;
			break;
		case GameDifficultyHard:
			if(completedHard)
				completed = true;
			break;
	}
	if(completed)
		setColor(255,255,255,255);
	else
		setColor(255,255,255,64);
}

// scene select level
SceneSelectLevel::SceneSelectLevel() {
	IGScene::IGScene();
	IGLog("SceneSelectLevel init");

	// load the resources
	IwGetResManager()->LoadGroup("select_level.group");
	
	// the background
	IGSprite* spriteBackground = NULL;
	switch(GameData::getInstance()->stage) {
	case GameStageForest:
		spriteBackground = new IGSprite("select_level_forest", IGPoint(160,240), 0);
		break;
	case GameStageCaves:
		spriteBackground = new IGSprite("select_level_caves", IGPoint(160,240), 0);
		break;
	case GameStageBeach:
		spriteBackground = new IGSprite("select_level_beach", IGPoint(160,240), 0);
		break;
	case GameStageShip:
		spriteBackground = new IGSprite("select_level_ship", IGPoint(160,240), 0);
		break;
	}
	this->addChild(spriteBackground);
	
	// header
	IGSprite* spriteHeader = new IGSprite("select_level_header", IGPoint(192,35), 1);
	this->addChild(spriteHeader);

	// back
	SelectLevelButtonBack* buttonBack = new SelectLevelButtonBack();
	this->addChild(buttonBack);

	// display the level numbers
	startingLevel = 0;
	switch(GameData::getInstance()->stage) {
		case GameStageForest: startingLevel = 1; break;
		case GameStageCaves: startingLevel = 31; break;
		case GameStageBeach: startingLevel = 61; break;
		case GameStageShip: startingLevel = 91; break;
	}
	float x = 31.5, y = 100;
	for(int i=startingLevel; i<startingLevel+30; i++) {
		SelectLevelLabelNumber* num = new SelectLevelLabelNumber(i, x, y);
		this->addChild(num);

		// display stars for perfect levels
		if(num->perfect) {
			IGSprite* spritePerfect = new IGSprite("select_level_perfect", IGPoint(x+18,y+17), 3);
			this->addChild(spritePerfect);
		}

		// update coordinates
		x += 64;
		if(x > 320) {
			x = 31.5;
			y += 60;
		}
	}

	// difficulty
	spriteDiffEasy = new IGSprite("select_level_diff_easy", IGPoint((float)52.5,(float)455), 2);
	spriteDiffMedium = new IGSprite("select_level_diff_medium", IGPoint(160,455), 2);
	spriteDiffHard = new IGSprite("select_level_diff_hard", IGPoint((float)267.5,(float)455), 2);
	this->addChild(spriteDiffEasy);
	this->addChild(spriteDiffMedium);
	this->addChild(spriteDiffHard);
	updateDifficulty();

	// level to load
	levelToLoad = -1;
	loadLevelTick = 0;
}

SceneSelectLevel::~SceneSelectLevel() {
}

void SceneSelectLevel::updateDifficulty() {
	// make the difficulty button opacities right
	switch(Settings::getInstance()->difficulty) {
	case GameDifficultyEasy:
		spriteDiffEasy->setOpacity(255);
		spriteDiffMedium->setOpacity(64);
		spriteDiffHard->setOpacity(64);
		break;
	case GameDifficultyMedium:
		spriteDiffEasy->setOpacity(64);
		spriteDiffMedium->setOpacity(255);
		spriteDiffHard->setOpacity(64);
		break;
	case GameDifficultyHard:
		spriteDiffEasy->setOpacity(64);
		spriteDiffMedium->setOpacity(64);
		spriteDiffHard->setOpacity(255);
		break;
	}

	// update the opacity of the level numbers
	for(int i=startingLevel; i<startingLevel+30; i++) {
		SelectLevelLabelNumber* num = (SelectLevelLabelNumber*)this->getChildByTag(SelectLevelTagLevels+i);
		if(num != NULL)
			num->updateOpacity();
	}
}

void SceneSelectLevel::update() {
	// level selected?
	if(levelToLoad != -1) {
		if(loadLevelTick == 2) {
			// change music
			Sounds::getInstance()->startMusicGameplay();

			// load level, start game
			GameData::getInstance()->level = levelToLoad;
			GameData::getInstance()->loadLevel();
			this->removeAllChildren();
			IGDirector::getInstance()->switchScene(new SceneGame());
			return;
		}
		loadLevelTick++;
	}

	// update everything else
	IGNode::update();
}

bool SceneSelectLevel::touch(s3ePointerTouchEvent* event) {
	char buffer[100];

	// handle other touches first
	if(IGNode::touch(event))
		return true;

	// translate coordinates
	float x = IGDistorter::getInstance()->distortBackX((float)event->m_x);
	float y = IGDistorter::getInstance()->distortBackY((float)event->m_y);

	// touching down
	if(event->m_Pressed) {
		// check difficulty
		if(y >= 430) {
			// easy
			if(x < 105) {
				Settings::getInstance()->difficulty = GameDifficultyEasy;
				IGLog("SceneSelectLevel changed difficulty to easy");
			}
			// medium
			else if(x >= 105 && x < 215) {
				Settings::getInstance()->difficulty = GameDifficultyMedium;
				IGLog("SceneSelectLevel changed difficulty to medium");
			}
			// hard
			else {
				Settings::getInstance()->difficulty = GameDifficultyHard;
				IGLog("SceneSelectLevel changed difficulty to hard");
			}

			// save settings
			Sounds::getInstance()->playClick();
			Settings::getInstance()->save();
			updateDifficulty();
			return true;
		}

		// check levels
		if(y >= 70 && y < 430 && x >= 0 && x < 320) {
			// see which level is selected
			int levelX = (int)(x/63);
			int levelY = (int)((y-70)/60);
			int level = (5*levelY)+levelX+startingLevel;
			sprintf(buffer, "SceneSelectLevel touched level %i", level);
			IGLog(buffer);

			// highlight the label
			SelectLevelLabelNumber* num = (SelectLevelLabelNumber*)this->getChildByTag(SelectLevelTagLevels+level);
			num->setColor(255,196,0,255);
			
			// level will load on next frame
			Sounds::getInstance()->playClick();
			levelToLoad = level;
			return true;
		}
	}
	return false;
}