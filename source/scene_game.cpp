#include "scene_game.h"
#include "IwResManager.h"
#include "s3eAccelerometer.h"
#include "settings.h"
#include "sounds.h"
#include "game_data.h"
#include "achievements.h"
#include "scene_menu.h"
#include "scene_game_menu.h"
#include "scene_select_level.h"
#include <math.h>

// menu button
GameButtonMenu::GameButtonMenu() {
	imageNormalName = std::string("game_menu");
	imageNormal = IGResourceManager::getInstance()->getImage(imageNormalName);
	imageSelectedName = std::string("game_menu2");
	imageSelected = IGResourceManager::getInstance()->getImage(imageSelectedName);
	image = imageNormal;
	this->set(IGPoint(60,30));
	touchSize = IGRect(110,60);
	z = 4;
	tag = 0;
}
void GameButtonMenu::buttonPressed() {
	Sounds::getInstance()->playClick();
}
void GameButtonMenu::buttonReleased() {
	parent->removeAllChildren();
	IGDirector::getInstance()->switchScene(new SceneGameMenu());
}

// tile
GameTile::GameTile(int _tileType, int _x, int _y, int _z, int _tag) {
	tileType = _tileType;
	imageName = getResourceName(tileType);
	image = IGResourceManager::getInstance()->getImage(imageName);
	this->changePosition(_x, _y);
	z = _z;
	tag = _tag;
	this->setOpacity(255);
}
void GameTile::changeType(int _tileType) {
	tileType = _tileType;
	changeImage(getResourceName(tileType).c_str());
}
void GameTile::changePosition(int _x, int _y) {
	x = _x;
	y = _y;
	this->set(IGPoint((float)(35+50*x),(float)(95+50*y)), IGRect(image->GetWidth(), image->GetHeight()));
}
std::string GameTile::getResourceName(int _tileType) {
	std::string name;
	char buffer1[50];
	char buffer2[200];
	switch(GameData::getInstance()->stage) {
		case GameStageForest: sprintf(buffer1, "forest"); break;
		case GameStageCaves: sprintf(buffer1, "caves"); break;
		case GameStageBeach: sprintf(buffer1, "beach"); break;
		case GameStageShip: sprintf(buffer1, "ship"); break;
	}
	switch(_tileType) {
		case GameTileSolid4Sides:
			sprintf(buffer2, "game_%s_4s", buffer1);
			name = std::string(buffer2);
			break;
		case GameTileSolid3SidesTRB:
			sprintf(buffer2, "game_%s_3s_trb", buffer1);
			name = std::string(buffer2);
			break;
		case GameTileSolid3SidesTRL:
			sprintf(buffer2, "game_%s_3s_trl", buffer1);
			name = std::string(buffer2);
			break;
		case GameTileSolid3SidesTLB:
			sprintf(buffer2, "game_%s_3s_tlb", buffer1);
			name = std::string(buffer2);
			break;
		case GameTileSolid3SidesRBL:
			sprintf(buffer2, "game_%s_3s_rbl", buffer1);
			name = std::string(buffer2);
			break;
		case GameTileSolid2SidesTR:
			sprintf(buffer2, "game_%s_2s_tr", buffer1);
			name = std::string(buffer2);
			break;
		case GameTileSolid2SidesTB:
			sprintf(buffer2, "game_%s_2s_tb", buffer1);
			name = std::string(buffer2);
			break;
		case GameTileSolid2SidesTL:
			sprintf(buffer2, "game_%s_2s_tl", buffer1);
			name = std::string(buffer2);
			break;
		case GameTileSolid2SidesRB:
			sprintf(buffer2, "game_%s_2s_rb", buffer1);
			name = std::string(buffer2);
			break;
		case GameTileSolid2SidesRL:
			sprintf(buffer2, "game_%s_2s_rl", buffer1);
			name = std::string(buffer2);
			break;
		case GameTileSolid2SidesBL:
			sprintf(buffer2, "game_%s_2s_bl", buffer1);
			name = std::string(buffer2);
			break;
		case GameTileSolid1SidesT:
			sprintf(buffer2, "game_%s_1s_t", buffer1);
			name = std::string(buffer2);
			break;
		case GameTileSolid1SidesR:
			sprintf(buffer2, "game_%s_1s_r", buffer1);
			name = std::string(buffer2);
			break;
		case GameTileSolid1SidesB:
			sprintf(buffer2, "game_%s_1s_b", buffer1);
			name = std::string(buffer2);
			break;
		case GameTileSolid1SidesL:
			sprintf(buffer2, "game_%s_1s_l", buffer1);
			name = std::string(buffer2);
			break;
		case GameTileSolid0Sides:
			sprintf(buffer2, "game_%s_0s", buffer1);
			name = std::string(buffer2);
			break;
		case GameTileKey:
			name = std::string("game_sprite_key");
			break;
		case GameTileChest:
			name = std::string("game_sprite_chest_closed");
			break;
		case GameTileChestOpen:
			name = std::string("game_sprite_chest_open");
			break;
		case GameTileSwitch:
			name = std::string("game_sprite_switch");
			break;
		case GameTileDoorLRClosed:
			name = std::string("game_sprite_door_lr_closed");
			break;
		case GameTileDoorLROpen:
			name = std::string("game_sprite_door_lr_open");
			break;
		case GameTileDoorTBClosed:
			name = std::string("game_sprite_door_tb_closed");
			break;
		case GameTileDoorTBOpen:
			name = std::string("game_sprite_door_tb_open");
			break;	
	}
	return name;
}

