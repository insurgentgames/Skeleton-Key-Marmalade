#pragma once
#ifndef SCENE_INSTRUCTION_H
#define SCENE_INSTRUCTION_H

#include "Iw2D.h"
#include "ig2d/ig.h"

// buttons
class InstructionsButtonMenu: public IGButton {
public:
	InstructionsButtonMenu();
	void buttonPressed();
	void buttonReleased();
};
class InstructionsButtonBack: public IGButton {
public:
	InstructionsButtonBack();
	void buttonPressed();
	void buttonReleased();
};
class InstructionsButtonNext: public IGButton {
public:
	InstructionsButtonNext();
	void buttonPressed();
	void buttonReleased();
};

// instructions scene
class SceneInstructions: public IGScene {
public:
	SceneInstructions();
	~SceneInstructions();

	InstructionsButtonBack* buttonBack;
	InstructionsButtonNext* buttonNext;
	IGSprite* spritePage;
	int pageNumber;

	void pageBack();
	void pageNext();
	void changePageImage();
};

#endif // SCENE_INSTRUCTION_H
