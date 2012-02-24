#pragma once
#ifndef SCENE_MAP_H
#define SCENE_MAP_H

#include "Iw2D.h"
#include "ig2d/ig.h"

// animations
class MapKey: public IGAnimation {
public:
	MapKey(float x, float y);
};
class MapLock: public IGAnimation {
public:
	MapLock(float x, float y);
};

// map scene
class SceneMap: public IGScene {
public:
	SceneMap();
	~SceneMap();

	bool stageLockedForest;
	bool stageLockedCaves;
	bool stageLockedBeach;
	bool stageLockedShip;

	// touch
	bool touch(s3ePointerTouchEvent* event);
};

#endif // SCENE_MAP_H