#include "game_data.h"
#include "ig2d/ig_global.h"
#include "settings.h"
#include "sounds.h"

GameData* GameData::instance = NULL;

GameData* GameData::getInstance() {
	if(instance == NULL)
		instance = new GameData();
	return instance;
}

GameData::GameData() {
	IGLog("GameData init");
	activeGame = false;
}

GameData::~GameData() {
}

void GameData::shutdown() {
	if(instance != NULL)
		delete instance;
}

void GameData::loadLevel() {
	SQLite3Wrapper db("levels.db");
	char buffer[100];
	sprintf(buffer, "SELECT data,min_moves FROM level WHERE num='%i';", level);
	db.exe(std::string(buffer));
	std::vector<std::string>::iterator i = db.vdata.begin();

	// tiles
	std::string strData = *i;
	strcpy(tileString, strData.c_str());

	// moves
	++i;
	std::string strMoves = *i;
	movesLeft = atoi(strMoves.c_str());

	// make sure the stage is correct
	stage = stageForLevel(level);
	
	// set difficulty
	difficulty = Settings::getInstance()->difficulty;

	// update moves left based on difficulty
	switch(difficulty) {
		case GameDifficultyEasy: movesLeft += 10; break;
		case GameDifficultyMedium: movesLeft += 6; break;
		case GameDifficultyHard: movesLeft += 2; break;
	}

	// set up tiles
	keys.clear();
	int keyId = 0;
	int x, y;
	for(x=0; x<GAME_BOARD_WIDTH; x++) {
		for(y=0; y<GAME_BOARD_HEIGHT; y++) {
			switch(tileString[GAME_BOARD_WIDTH*y+x]) {
			case '.': // space
			default:
				tiles[x][y] = GameTileSpace;
				break;
			case '*': // solid
				tiles[x][y] = GameTileSolid4Sides;
				break;
			case '!': // key
				tiles[x][y] = GameTileSpace;
				GameData::Key k;
				k.x = x; k.y = y; k.used = false;
				k.id = keyId; keyId++;
				keys.push_back(k);
				break;
			case 'x': // chest
				tiles[x][y] = GameTileChest;
				break;
			case 'X': // open chest
				tiles[x][y] = GameTileChestOpen;
				break;
			case 'o': // door switch
				tiles[x][y] = GameTileSwitch;
				break;
			case '|': // left-right door closed
				tiles[x][y] = GameTileDoorLRClosed;
				break;
			case '#': // left-right door open
				tiles[x][y] = GameTileDoorLROpen;
				break;
			case '-': // top-bottom door closed
				tiles[x][y] = GameTileDoorTBClosed;
				break;
			case '=': // top-bottom door open
				tiles[x][y] = GameTileDoorTBOpen;
				break;
			}
		}
	}
	// figure out what all the walls should look like
	bool top, right, bottom, left;
	for(x=0; x<GAME_BOARD_WIDTH; x++) {
		for(y=0; y<GAME_BOARD_HEIGHT; y++) {
			if(tiles[x][y] == GameTileSolid4Sides) {
				top = false; right = false; bottom = false; left = false;
				if(y == 0 || (tiles[x][y-1] >= GameTileSolid4Sides && tiles[x][y-1] <= GameTileSolid0Sides))
					top = true;
				if(x == GAME_BOARD_WIDTH-1 || (tiles[x+1][y] >= GameTileSolid4Sides && tiles[x+1][y] <= GameTileSolid0Sides))
					right = true;
				if(y == GAME_BOARD_HEIGHT-1 || (tiles[x][y+1] >= GameTileSolid4Sides && tiles[x][y+1] <= GameTileSolid0Sides))
					bottom = true;
				if(x == 0 || (tiles[x-1][y] >= GameTileSolid4Sides && tiles[x-1][y] <= GameTileSolid0Sides))
					left = true;
				
				// set the walls tiles
				if(top && right && bottom && left)
					tiles[x][y] = GameTileSolid4Sides;
				else if(top && right && bottom && !left)
					tiles[x][y] = GameTileSolid3SidesTRB;
				else if(top && right && !bottom && left)
					tiles[x][y] = GameTileSolid3SidesTRL;
				else if(top && !right && bottom && left)
					tiles[x][y] = GameTileSolid3SidesTLB;
				else if(!top && right && bottom && left)
					tiles[x][y] = GameTileSolid3SidesRBL;
				else if(top && right && !bottom && !left)
					tiles[x][y] = GameTileSolid2SidesTR;
				else if(top && !right && bottom && !left)
					tiles[x][y] = GameTileSolid2SidesTB;
				else if(top && !right && !bottom && left)
					tiles[x][y] = GameTileSolid2SidesTL;
				else if(!top && right && bottom && !left)
					tiles[x][y] = GameTileSolid2SidesRB;
				else if(!top && right && !bottom && left)
					tiles[x][y] = GameTileSolid2SidesRL;
				else if(!top && !right && bottom && left)
					tiles[x][y] = GameTileSolid2SidesBL;
				else if(top && !right && !bottom && !left)
					tiles[x][y] = GameTileSolid1SidesT;
				else if(!top && right && !bottom && !left)
					tiles[x][y] = GameTileSolid1SidesR;
				else if(!top && !right && bottom && !left)
					tiles[x][y] = GameTileSolid1SidesB;
				else if(!top && !right && !bottom && left)
					tiles[x][y] = GameTileSolid1SidesL;
				else if(!top && !right && !bottom && !left)
					tiles[x][y] = GameTileSolid0Sides;
			}
		}
	}
}

