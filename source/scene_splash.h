#pragma once
#ifndef SCENE_SPLASH_H
#define SCENE_SPLASH_H

#include "Iw2D.h"
#include "ig2d/ig.h"

// splash image
class SplashImage: public IGSprite {
public:
	SplashImage();
};

// splash scene
class SceneSplash: public IGScene {
public:
	SceneSplash();
	~SceneSplash();
	void update();
	
	SplashImage* splashImage;
	int stage;
	int fadeCount;
	int64 start;
};

#endif // SCENE_SPLASH_H