// scene game
SceneGame::SceneGame() {
	IGScene::IGScene();
	IGLog("SceneGame init");

	// set the game as active
	GameData::getInstance()->activeGame = true;

	// load the resources
	IwGetResManager()->LoadGroup("game.group");

	// start up the level
	this->restartLevel();
}

SceneGame::~SceneGame() {
	// the game is no longer active
	GameData::getInstance()->activeGame = false;

	// save the game
	saveGame();

	// shaking
	if(Settings::getInstance()->shakeToRestart) {
		s3eAccelerometerStop();
	}
}

void SceneGame::restartLevel() {
	// in case we've already been playing...
	this->removeAllChildren();
	shakeStart = s3eTimerGetMs();

	// the background
	IGSprite* spriteBackground = NULL;
	switch(GameData::getInstance()->stage) {
	case GameStageForest:
		spriteBackground = new IGSprite("game_background_forest", IGPoint(160,270), 0);
		break;
	case GameStageCaves:
		spriteBackground = new IGSprite("game_background_caves", IGPoint(160,270), 0);
		break;
	case GameStageBeach:
		spriteBackground = new IGSprite("game_background_beach", IGPoint(160,270), 0);
		break;
	case GameStageShip:
		spriteBackground = new IGSprite("game_background_ship", IGPoint(160,270), 0);
		break;
	}
	this->addChild(spriteBackground);

	// the wood
	IGSprite* spriteWood = new IGSprite("game_wood", IGPoint(160,240), 3);
	this->addChild(spriteWood);
	
	// menu button
	GameButtonMenu* buttonMenu =  new GameButtonMenu();
	this->addChild(buttonMenu);
	
	// heads up display
	char buffer[100];
	sprintf(buffer, "%i", GameData::getInstance()->level);
	IGLabel* labelLevel = new IGLabel("font_algerian_24", std::string(buffer), IGPoint(160,22), IGRect(100,44), 4);
	labelLevel->setColor(255,255,255,255);
	this->addChild(labelLevel);
	sprintf(buffer, "%i", GameData::getInstance()->movesLeft);
	IGLabel* labelMoves = new IGLabel("font_algerian_24", std::string(buffer), IGPoint(260,22), IGRect(100,44), 4, GameTagMoves);
	labelMoves->setColor(255,255,255,255);
	this->addChild(labelMoves);
	
	// draw the tiles
	int x,y;
	GameTile* tile = NULL;
	for(x=0; x<GAME_BOARD_WIDTH; x++) {
		for(y=0; y<GAME_BOARD_HEIGHT; y++) {
			if(GameData::getInstance()->tiles[x][y] != GameTileSpace) {
				tile = new GameTile(GameData::getInstance()->tiles[x][y], x, y, 1, GameTagTiles+(GAME_BOARD_WIDTH*y+x));
				this->addChild(tile);
			}
		}
	}

	// draw the keys
	for(unsigned int i=0; i < GameData::getInstance()->keys.size(); i++) {
		if(GameData::getInstance()->keys[i].used == false) {
			x = GameData::getInstance()->keys[i].x;
			y = GameData::getInstance()->keys[i].y;
			tile = new GameTile(GameTileKey, x, y, 2, GameTagKeys+i);
			this->addChild(tile);
		}
	}

	// checking for winning
	won = false;
	wonTicks = 0;
	wonStart = 0;

	// message
	if(GameData::getInstance()->level == 1)
		messageDisplay(GameMessageSwipeScreen);
	
	// prevent touches for the first couple frames
	touchCount = 0;
	touchStartX = 0;
	touchStartY = 0;

	// saving the game, only save if a move has been made
	firstMove = false;

	// shaking
	if(Settings::getInstance()->shakeToRestart) {
		s3eAccelerometerStart();
		shakeNum = 0;
		shakeX = 0;
		shakeY = 0;
		shakeZ = 0;
	}

	// achievement
	isAchievementActive = false;
	achievementCount = 0;
	achievementStage = 0;
}