bool GameData::loadGame() {
	SQLite3Wrapper db("saved_game.db");
	db.exe(std::string("SELECT stage,difficulty,level,tiles,moves,keys FROM saved_game LIMIT 1;"));
	if(db.vdata.size() == 0)
		return false;
	std::vector<std::string>::iterator i = db.vdata.begin();

	// stage
	std::string strStage = *i;
	stage = atoi(strStage.c_str());

	// difficulty
	++i;
	std::string strDifficulty = *i;
	difficulty = atoi(strDifficulty.c_str());
	Settings::getInstance()->difficulty = difficulty;

	// level
	++i;
	std::string strLevel = *i;
	level = atoi(strLevel.c_str());

	// tiles
	++i;
	std::string strTiles = *i;
	strcpy(tileString, strTiles.c_str());

	// moves
	++i;
	std::string strMoves = *i;
	movesLeft = atoi(strMoves.c_str());

	// keys (key string format is "x,y:x,y:x,y")
	keys.clear();
	++i;
	std::string strKeys = *i;
	// split key string by ":", returns a vector of "x,y" pairs
	std::vector<std::string> keyVec1 = GameData::explode(std::string(":"), strKeys);
	std::vector<std::string>::iterator ii = keyVec1.begin();
	int keyId = 0;
	while(ii!=keyVec1.end()) {
		std::string keyVec1Str = *ii;
		// split "x,y,used" pair by ",", return an x and a y and used bool
		std::vector<std::string> keyVec2 = GameData::explode(std::string(","), keyVec1Str);
		std::vector<std::string>::iterator iii = keyVec2.begin();
		// now make it a key
		GameData::Key key;
		key.x = atoi(((std::string)(*iii)).c_str());
		++iii;
		key.y = atoi(((std::string)(*iii)).c_str());
		++iii;
		key.used = (atoi(((std::string)(*iii)).c_str()) == 0 ? false : true);
		key.id = keyId; keyId++;
		keys.push_back(key);
		// continue the loop
		++ii;
	}
	
	// set up tiles
	int x, y;
	for(x=0; x<GAME_BOARD_WIDTH; x++)
		for(y=0; y<GAME_BOARD_HEIGHT; y++)
			tiles[x][y] = tileString[GAME_BOARD_WIDTH*y+x]-'A';
	
	return true;
}

void GameData::saveGame() {
	// set up tile string
	int x, y;
	for(x=0; x<GAME_BOARD_WIDTH; x++)
		for(y=0; y<GAME_BOARD_HEIGHT; y++)
			tileString[GAME_BOARD_WIDTH*y+x] = tiles[x][y]+'A';
	tileString[GAME_BOARD_WIDTH*GAME_BOARD_HEIGHT] = '\0';

	// set up key string
	std::string keyString;
	for(unsigned int i=0; i < keys.size(); i++) {
		char buffer2[50];
		sprintf(buffer2, "%i,%i,%i", keys[i].x, keys[i].y, (keys[i].used?1:0));
		keyString.append(buffer2);
		if(i < keys.size()-1)
			keyString.append(":");
	}

	SQLite3Wrapper db("saved_game.db");
	char buffer[512];
	sprintf(buffer, "DELETE FROM saved_game; INSERT INTO saved_game (stage,difficulty,level,tiles,moves,keys) VALUES('%i','%i','%i','%s','%i','%s')", stage, difficulty, level, tileString, movesLeft, keyString.c_str());
	db.exe(std::string(buffer));
}

bool GameData::isSavedGame() {
	SQLite3Wrapper db("saved_game.db");
	db.exe(std::string("SELECT level FROM saved_game;"));
	if(db.vdata.size() == 0)
		return false;
	else
		return true;
}

