#include "config.h"
#include "ig2d/ig_global.h"
#include "sounds.h"
#include "settings.h"
#include "game_data.h"
#include <s3eExtensions/s3eExt_BackgroundMusic.h>
#include <s3eExtensions/s3eExt_BackgroundAudio.h>

Sounds* Sounds::instance = NULL;

Sounds* Sounds::getInstance() {
	if(instance == NULL)
		instance = new Sounds();
	return instance;
}

Sounds::Sounds() {
	IGLog("Sounds init");

	soundKeyMove = NULL;
	soundOpenChest = NULL;
	soundRestartLevel = NULL;
	soundDoor = NULL;
	soundClick = NULL;
	soundMapLocked = NULL;
	soundUnlockAchievement = NULL;
	
	if(AIRPLAY_DEVICE == AIRPLAY_DEVICE_IPHONE) {
		// for iphone, if there's background music playing, keep it playing
		if(s3eExtBackgroundMusicAvailable() == S3E_TRUE) {
			if(s3eBackgroundMusicGetInt(S3E_BACKGROUNDMUSIC_PLAYBACK_STATE) == S3E_BACKGROUNDMUSIC_PLAYBACK_INTERRUPTED)
				s3eBackgroundMusicPlay();
			if(s3eExtBackgroundAudioIsPlaying() == S3E_TRUE)
				s3eExtBackgroundAudioSetMix(S3E_TRUE);
		}
	}

	// set default frequency at which all channels will play at, in Hz.
	s3eSoundSetInt(S3E_SOUND_DEFAULT_FREQ, 44100);

	// turn down the volume
	s3eSoundSetInt(S3E_SOUND_VOLUME, 96);
	s3eAudioSetInt(S3E_AUDIO_VOLUME, 96);

	// if sounds are enabled, load the sounds
	if(Settings::getInstance()->soundEnabled)
		loadSounds();

	// start playing music?
	gameplayStart = 0;
	gameplayPlaying = false;
	if(Settings::getInstance()->musicEnabled) {
		if(GameData::getInstance()->isSavedGame())
			startMusicGameplay();
		else
			startMusicMenu();
	}
}

Sounds::~Sounds() {
	unloadSounds();
}

void Sounds::shutdown() {
	if(instance != NULL)
		delete instance;
}

void Sounds::loadSounds() {
	soundKeyMove = loadSound("key_move.raw");
	soundOpenChest = loadSound("open_chest.raw");
	soundRestartLevel = loadSound("restart_level.raw");
	soundDoor = loadSound("door.raw");
	soundClick = loadSound("click.raw");
	soundMapLocked = loadSound("map_locked.raw");
	soundUnlockAchievement= loadSound("unlock_achievement.raw");
	IGLog("Sounds loaded");
}

void Sounds::unloadSounds() {
	if(soundKeyMove != NULL) {
		s3eFreeBase(soundKeyMove->buffer);
		delete soundKeyMove;
		soundKeyMove = NULL;
	}
	if(soundOpenChest != NULL) {
		s3eFreeBase(soundOpenChest->buffer);
		delete soundOpenChest;
		soundOpenChest = NULL;
	}
	if(soundRestartLevel != NULL) {
		s3eFreeBase(soundRestartLevel->buffer);
		delete soundRestartLevel;
		soundRestartLevel = NULL;
	}
	if(soundDoor != NULL) {
		s3eFreeBase(soundDoor->buffer);
		delete soundDoor;
		soundDoor = NULL;
	}
	if(soundClick != NULL) {
		s3eFreeBase(soundClick->buffer);
		delete soundClick;
		soundClick = NULL;
	}
	if(soundMapLocked != NULL) {
		s3eFreeBase(soundMapLocked->buffer);
		delete soundMapLocked;
		soundMapLocked = NULL;
	}
	if(soundUnlockAchievement != NULL) {
		s3eFreeBase(soundUnlockAchievement->buffer);
		delete soundUnlockAchievement;
		soundMapLocked = NULL;
	}
	IGLog("Sounds unloaded");
}