void SceneGame::moveKeys(int dir) {
	firstMove = true;

	int movesLeft = GameData::getInstance()->movesLeft;
	if(movesLeft == 0) {
		// out of moves, sorry
		if(Settings::getInstance()->shakeToRestart) {
			messageDisplay(GameMessageNoMovesShake);
		} else {
			messageDisplay(GameMessageNoMovesMenu);
		}
		return;
	}

	// update moves left label?
	GameData::getInstance()->moveKeys(dir);
	if(GameData::getInstance()->doorOpened) {
		// achievement "Knock Knock"
		unlockAchievement(AchievementKnockKnock);
		AchievementData::getInstance()->doorsOpened++;
		AchievementData::getInstance()->save();

		// check for achievement "The Doorman"
		if(AchievementData::getInstance()->doorsOpened == 50) {
			unlockAchievement(AchievementTheDoorman);
		}

		// check for achievement "The Door to Nowhere"
		if(GameData::getInstance()->level == 85) {
			unlockAchievement(AchievementTheDoorToKnowhere);
		}
	}
	if(GameData::getInstance()->movesLeft != movesLeft) {
		IGLabel* labelMoves = (IGLabel*)getChildByTag(GameTagMoves);
		char buffer[50];
		sprintf(buffer, "%i", GameData::getInstance()->movesLeft);
		labelMoves->setString(buffer);
	}

	// update the tiles, keys
	for(int x2=0; x2<GAME_BOARD_WIDTH; x2++) {
		for(int y2=0; y2<GAME_BOARD_HEIGHT; y2++) {
			GameTile* tile = (GameTile*)getChildByTag(GameTagTiles+(GAME_BOARD_WIDTH*y2+x2));
			if(tile != NULL) {
				if(GameData::getInstance()->tiles[x2][y2] != tile->tileType) {
					tile->changeType(GameData::getInstance()->tiles[x2][y2]);
				}
			}
		}
	}
	// update the keys
	for(unsigned int i=0; i<GameData::getInstance()->keys.size(); i++) {
		GameTile* tile = (GameTile*)getChildByTag(GameTagKeys+i);
		if(tile != NULL) {
			// if it's used now, remove it
			if(GameData::getInstance()->keys[i].used == true) {
				removeChildByTag(GameTagKeys+i);
			} else {
				// if the key has moved, update it
				if(tile->x != GameData::getInstance()->keys[i].x || tile->y != GameData::getInstance()->keys[i].y)
					tile->changePosition(GameData::getInstance()->keys[i].x, GameData::getInstance()->keys[i].y);
			}
		}
	}
	
	// check for a win
	bool checkForWin = true;
	for(unsigned int i=0; i<GameData::getInstance()->keys.size(); i++) {
		if(GameData::getInstance()->keys[i].used == false)
			checkForWin = false;
	}
	if(checkForWin) {
		IGLog("SceneGame level complete!");
		messageDisplay(GameMessageNoMessage);

		// figure out the level complete text
		char buffer[200];
		char font[80];
		int moves = GameData::getInstance()->movesLeft;
		switch(GameData::getInstance()->difficulty) {
			case GameDifficultyEasy: moves -= 10; break;
			case GameDifficultyMedium: moves -= 6; break;
			case GameDifficultyHard: moves -= 2; break;
		}
		moves *= -1;
		if(moves <= 0) {
			sprintf(buffer, "Perfect!");
			sprintf(font, "font_algerian_24");
		} else {
			if(moves == 1)
				sprintf(buffer, "good job!\n1 move away\nfrom perfect");
			else
				sprintf(buffer, "good job!\n%i moves away\nfrom perfect", moves);
			sprintf(font, "font_algerian_20");
		}

		// display the level complete nodes
		IGSprite* spriteLevelCompleteBackground = new IGSprite("background_black", IGPoint(160,240), 20, GameTagLevelCompleteBackground);
		spriteLevelCompleteBackground->setOpacity(0);
		this->addChild(spriteLevelCompleteBackground);
		IGSprite* spriteLevelComplete = new IGSprite("game_level_complete", IGPoint(160,240), 21, GameTagLevelComplete);
		spriteLevelComplete->setOpacity(0);
		this->addChild(spriteLevelComplete);
		IGLabel* spriteLevelCompleteLabel = new IGLabel(font, buffer, IGPoint(160,310), IGRect(300,100), 21, GameTagLevelCompleteLabel);
		spriteLevelCompleteLabel->setColor(255,255,255,0);
		this->addChild(spriteLevelCompleteLabel);
		if(moves <= 0) {
			// if perfect, move the "perfect!" label, and add a star
			spriteLevelCompleteLabel->position.y -= 30;
			IGSprite* spritePerfect = new IGSprite("game_message_perfect", IGPoint(160, 340), 21, GameTagLevelCompletePerfect);
			spriteLevelComplete->setOpacity(0);
			this->addChild(spritePerfect);
			
			// update level to be perfect
			this->perfectLevel();

			// check for perfectionist achievement
			int perfectLevels = this->numPerfectLevels();
			if(perfectLevels >= 10) {
				unlockAchievement(AchievementPerfectionist);
			}
			// check for professional perfectionist achievement
			if(perfectLevels >= 30) {
				unlockAchievement(AchievementProfessionalPerfectionist);
			}
			// check for mastermind
			if(perfectLevels >= TOTAL_LEVELS) {
				unlockAchievement(AchievementMastermind);
			}
		}
		wonStart = s3eTimerGetMs();
		won = true;
		GameData::getInstance()->beatLevel();
		
		// delete the saved game - if they continue, then load the next one
		deleteSavedGame();

		// check for achievements
		SQLite3Wrapper *db = NULL;
		std::vector<std::string>::iterator it;

		// check for "Begin the Hunt" achievement
		db = new SQLite3Wrapper("levels.db");
		db->exe("SELECT complete FROM level WHERE num >= 1 AND num <= 10 ORDER BY num;");
		it = db->vdata.begin();
		bool beginTheHunt = true;
		for(int i=0; i<10; i++) {
			std::string tmp = *it;
			if(tmp == "0") {
				beginTheHunt = false;
				break;
			}
			++it;
		}
		if(beginTheHunt) {
			unlockAchievement(AchievementBeginTheHunt);
		}
		delete db;

		// check for "Enter the Darkness" achievement
		db = new SQLite3Wrapper("levels.db");
		db->exe("SELECT complete FROM level WHERE num >= 1 AND num <= 30 ORDER BY num;");
		it = db->vdata.begin();
		bool enterTheDarkness = true;
		for(int i=0; i<30; i++) {
			std::string tmp = *it;
			if(tmp == "0") {
				enterTheDarkness = false;
				break;
			}
			++it;
		}
		if(enterTheDarkness) {
			unlockAchievement(AchievementEnterTheDarkness);
		}
		delete db;

		// check for "Enjoy the Sun" achievement
		db = new SQLite3Wrapper("levels.db");
		db->exe("SELECT complete FROM level WHERE num >= 31 AND num <= 60 ORDER BY num;");
		it = db->vdata.begin();
		bool enjoyTheSun = true;
		for(int i=0; i<30; i++) {
			std::string tmp = *it;
			if(tmp == "0") {
				enjoyTheSun = false;
				break;
			}
			++it;
		}
		if(enjoyTheSun) {
			unlockAchievement(AchievementEnjoyTheSun);
		}
		delete db;

		// check for "Arrrgh!" achievement
		db = new SQLite3Wrapper("levels.db");
		db->exe("SELECT complete FROM level WHERE num >= 61 AND num <= 90 ORDER BY num;");
		it = db->vdata.begin();
		bool arrrgh = true;
		for(int i=0; i<30; i++) {
			std::string tmp = *it;
			if(tmp == "0") {
				arrrgh = false;
				break;
			}
			++it;
		}
		if(arrrgh) {
			unlockAchievement(AchievementArrrgh);
		}
		delete db;
		
		// check for difficulty achievements
		bool treasureHunter = true;
		bool anAdventurerIsYou = true;
		bool intrepidExplorer = true;
		switch(GameData::getInstance()->difficulty) {
			case GameDifficultyEasy:
				// check for "Treasure Hunter" achievement
				db = new SQLite3Wrapper("levels.db");
				db->exe("SELECT complete_easy FROM level WHERE num >= 1 AND num <= 120 ORDER BY num;");
				it = db->vdata.begin();
				for(int i=0; i<120; i++) {
					std::string tmp = *it;
					if(tmp == "0") {
						treasureHunter = false;
						break;
					}
					++it;
				}
				if(treasureHunter) {
					unlockAchievement(AchievementTreasureHunter);
				}
				delete db;
				break;
			case GameDifficultyMedium:
				// check for "An Adventurer is You" achievement
				db = new SQLite3Wrapper("levels.db");
				db->exe("SELECT complete_medium FROM level WHERE num >= 1 AND num <= 120 ORDER BY num;");
				it = db->vdata.begin();
				for(int i=0; i<120; i++) {
					std::string tmp = *it;
					if(tmp == "0") {
						anAdventurerIsYou = false;
						break;
					}
					++it;
				}
				if(anAdventurerIsYou) {
					unlockAchievement(AchievementAnAdventurerIsYou);
				}
				delete db;
				break;
			case GameDifficultyHard:
				// check for "Intrepid Explorer" achievement
				db = new SQLite3Wrapper("levels.db");
				db->exe("SELECT complete_hard FROM level WHERE num >= 1 AND num <= 120 ORDER BY num;");
				it = db->vdata.begin();
				for(int i=0; i<120; i++) {
					std::string tmp = *it;
					if(tmp == "0") {
						intrepidExplorer = false;
						break;
					}
					++it;
				}
				if(intrepidExplorer) {
					unlockAchievement(AchievementIntrepidExplorer);
				}
				delete db;
				break;
		}
	} else {
		// check for stuck
		if(GameData::getInstance()->cannotMove()) {
			if(Settings::getInstance()->shakeToRestart) {
				messageDisplay(GameMessageNoMovesShake);
			} else {
				messageDisplay(GameMessageNoMovesMenu);
			}
		}
	}
}

