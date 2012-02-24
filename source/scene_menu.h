#pragma once
#ifndef SCENE_MENU_H
#define SCENE_MENU_H

#include "Iw2D.h"
#include "ig2d/ig.h"

// buttons
class MenuButtonPlayGame: public IGButton {
public:
	MenuButtonPlayGame();
	void buttonPressed();
	void buttonReleased();
};
class MenuButtonInstructions: public IGButton {
public:
	MenuButtonInstructions();
	void buttonPressed();
	void buttonReleased();
};
class MenuButtonAchievements: public IGButton {
public:
	MenuButtonAchievements();
	void buttonPressed();
	void buttonReleased();
};
class MenuButtonOptions: public IGButton {
public:
	MenuButtonOptions();
	void buttonPressed();
	void buttonReleased();
};
class MenuButtonOtherGames: public IGButton {
public:
	MenuButtonOtherGames();
	void buttonPressed();
	void buttonReleased();
};

// menu scene
class SceneMenu: public IGScene {
public:
	SceneMenu();
	~SceneMenu();
};

#endif // SCENE_MENU_H