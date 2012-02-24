#pragma once
#ifndef SCENE_GAME_H
#define SCENE_GAME_H

#include "Iw2D.h"
#include "ig2d/ig.h"

// menu button
class GameButtonMenu: public IGButton {
public:
	GameButtonMenu();
	void buttonPressed();
	void buttonReleased();
};

// tile class
class GameTile: public IGSprite {
public:
	GameTile(int _tileType, int _x, int _y, int _z, int _tag);
	void changeType(int _tileType);
	void changePosition(int _x, int _y);
	int x, y;
	int tileType;

private:
	std::string getResourceName(int _tileType);
};

// tags
typedef enum {
	GameTagMoves = 1,
	GameTagLevelCompleteBackground = 2,
	GameTagLevelComplete = 3,
	GameTagLevelCompleteLabel = 4,
	GameTagLevelCompletePerfect = 5,
	GameTagAchievementBackground = 6,
	GameTagAchievementTitle = 7,
	GameTagAchievementDescription = 8,
	GameTagMessage = 9,
	GameTagTiles = 100,
	GameTagKeys = 200
} GameTags;

// messages
typedef enum {
	GameMessageNoMessage = 0,
	GameMessageSwipeScreen = 1,
	GameMessageNoMovesMenu = 2,
	GameMessageNoMovesShake = 3
} GameMessages;

// game scene
class SceneGame: public IGScene {
public:
	SceneGame();
	~SceneGame();
	void restartLevel();
	void moveKeys(int dir);

	// messages
	void messageDisplay(int messageToDisplay);
	int message;
	int64 messageStart;
	int messageCount;
	int messageStage;

	// achievements
	void unlockAchievement(int achievementId);
	void animateAchievement(int achievementId);
	void removeAchievement();
	bool isAchievementActive;
	int achievementCount;
	int achievementStage;
	int64 achievementStart;

	// misc methods
	void perfectLevel(); // set current level to perfect
	int numPerfectLevels();

	// beating the level variables
	bool won;
	int wonTicks;
	int64 wonStart;

	// saving the game
	bool firstMove;
	void saveGame();
	void deleteSavedGame();

	// update
	void update();

	// touch
	bool touch(s3ePointerTouchEvent* event);
	int touchCount;
	float touchStartX, touchStartY;

	// shake to restart
	int64 shakeStart;
	int shakeNum;
	int32 shakeX, shakeY, shakeZ;
};

#endif // SCENE_GAME_H