void SceneGame::messageDisplay(int messageToDisplay) {
	message = messageToDisplay;
	IGSprite* spriteMessage = (IGSprite*)getChildByTag(GameTagMessage);
	if(spriteMessage != NULL)
		this->removeChildByTag(GameTagMessage);
	
	// which message?
	switch(messageToDisplay) {
	case GameMessageNoMessage:
		IGLog("SceneGame getting rid of message");
		break;
	case GameMessageSwipeScreen:
		IGLog("SceneGame display message intro");
		spriteMessage = new IGSprite("game_message_start", IGPoint(160,240), 20, GameTagMessage);
		this->addChild(spriteMessage);
		break;
	case GameMessageNoMovesMenu:
		IGLog("SceneGame display message no moves menu");
		spriteMessage = new IGSprite("game_message_no_moves_menu", IGPoint(160,240), 20, GameTagMessage);
		this->addChild(spriteMessage);
		break;
	case GameMessageNoMovesShake:
		IGLog("SceneGame display message no moves shake");
		spriteMessage = new IGSprite("game_message_no_moves", IGPoint(160,240), 20, GameTagMessage);
		this->addChild(spriteMessage);
		break;
	}
	messageStart = s3eTimerGetMs();
	messageCount = 0;
	messageStage = 0;
}