void GameData::moveKeys(int dir) {
	int x, y, newX=-1, newY=-1;
	int keysMoved = 0;
	std::vector<std::string> doorsSwitched;
	unsigned int i,j;
	bool hitChest = false; // debug

	// loop through the keys
	for(i=0; i < keys.size(); i++) {
		if(keys[i].used == false) {
			// find the coordinates if the key moved
			switch(dir) {
			case GameDirLeft:
				newX = keys[i].x-1;
				newY = keys[i].y;
				break;
			case GameDirRight:
				newX = keys[i].x+1;
				newY = keys[i].y;
				break;
			case GameDirDown:
				newX = keys[i].x;
				newY = keys[i].y+1;
				break;
			}
			
			// if it won't move off the board
			if(newX >= 0 && newX < GAME_BOARD_WIDTH && newY >= 0 && newY < GAME_BOARD_HEIGHT) {
				// if it hits nothing (or an open door), move it
				if(tiles[newX][newY] == GameTileSpace || tiles[newX][newY] == GameTileDoorLROpen || tiles[newX][newY] == GameTileDoorTBOpen) {
					keys[i].x = newX;
					keys[i].y = newY;
					keysMoved++;
				}

				// if it hits a chest, change it
				else if(tiles[newX][newY] == GameTileChest) {
					hitChest = true; //debug
					keys[i].used = true;
					tiles[newX][newY] = GameTileChestOpen;
					keys[i].x = newX;
					keys[i].y = newY;
					keysMoved++;
					IGLog("GameData a key hit a chest!");

					// play open chest sound
					Sounds::getInstance()->playOpenChest();
				}

				// if it hits a door switch, open doors
				else if(tiles[newX][newY] == GameTileSwitch) {
					// move key
					keys[i].x = newX;
					keys[i].y = newY;
					keysMoved++;
					// toggle doors
					char buffer[100];
					sprintf(buffer, "%i,%i", newX, newY);
					doorsSwitched.push_back(std::string(buffer));
					IGLog("GameData a key hit a switch");
				}
			}
		}
	}

	// now that the keys have moved, go back and undo any key stacking
	bool keysAreStacked = true;
	while(keysAreStacked) {
		keysAreStacked = false;
		// loop through keys
		for(i=0; i<keys.size(); i++) {
			// if these keys aren't used yet
			if(keys[i].used == false) {
				// loop through keys again
				for(j=0; j<keys.size(); j++) {
					// and if these ones aren't used
					if(keys[j].used == false) {
						// and we're not looking at the same key
						if(keys[i].id != keys[j].id) {
							// and if they have the same coordinates
							if(keys[i].x == keys[j].x && keys[i].y == keys[j].y) {
								IGLog("GameData keys are stacked, unstacking...");
								keysAreStacked = true;
								switch(dir) {
								case GameDirLeft:
									newX = keys[i].x+1;
									newY = keys[i].y;
									keysMoved--;
									break;
								case GameDirRight:
									newX = keys[i].x-1;
									newY = keys[i].y;
									keysMoved--;
									break;
								case GameDirDown:
									newX = keys[i].x;
									newY = keys[i].y-1;
									keysMoved--;
									break;
								}
								// if we unstack the key that hit the door, we didn't actually hit the door
								if(doorsSwitched.size() > 0) {
									char buffer[100];
									sprintf(buffer, "%i,%i", keys[i].x, keys[i].y);
									std::string doorSwitchString(buffer);
									std::vector<std::string>::iterator it = doorsSwitched.begin();
									while(it != doorsSwitched.end()) {
										if((std::string)(*it) == doorSwitchString) {
											IGLog("GameData nevermind, the key that hit the door switch moved back");
											doorsSwitched.erase(it);
											break;
										}
										++it;
									}
								}
								keys[i].x = newX;
								keys[i].y = newY;
							}
						}
					}
				}
			}
		}
	}
	
	// if doors have changed, lets make sure we haven't closed any doors on keys
	if(doorsSwitched.size() > 0) {
		doorOpened = true;
		// play door sound
		Sounds::getInstance()->playDoor();

		// open/close doors
		for(x=0; x<GAME_BOARD_WIDTH; x++) {
			for(y=0; y<GAME_BOARD_HEIGHT; y++) {
				// open closed doors
				if(tiles[x][y] == GameTileDoorLRClosed) {
					tiles[x][y] = GameTileDoorLROpen;
				} else if(tiles[x][y] == GameTileDoorTBClosed) {
					tiles[x][y] = GameTileDoorTBOpen;
				}
				// close open doors?
				else if(tiles[x][y] == GameTileDoorLROpen || tiles[x][y] == GameTileDoorTBOpen) {
					bool closeDoor = true;
					for(i=0; i<keys.size(); i++) {
						if(keys[i].used == false) {
							// is there a key blocking this door?
							if(keys[i].x == x && keys[i].y == y) {
								closeDoor = false;
								IGLog("GameData can't close a door, there's a key in the way");
							}
						}
					}
					// if we're supposed to close, or we're supposed to not close a different door
					if(closeDoor) {
						// then close
						if(tiles[x][y] == GameTileDoorLROpen) {
							tiles[x][y] = GameTileDoorLRClosed;
						} else {
							tiles[x][y] = GameTileDoorTBClosed;
						}
					} else {
						IGLog("close door should be false, right?");
					}
				}
			}
		}
	} else {
		doorOpened = false;
	}

	// if keys have indeed moved, substract from movesLeft
	if(keysMoved > 0) {
		movesLeft--;
		// play key move sounds
		Sounds::getInstance()->playKeyMove();
	}

	if(hitChest) {
		hitChest = true; // debug
	}
}

