#pragma once
#ifndef SCENE_ACHIEVEMENTS_H
#define SCENE_ACHIEVEMENTS_H

#include "Iw2D.h"
#include "ig2d/ig.h"

// buttons
class AchievementsButtonMenu: public IGButton {
public:
	AchievementsButtonMenu();
	void buttonPressed();
	void buttonReleased();
};
class AchievementsButtonBack: public IGButton {
public:
	AchievementsButtonBack();
	void buttonPressed();
	void buttonReleased();
};
class AchievementsButtonNext: public IGButton {
public:
	AchievementsButtonNext();
	void buttonPressed();
	void buttonReleased();
};

// tags
typedef enum {
	AchievementsTagItems = 100
} AchievementsTags;

// instructions scene
class SceneAchievements: public IGScene {
public:
	SceneAchievements();
	~SceneAchievements();

	AchievementsButtonBack* buttonBack;
	AchievementsButtonNext* buttonNext;
	int numPages;
	int pageNumber;

	void pageBack();
	void pageNext();
	void updateAchievements();
};

#endif // SCENE_ACHIEVEMENTS_H