void SceneGame::saveGame() {
	if(firstMove)
		GameData::getInstance()->saveGame();
}

void SceneGame::deleteSavedGame() {
	SQLite3Wrapper db("saved_game.db");
	db.exe(std::string("DELETE FROM saved_game;"));
	firstMove = false;
}

void SceneGame::update() {
	// update the other nodes
	IGNode::update();
	
	// message
	if(message != GameMessageNoMessage) {
		switch(messageStage) {
			case 0: // wait
				if(s3eTimerGetMs() - messageStart >= 4000) { // 4 second
					messageStage = 1;
					messageStart = s3eTimerGetMs();
					messageCount = 0;
				}
				break;
			case 1: // fade
				if(s3eTimerGetMs() - messageStart >= 10) { // .01 seconds
					messageStart = s3eTimerGetMs();
					messageCount++;
					IGSprite* spriteMessage = (IGSprite*)getChildByTag(GameTagMessage);
					if(spriteMessage != NULL)
						spriteMessage->setOpacity((uint8)((10-messageCount)*(float)(255.0/10.0)));
					else
						message = GameMessageNoMessage;
				}
				if(messageCount >= 10)
					messageDisplay(GameMessageNoMessage);
				break;
		}
	}

	// achievement
	if(isAchievementActive) {
		switch(achievementStage) {
			case 0: // moving up
				if(s3eTimerGetMs() - achievementStart >= 3) { // .003 seconds
					achievementStart = s3eTimerGetMs();
					IGSprite* spriteAchievement = (IGSprite*)this->getChildByTag(GameTagAchievementBackground);
					IGLabel* labelName = (IGLabel*)this->getChildByTag(GameTagAchievementTitle);
					IGLabel* labelDescription = (IGLabel*)this->getChildByTag(GameTagAchievementDescription);
					if(spriteAchievement != NULL) {
						spriteAchievement->position.y -= 10;
						labelName->position.y -= 10;
						labelDescription->position.y -= 10;
					}
					achievementCount++;
					if(achievementCount == 23)
						achievementStage = 1;
				}
				break;
			case 1: // waiting
				if(s3eTimerGetMs() - achievementStart >= 3000) { // 3 seconds
					achievementStart = s3eTimerGetMs();
					achievementStage = 2;
					achievementCount = 0;
				}
				break;
			case 2: // moving down
				if(s3eTimerGetMs() - achievementStart >= 3) { // .003 seconds
					achievementStart = s3eTimerGetMs();
					IGSprite* spriteAchievement = (IGSprite*)this->getChildByTag(GameTagAchievementBackground);
					IGLabel* labelName = (IGLabel*)this->getChildByTag(GameTagAchievementTitle);
					IGLabel* labelDescription = (IGLabel*)this->getChildByTag(GameTagAchievementDescription);
					if(spriteAchievement != NULL) {
						spriteAchievement->position.y += 10;
						labelName->position.y += 10;
						labelDescription->position.y += 10;
					}
					achievementCount++;
					if(achievementCount == 23) {
						removeAchievement();
						isAchievementActive = false;
					}
				}
				break;
		}
	}

	// if the level has been won
	if(won) {
		if(wonTicks < 10) {
			if(s3eTimerGetMs() - wonStart >= 10) { //.01 seconds
				wonStart = s3eTimerGetMs();
				wonTicks++;
				IGSprite* spriteLevelCompleteBackground = (IGSprite*)getChildByTag(GameTagLevelCompleteBackground);
				if(spriteLevelCompleteBackground != NULL)
					spriteLevelCompleteBackground->setOpacity((uint8)(wonTicks*(float)(128.0/10.0)));
				IGSprite* spriteLevelComplete = (IGSprite*)getChildByTag(GameTagLevelComplete);
				if(spriteLevelComplete != NULL)
					spriteLevelComplete->setOpacity((uint8)(wonTicks*(float)(255.0/10.0)));
				IGLabel* spriteLevelCompleteLabel = (IGLabel*)getChildByTag(GameTagLevelCompleteLabel);
				if(spriteLevelCompleteLabel != NULL)
					spriteLevelCompleteLabel->setOpacity((uint8)(wonTicks*(float)(255.0/10.0)));
				IGSprite* spriteLevelCompletePerfect = (IGSprite*)getChildByTag(GameTagLevelCompletePerfect);
				if(spriteLevelCompletePerfect != NULL)
					spriteLevelCompletePerfect->setOpacity((uint8)(wonTicks*(float)(255.0/10.0)));
			}
		}
	}
	
	// prevent touches for the first couple frames
	if(touchCount < 2)
		touchCount++;
	
	// shake to restart
	static int shakeThreshold = 500;
	if(Settings::getInstance()->shakeToRestart) {
		bool shake = false;
		int32 newShakeX = s3eAccelerometerGetX();
		int32 newShakeY = s3eAccelerometerGetY();
		int32 newShakeZ = s3eAccelerometerGetZ();
		int32 shakeDiffX = ABS(newShakeX - shakeX);
		int32 shakeDiffY = ABS(newShakeY - shakeY);
		int32 shakeDiffZ = ABS(newShakeZ - shakeZ);
		if(shakeDiffX > shakeThreshold || shakeDiffX < -1*shakeThreshold)
			shake = true;
		if(shakeDiffY > shakeThreshold || shakeDiffY < -1*shakeThreshold)
			shake = true;
		if(shakeDiffZ > shakeThreshold || shakeDiffZ < -1*shakeThreshold)
			shake = true;
		if(shake) {
			// if it's been more than a second since the last shake
			if(s3eTimerGetMs() - shakeStart > 1000) {
				shakeStart = s3eTimerGetMs();
				shakeNum = 0;
			}
			IGLog("SceneGame small shake detected");
			shakeNum++;
		}
		if(shakeNum >= 6 && s3eTimerGetMs() - shakeStart <= 1000) {
			IGLog("SceneGame shake detected");
			shakeNum = 0;
			Sounds::getInstance()->playRestartLevel();
			GameData::getInstance()->loadLevel();
			removeAllChildren();
			this->removeAllChildren();
			this->restartLevel();
			//IGDirector::getInstance()->switchScene(new SceneGame());
		}
		shakeX = newShakeX;
		shakeY = newShakeY;
		shakeZ = newShakeZ;

		//char buffer[200];
		//sprintf(buffer, "accelerometer: (%li,%li,%li)", s3eAccelerometerGetX(), s3eAccelerometerGetY(), s3eAccelerometerGetZ());
		//IGLog(buffer);
	}
}

