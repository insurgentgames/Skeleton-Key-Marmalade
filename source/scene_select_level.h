#pragma once
#pragma once
#ifndef SCENE_SELECT_LEVEL_H
#define SCENE_SELECT_LEVEL_H

#include "Iw2D.h"
#include "ig2d/ig.h"

// tags
typedef enum {
	SelectLevelTagLevels = 50
} SelectLevelTags;

// back button
class SelectLevelButtonBack: public IGButton {
public:
	SelectLevelButtonBack();
	void buttonPressed();
	void buttonReleased();
};

// level number
class SelectLevelLabelNumber: public IGLabel {
public:
	SelectLevelLabelNumber(int _level, float x, float y);
	void updateOpacity();
	int level;
	bool completedEasy;
	bool completedMedium;
	bool completedHard;
	bool perfect;
};

// select level scene
class SceneSelectLevel: public IGScene {
public:
	SceneSelectLevel();
	~SceneSelectLevel();
	int startingLevel;

	// difficulty
	IGSprite* spriteDiffEasy;
	IGSprite* spriteDiffMedium;
	IGSprite* spriteDiffHard;
	void updateDifficulty();

	// loading level
	int levelToLoad;
	int loadLevelTick;

	// update, touch
	void update();
	bool touch(s3ePointerTouchEvent* event);
};

#endif // SCENE_SELECT_LEVEL_H