bool GameData::cannotMove() {
	int tile;

	// loop through keys seeing if they can move
	for(unsigned int i=0; i<keys.size(); i++) {
		if(keys[i].used == false) {
			// left
			if(keys[i].x > 0) {
				tile = tiles[keys[i].x-1][keys[i].y];
				if(tile == GameTileSpace || tile == GameTileChest || tile == GameTileDoorLROpen || tile == GameTileDoorTBOpen || tile == GameTileSwitch) {
					return false;
				}
			}

			// right
			if(keys[i].x < GAME_BOARD_WIDTH-1) {
				tile = tiles[keys[i].x+1][keys[i].y];
				if(tile == GameTileSpace || tile == GameTileChest || tile == GameTileDoorLROpen || tile == GameTileDoorTBOpen || tile == GameTileSwitch) {
					return false;
				}
			}

			// down
			if(keys[i].y < GAME_BOARD_HEIGHT-1) {
				tile = tiles[keys[i].x][keys[i].y+1];
				if(tile == GameTileSpace || tile == GameTileChest || tile == GameTileDoorLROpen || tile == GameTileDoorTBOpen || tile == GameTileSwitch) {
					return false;
				}
			}
		}
	}
	return true;
}

void GameData::beatLevel() {
	SQLite3Wrapper db("levels.db");
	char buffer1[50];
	switch(difficulty) {
		case GameDifficultyEasy: sprintf(buffer1, "complete_easy"); break;
		case GameDifficultyMedium: sprintf(buffer1, "complete_medium"); break;
		case GameDifficultyHard: sprintf(buffer1, "complete_hard"); break;
	}
	char buffer2[100];
	sprintf(buffer2, "UPDATE level SET complete='1',%s='1' WHERE num='%i'", buffer1, level);
	db.exe(std::string(buffer2));
	IGLog("GameData marked level as complete");
}

bool GameData::stageLocked(int stage) {
	// levels per stage
	int min, max;
	switch(stage) {
	default:
	case GameStageCaves: // caves
		min = 1; max = 30;
		break;
	case GameStageBeach: // beach
		min = 31; max = 60;
		break;
	case GameStageShip: // pirate ship
		min = 61; max = 90;
		break;
	}
	
	// search the database
	SQLite3Wrapper db("levels.db");
	char buffer[100];
	sprintf(buffer, "SELECT num FROM level WHERE complete='0' AND num >= '%i' AND num <= '%i';", min, max);
	db.exe(std::string(buffer));

	// if there are any uncomplete levels, it's locked
	if(db.vdata.size() > 0)
		return true;
	else
		return false;
}

int GameData::stageForLevel(int level) {
	if(level <= 30)
		return GameStageForest;
	else if(level >= 31 && level <= 60)
		return GameStageCaves;
	else if(level >= 61 && level <= 90)
		return GameStageBeach;
	else
		return GameStageShip;
}

std::vector<std::string> GameData::explode(const std::string &delimiter, const std::string &str) {
	std::vector<std::string> arr;

	int strleng = str.length();
	int delleng = delimiter.length();
	if(delleng==0)
		return arr; // no change

	int i=0;
	int k=0;
	while(i<strleng) {
		int j=0;
		while(i+j<strleng && j<delleng && str[i+j]==delimiter[j])
			j++;
		if (j==delleng) { // found delimiter
			arr.push_back(str.substr(k, i-k));
			i+=delleng;
			k=i;
		} else {
			i++;
		}
	}
	arr.push_back(str.substr(k, i-k));
	return arr;
}