bool SceneGame::touch(s3ePointerTouchEvent* event) {
	static float swipeDist = 15;

	// prevent touches for the first couple frames, because sometimes it starts out moving down
	if(touchCount < 2)
		return false;
	
	// if the game has been won, ignore other touches and load next level
	if(won) {
		if(event->m_Pressed == false) {
			// if this is the last level, return to main menu
			if(GameData::getInstance()->level == TOTAL_LEVELS) {
				Sounds::getInstance()->playClick();
				Sounds::getInstance()->startMusicMenu();
				this->removeAllChildren();
				IGDirector::getInstance()->switchScene(new SceneMenu());
			} else {
				// play click, change music
				Sounds::getInstance()->playClick();
				Sounds::getInstance()->startMusicGameplay();

				// load the next level
				int stage = GameData::getInstance()->stageForLevel(GameData::getInstance()->level);
				int nextLevelStage = GameData::getInstance()->stageForLevel(GameData::getInstance()->level+1);
				if(stage != nextLevelStage) {
					// if the next stage is locked, go to select level scene instead
					if(GameData::getInstance()->stageLocked(nextLevelStage)) {
						this->removeAllChildren();
						IGDirector::getInstance()->switchScene(new SceneSelectLevel());
						return true;
					}
				}
				GameData::getInstance()->level++;
				GameData::getInstance()->loadLevel();
				this->removeAllChildren();
				this->restartLevel();
				//IGDirector::getInstance()->switchScene(new SceneGame());
				return true;
			}
		}
		return false;
	}

	// handle other touches first
	if(IGNode::touch(event))
		return true;

	// translate coordinates
	float x = IGDistorter::getInstance()->distortBackX((float)event->m_x);
	float y = IGDistorter::getInstance()->distortBackY((float)event->m_y);

	// touch down
	if(event->m_Pressed) {
		touchStartX = x;
		touchStartY = y;

		// if the start message is still up, bring it down
		if(message == GameMessageSwipeScreen)
			messageDisplay(GameMessageNoMessage);
	}
	// touch release
	else {
		// is it a swipe?
		int dir = GameDirNone;
		float dx = x - touchStartX;
		float dy = y - touchStartY;
		float dist = (float)sqrt(dx*dx + dy*dy);
		if(dist >= swipeDist && dx != dy) {
			// left or right swipe
			if(ABS(dx) > ABS(dy)) {
				if(dx > 0) {
					dir = GameDirRight;
					IGLog("SceneGame swiped right");
				} else {
					dir = GameDirLeft;
					IGLog("SceneGame swiped left");
				}
			}
			// up or down swipe
			else {
				if(dy > 0) {
					dir = GameDirDown;
					IGLog("SceneGame swiped down");
				} else {
					dir = GameDirUp;
					IGLog("SceneGame swiped up");
				}
			}

			// if it's left, right, or down, move the key
			if(dir == GameDirLeft || dir == GameDirRight || dir == GameDirDown) {
				moveKeys(dir);
			}
			return true;
		}
	}
	return false;
}

