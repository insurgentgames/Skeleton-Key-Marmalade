#pragma once
#ifndef SCENE_OPTIONS_H
#define SCENE_OPTIONS_H

#include "Iw2D.h"
#include "ig2d/ig.h"

// back button
class OptionsButtonBack: public IGButton {
public:
	OptionsButtonBack();
	void buttonPressed();
	void buttonReleased();
};

// reset buttons
class OptionsButtonReset: public IGButton {
public:
	OptionsButtonReset();
	void buttonPressed();
	void buttonReleased();
};
class OptionsButtonResetYes: public IGButton {
public:
	OptionsButtonResetYes();
	void buttonPressed();
	void buttonReleased();
};
class OptionsButtonResetCancel: public IGButton {
public:
	OptionsButtonResetCancel();
	void buttonPressed();
	void buttonReleased();
};

typedef enum {
	OptionsTagResetBlackBackground = 1,
	OptionsTagResetBackground = 2,
	OptionsTagResetButtonYes = 3,
	OptionsTagResetButtonCancel = 4
} OptionsTags;

// options scene
class SceneOptions: public IGScene {
public:
	SceneOptions();
	~SceneOptions();
	
	// options
	IGSprite* spriteSound;
	IGSprite* spriteMusic;
	IGSprite* spriteShake;

	// reset
	bool resetConfirmUp;
	void resetPressed();
	void resetDelete();
	void resetCancel();

	// touch
	bool touch(s3ePointerTouchEvent* event);
};

#endif // SCENE_OPTIONS_H
