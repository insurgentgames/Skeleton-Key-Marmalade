#pragma once
#ifndef GAME_DATA_H
#define GAME_DATA_H

#include <vector>
#include <string>

// game board dimensions
#define GAME_BOARD_WIDTH 6
#define GAME_BOARD_HEIGHT 8

// levels
#define TOTAL_LEVELS 120

// stage names
typedef enum {
	GameStageForest = 0,
	GameStageCaves = 1,
	GameStageBeach = 2,
	GameStageShip = 3
} GameStageNames;

// difficulties
typedef enum {
	GameDifficultyEasy = 0,
	GameDifficultyMedium = 1,
	GameDifficultyHard = 2
} GameDifficulties;

// tile types
typedef enum {
	GameTileSpace = 0,
	GameTileSolid4Sides = 1,
	GameTileSolid3SidesTRB = 2,
	GameTileSolid3SidesTRL = 3,
	GameTileSolid3SidesTLB = 4,
	GameTileSolid3SidesRBL = 5,
	GameTileSolid2SidesTR = 6,
	GameTileSolid2SidesTB = 7,
	GameTileSolid2SidesTL = 8,
	GameTileSolid2SidesRB = 9,
	GameTileSolid2SidesRL = 10,
	GameTileSolid2SidesBL = 11,
	GameTileSolid1SidesT = 12,
	GameTileSolid1SidesR = 13,
	GameTileSolid1SidesB = 14,
	GameTileSolid1SidesL = 15,
	GameTileSolid0Sides = 16,
	GameTileKey = 17,
	GameTileChest = 18,
	GameTileChestOpen = 19,
	GameTileSwitch = 20,
	GameTileDoorLRClosed = 21,
	GameTileDoorLROpen = 22,
	GameTileDoorTBClosed = 23,
	GameTileDoorTBOpen = 24
} GameTiles;

// directions
typedef enum {
	GameDirNone = 0,
	GameDirUp = 1,
	GameDirDown = 2,
	GameDirLeft = 3,
	GameDirRight = 4
} GameDirs;

class GameData {
public:
	// return the instance
	static GameData* getInstance();
	
	// constructor and destructor
	GameData();
	~GameData();
	static void shutdown();

	// is the game active?
	bool activeGame;

	// a key
	struct Key {
		int x, y;
		bool used;
		int id;
	};

	// shared data
	int stage;
	int difficulty;
	int level;
	char tileString[49];
	char tiles[6][8];
	std::vector<GameData::Key> keys;
	int movesLeft;
	bool doorOpened;
	
	// saving, loading
	void loadLevel();
	bool loadGame();
	void saveGame();
	bool isSavedGame();

	// gameplay mechanics
	void moveKeys(int dir);
	bool cannotMove();
	void beatLevel();

	// misc
	bool optionsReturnsToGame;
	bool stageLocked(int stage);
	int stageForLevel(int level);
	
private:
	static GameData* instance;

	std::vector<std::string> explode(const std::string &delimiter, const std::string &str);
};

#endif // GAME_DATA_H