void SceneGame::unlockAchievement(int achievementId) {
	if(Achievements::getInstance()->unlockAchievement(achievementId)) {
		animateAchievement(achievementId);
	}
}

void SceneGame::animateAchievement(int achievementId) {
	// just in case there's an active achievement, remove it
	removeAchievement();

	// add the graphics
	IGSprite* spriteAchievement = new IGSprite("game_achievement", IGPoint(160,626), 30, GameTagAchievementBackground); // it will move up to y=398
	this->addChild(spriteAchievement);

	// get the achievement
	Achievement a = Achievements::getInstance()->getAchievement(achievementId);

	// add the labels
	IGLabel* labelName = new IGLabel("font_gabriola_22b", a.name.c_str(), IGPoint(220,635), IGRect(200,60), 31, GameTagAchievementTitle);
	labelName->setColor(255,255,255,255);
	this->addChild(labelName);
	IGLabel* labelDescription = new IGLabel("font_gabriola_14", a.description.c_str(), IGPoint(220,674), IGRect(200,60), 31, GameTagAchievementDescription);
	labelDescription->setColor(196,207,226,255);
	this->addChild(labelDescription);

	// start the achievement
	achievementCount = 0;
	isAchievementActive = true;
	achievementStage = 0;
	achievementStart = s3eTimerGetMs();
}

void SceneGame::removeAchievement() {
	if(isAchievementActive) {
		this->removeChildByTag(GameTagAchievementBackground);
		this->removeChildByTag(GameTagAchievementTitle);
		this->removeChildByTag(GameTagAchievementDescription);
	}
}

void SceneGame::perfectLevel() {
	char buffer[200];
	SQLite3Wrapper db("levels.db");
	sprintf(buffer, "UPDATE level SET perfect='1' WHERE num='%i';", GameData::getInstance()->level);
	db.exe(buffer);
}

int SceneGame::numPerfectLevels() {
	SQLite3Wrapper db("levels.db");
	db.exe("SELECT num FROM level WHERE perfect='1'");
	return db.vdata.size();
}
