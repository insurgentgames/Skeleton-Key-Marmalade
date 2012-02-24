#pragma once
#ifndef SOUNDS_H
#define SOUNDS_H

#include "s3eSound.h"
#include "s3eAudio.h"

class Sounds {
public:
	// return the instance
	static Sounds* getInstance();
	
	// constructor and destructor
	Sounds();
	~Sounds();
	static void shutdown();

	// load, unload sound effects
	void loadSounds();
	void unloadSounds();
	
	// play sound effects
	void playKeyMove();
	void playOpenChest();
	void playRestartLevel();
	void playDoor();
	void playClick();
	void playMapLocked();
	void playUnlockAchievement();

	// play music
	void startMusicMenu();
	void startMusicGameplay();
	void stopMusic();
	bool gameplayPlaying;
	int gameplayMusicNum;
	int64 gameplayStart;
	
private:
	static Sounds* instance;

	// a sound
	struct Sound {
		int16* buffer;
		int32 fileSize;
	};

	// sound methods
	Sounds::Sound* loadSound(const char* filename);
	void playSound(Sounds::Sound* sound);
	
	// sound buffers
	Sounds::Sound* soundKeyMove;
	Sounds::Sound* soundOpenChest;
	Sounds::Sound* soundRestartLevel;
	Sounds::Sound* soundDoor;
	Sounds::Sound* soundClick;
	Sounds::Sound* soundMapLocked;
	Sounds::Sound* soundUnlockAchievement;
};

#endif // SOUNDS_H