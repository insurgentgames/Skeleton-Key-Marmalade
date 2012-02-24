#include "scene_map.h"
#include "IwResManager.h"
#include "settings.h"
#include "sounds.h"
#include "sqlite3_wrapper.h"
#include "game_data.h"
#include "scene_select_level.h"
#include <math.h>

// key
MapKey::MapKey(float x, float y) {
	addFrame(std::string("map_key1"));
	addFrame(std::string("map_key2"));
	addFrame(std::string("map_key3"));
	addFrame(std::string("map_key4"));
	addFrame(std::string("map_key5"));
	addFrame(std::string("map_key4"));
	addFrame(std::string("map_key3"));
	addFrame(std::string("map_key2"));
	firstFrame();
	this->set(IGPoint(x,y));
	this->setFPS(8);
	z = 2;
	tag = 0;
}

// lock
MapLock::MapLock(float x, float y) {
	addFrame(std::string("map_lock1"));
	addFrame(std::string("map_lock2"));
	addFrame(std::string("map_lock3"));
	addFrame(std::string("map_lock4"));
	addFrame(std::string("map_lock5"));
	addFrame(std::string("map_lock4"));
	addFrame(std::string("map_lock3"));
	addFrame(std::string("map_lock2"));
	firstFrame();
	this->set(IGPoint(x,y));
	this->setFPS(8);
	z = 2;
	tag = 0;
}

// map menu
SceneMap::SceneMap() {
	IGScene::IGScene();
	IGLog("SceneMap init");
	float x, y;

	// load the resources
	IwGetResManager()->LoadGroup("map.group");
	
	// the background
	IGSprite* spriteBackgroundBlack = new IGSprite("background_black", IGPoint(160,240), 0);
	this->addChild(spriteBackgroundBlack);
	IGSprite* spriteBackground = new IGSprite("map_background", IGPoint(160,240), 0);
	this->addChild(spriteBackground);

	// the key and locks
	stageLockedForest = false;
	stageLockedCaves = GameData::getInstance()->stageLocked(GameStageCaves);
	stageLockedBeach = GameData::getInstance()->stageLocked(GameStageBeach);
	stageLockedShip = GameData::getInstance()->stageLocked(GameStageShip);
	
	// forest
	x = 102; y = 63;
	MapKey* forestKey = new MapKey(x,y);
	this->addChild(forestKey);

	// caves
	x = 285; y = 132;
	if(stageLockedCaves) {
		MapLock* cavesLock = new MapLock(x,y);
		this->addChild(cavesLock);
	} else {
		MapKey* cavesKey = new MapKey(x,y);
		this->addChild(cavesKey);
	}

	// beach
	x = 110; y = 282;
	if(stageLockedBeach) {
		MapLock* beachLock = new MapLock(x,y);
		this->addChild(beachLock);
	} else {
		MapKey* beachKey = new MapKey(x,y);
		this->addChild(beachKey);
	}

	// pirate ship
	x = 270; y = 396;
	if(stageLockedShip) {
		MapLock* shipLock = new MapLock(x,y);
		this->addChild(shipLock);
	} else {
		MapKey* shipKey = new MapKey(x,y);
		this->addChild(shipKey);
	}
}

SceneMap::~SceneMap() {
}

bool SceneMap::touch(s3ePointerTouchEvent* event) {
	float stageX, stageY;
	float forestDistance, cavesDistance, beachDistance, shipDistance;

	// translate coordinates
	float x = IGDistorter::getInstance()->distortBackX((float)event->m_x);
	float y = IGDistorter::getInstance()->distortBackY((float)event->m_y);

	// touching down
	if(event->m_Pressed) {
		// calculate forest distance
		stageX = 102; stageY = 63;
		forestDistance = (float)sqrt(pow(x-stageX,2)+pow(y-stageY,2));

		// calculate caves distance
		stageX = 285; stageY = 132;
		cavesDistance = (float)sqrt(pow(x-stageX,2)+pow(y-stageY,2));

		// calculate beach distance
		stageX = 110; stageY = 282;
		beachDistance = (float)sqrt(pow(x-stageX,2)+pow(y-stageY,2));

		// calculate pirate ship distance
		stageX = 270; stageY = 396;
		shipDistance = (float)sqrt(pow(x-stageX,2)+pow(y-stageY,2));

		// set the stage
		if(forestDistance < cavesDistance && forestDistance < beachDistance && forestDistance < shipDistance)
			GameData::getInstance()->stage = GameStageForest;
		else if(cavesDistance < forestDistance && cavesDistance < beachDistance && cavesDistance < shipDistance)
			GameData::getInstance()->stage = GameStageCaves;
		else if(beachDistance < forestDistance && beachDistance < cavesDistance && beachDistance < shipDistance)
			GameData::getInstance()->stage = GameStageBeach;
		else
			GameData::getInstance()->stage = GameStageShip;
		
		// if the selected stage is unlocked
		if(GameData::getInstance()->stage == GameStageForest && !stageLockedForest || 
			GameData::getInstance()->stage == GameStageCaves && !stageLockedCaves || 
			GameData::getInstance()->stage == GameStageBeach && !stageLockedBeach || 
			GameData::getInstance()->stage == GameStageShip && !stageLockedShip) {
			
			// redirect to select level scene
			Sounds::getInstance()->playClick();
			removeAllChildren();
			IGDirector::getInstance()->switchScene(new SceneSelectLevel());
		} else {
			// map locked sound
			Sounds::getInstance()->playMapLocked();
		}

		return true;
	}
	return false;
}
