#pragma once
#ifndef SCENE_NAG_H
#define SCENE_NAG_H

#include "Iw2D.h"
#include "ig2d/ig.h"

// buttons
class NagButtonYes: public IGButton {
public:
	NagButtonYes();
	void buttonPressed();
	void buttonReleased();
};
class NagButtonNo: public IGButton {
public:
	NagButtonNo();
	void buttonPressed();
	void buttonReleased();
};

// nag scene
class SceneNag: public IGScene {
public:
	SceneNag();
	~SceneNag();
	void ontoTheGame();
};

#endif // SCENE_NAG_H