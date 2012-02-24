#pragma once
#ifndef SCENE_GAME_MENU_H
#define SCENE_GAME_MENU_H

#include "Iw2D.h"
#include "ig2d/ig.h"

// buttons
class GameMenuButtonResume: public IGButton {
public:
	GameMenuButtonResume();
	void buttonPressed();
	void buttonReleased();
};
class GameMenuButtonRestart: public IGButton {
public:
	GameMenuButtonRestart();
	void buttonPressed();
	void buttonReleased();
};
class GameMenuButtonNext: public IGButton {
public:
	GameMenuButtonNext();
	void buttonPressed();
	void buttonReleased();
};
class GameMenuButtonOptions: public IGButton {
public:
	GameMenuButtonOptions();
	void buttonPressed();
	void buttonReleased();
};
class GameMenuButtonAbandon: public IGButton {
public:
	GameMenuButtonAbandon();
	void buttonPressed();
	void buttonReleased();
};

// error continue button
class GameMenuButtonErrorContinue: public IGButton {
public:
	GameMenuButtonErrorContinue();
	void buttonPressed();
	void buttonReleased();
};

typedef enum {
	GameMenuTagErrorBlackBackground = 1,
	GameMenuTagErrorBackground = 2,
	GameMenuTagErrorMessage = 3,
	GameMenuTagErrorContinue = 4
} GameMenuTags;

// game menu scene
class SceneGameMenu: public IGScene {
public:
	SceneGameMenu();
	~SceneGameMenu();

	// error messages
	bool errorUp;
	void errorMessageStart();
	void errorMessageLastLevel();
	void errorMessageStageLocked();
	void errorContinue();

	// touch
	bool touch(s3ePointerTouchEvent* event);
};

#endif // SCENE_GAME_MENU_H