Sounds::Sound* Sounds::loadSound(const char* filename) {
	Sounds::Sound* sound = new Sounds::Sound();
	s3eFile *fileHandle = s3eFileOpen(filename, "rb");
	sound->fileSize = s3eFileGetSize(fileHandle);
	sound->buffer = (int16*)s3eMallocBase(sound->fileSize);
	memset(sound->buffer, 0, sound->fileSize);
	s3eFileRead(sound->buffer, sound->fileSize, 1, fileHandle);
	s3eFileClose(fileHandle);
	return sound;
}

void Sounds::playSound(Sounds::Sound* sound) {
	if(Settings::getInstance()->soundEnabled == false)
		return;
	int channel = s3eSoundGetFreeChannel();
	if(s3eSoundChannelPlay(channel, sound->buffer, sound->fileSize/2, 1, 0) == S3E_RESULT_ERROR) {
		char buffer[200];
		sprintf(buffer, "Error in s3eSoundChannelPlay: %s", s3eSoundGetErrorString());
		IGLog(buffer);
	}
}

// play sounds
void Sounds::playKeyMove()		{ playSound(soundKeyMove); }
void Sounds::playOpenChest()	{ playSound(soundOpenChest); }
void Sounds::playRestartLevel()	{ playSound(soundRestartLevel); }
void Sounds::playDoor()			{ playSound(soundDoor); }
void Sounds::playClick()		{ playSound(soundClick); }
void Sounds::playMapLocked()	{ playSound(soundMapLocked); }
void Sounds::playUnlockAchievement() { playSound(soundUnlockAchievement); }

// music
void Sounds::startMusicMenu() {
	if(Settings::getInstance()->musicEnabled == false)
		return;
	if(AIRPLAY_DEVICE == AIRPLAY_DEVICE_IPHONE) {
		if(s3eExtBackgroundMusicAvailable() == S3E_TRUE)
			if(s3eBackgroundMusicGetInt(S3E_BACKGROUNDMUSIC_PLAYBACK_STATE) == S3E_BACKGROUNDMUSIC_PLAYBACK_PLAYING)
				return;
	}
	
	IGLog("Sounds starting menu music");
	gameplayPlaying = false;
	stopMusic();
	if(s3eAudioIsCodecSupported(S3E_AUDIO_CODEC_MP3))
		s3eAudioPlay("menu.mp3", 0);
	else
		IGLog("mp3 codec not available on this system");
}
void Sounds::startMusicGameplay() {
	if(Settings::getInstance()->musicEnabled == false)
		return;
	if(s3eExtBackgroundMusicAvailable() == S3E_TRUE)
		if(s3eBackgroundMusicGetInt(S3E_BACKGROUNDMUSIC_PLAYBACK_STATE) == S3E_BACKGROUNDMUSIC_PLAYBACK_PLAYING)
			return;

	IGLog("Sounds starting gameplay music");
	
	// if gameplay music is already playing, only change song if it's been playing for a full minute
	if(gameplayPlaying) {
		if(s3eTimerGetMs() - gameplayStart <= 60000)
			return;
	}

	gameplayPlaying = true;
	gameplayStart = s3eTimerGetMs();
	stopMusic();
	char buffer[100];
	int randomSong = gameplayMusicNum;
	while(randomSong == gameplayMusicNum)
		randomSong = rand()%3;
	switch(randomSong) {
		default:
		case 0: sprintf(buffer, "gameplay1.mp3"); break;
		case 1: sprintf(buffer, "gameplay2.mp3"); break;
		case 2: sprintf(buffer, "gameplay3.mp3"); break;
	}
	if(s3eAudioIsCodecSupported(S3E_AUDIO_CODEC_MP3))
		s3eAudioPlay(buffer, 0);
}

void Sounds::stopMusic() {
	IGLog("Sounds stopping music");
	if(s3eAudioIsPlaying() == S3E_TRUE)
		s3eAudioStop();
}
