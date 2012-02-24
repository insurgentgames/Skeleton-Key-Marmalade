#include "scene_options.h"
#include "IwResManager.h"
#include "settings.h"
#include "sounds.h"
#include "scene_menu.h"
#include "scene_game.h"
#include "game_data.h"
#include "achievements.h"

// back button
OptionsButtonBack::OptionsButtonBack() {
	if(GameData::getInstance()->optionsReturnsToGame) {
		imageNormalName = std::string("options_back_game");
	imageSelectedName = std::string("options_back_game2");
	} else {
		imageNormalName = std::string("options_back_menu");
		imageSelectedName = std::string("options_back_menu2");
	}
	imageNormal = IGResourceManager::getInstance()->getImage(imageNormalName);
	imageSelected = IGResourceManager::getInstance()->getImage(imageSelectedName);
	image = imageNormal;
	this->set(IGPoint(160,452));
	touchSize = IGRect(320,56);
	z = 2;
	tag = 0;
}
void OptionsButtonBack::buttonPressed() {
	Sounds::getInstance()->playClick();
}
void OptionsButtonBack::buttonReleased() {
	parent->removeAllChildren();
	if(GameData::getInstance()->optionsReturnsToGame)
		IGDirector::getInstance()->switchScene(new SceneGame());
	else
		IGDirector::getInstance()->switchScene(new SceneMenu());
}

// reset button
OptionsButtonReset::OptionsButtonReset() {
	imageNormalName = std::string("options_reset");
	imageNormal = IGResourceManager::getInstance()->getImage(imageNormalName);
	imageSelectedName = std::string("options_reset2");
	imageSelected = IGResourceManager::getInstance()->getImage(imageSelectedName);
	image = imageNormal;
	this->set(IGPoint(160,376));
	touchSize = IGRect(320,56);
	z = 2;
	tag = 0;
}
void OptionsButtonReset::buttonPressed() {
	Sounds::getInstance()->playClick();
}
void OptionsButtonReset::buttonReleased() {
	((SceneOptions*)parent)->resetPressed();
}

// reset yes button
OptionsButtonResetYes::OptionsButtonResetYes() {
	imageNormalName = std::string("options_reset_confirm_yes");
	imageNormal = IGResourceManager::getInstance()->getImage(imageNormalName);
	imageSelectedName = std::string("options_reset_confirm_yes2");
	imageSelected = IGResourceManager::getInstance()->getImage(imageSelectedName);
	image = imageNormal;
	this->set(IGPoint(85,260));
	touchSize = IGRect(150,56);
	z = 7;
	tag = OptionsTagResetButtonYes;
}
void OptionsButtonResetYes::buttonPressed() {
	Sounds::getInstance()->playClick();
}
void OptionsButtonResetYes::buttonReleased() {
	((SceneOptions*)parent)->resetDelete();
}

// reset cancel button
OptionsButtonResetCancel::OptionsButtonResetCancel() {
	imageNormalName = std::string("options_reset_confirm_cancel");
	imageNormal = IGResourceManager::getInstance()->getImage(imageNormalName);
	imageSelectedName = std::string("options_reset_confirm_cancel2");
	imageSelected = IGResourceManager::getInstance()->getImage(imageSelectedName);
	image = imageNormal;
	this->set(IGPoint(235,260));
	touchSize = IGRect(150,56);
	z = 7;
	tag = OptionsTagResetButtonCancel;
}
void OptionsButtonResetCancel::buttonPressed() {
	Sounds::getInstance()->playClick();
}
void OptionsButtonResetCancel::buttonReleased() {
	((SceneOptions*)parent)->resetCancel();
}

// options scene
SceneOptions::SceneOptions() {
	IGScene::IGScene();
	IGLog("SceneOptions init");

	// load the resources
	IwGetResManager()->LoadGroup("options.group");
	
	// the background
	IGSprite* spriteBackground = new IGSprite("options_background", IGPoint(160,240), 0);
	this->addChild(spriteBackground);

	// options
	if(Settings::getInstance()->soundEnabled)
		spriteSound = new IGSprite("options_sound_on", IGPoint(160,148), 1);
	else
		spriteSound = new IGSprite("options_sound_off", IGPoint(160,148), 1);
	if(Settings::getInstance()->musicEnabled)
		spriteMusic = new IGSprite("options_music_on", IGPoint(160,204), 1);
	else
		spriteMusic = new IGSprite("options_music_off", IGPoint(160,204), 1);
	if(Settings::getInstance()->shakeToRestart)
		spriteShake = new IGSprite("options_shake_on", IGPoint(160,260), 1);
	else
		spriteShake = new IGSprite("options_shake_off", IGPoint(160,260), 1);
	this->addChild(spriteSound);
	this->addChild(spriteMusic);
	this->addChild(spriteShake);
	
	// back
	OptionsButtonBack* buttonBack = new OptionsButtonBack();
	this->addChild(buttonBack);
	
	// reset (only display option if not in a game)
	if(GameData::getInstance()->optionsReturnsToGame == false) {
		OptionsButtonReset* buttonReset = new OptionsButtonReset();
		this->addChild(buttonReset);
	}
	resetConfirmUp = false;
}

SceneOptions::~SceneOptions() {
}

void SceneOptions::resetPressed() {
	if(resetConfirmUp)
		return;
	resetConfirmUp = true;
	
	// add reset confirm dialog
	IGSprite* spriteBlackBackground = new IGSprite("background_black", IGPoint(160,240), 5, OptionsTagResetBlackBackground);
	spriteBlackBackground->setOpacity(192);
	IGSprite* spriteResetConfirmBackground = new IGSprite("options_reset_confirm_background", IGPoint(160,203), 6, OptionsTagResetBackground);
	OptionsButtonResetYes* buttonYes = new OptionsButtonResetYes();
	OptionsButtonResetCancel* buttonCancel = new OptionsButtonResetCancel();
	this->addChild(spriteBlackBackground);
	this->addChild(spriteResetConfirmBackground);
	this->addChild(buttonYes);
	this->addChild(buttonCancel);
}

void SceneOptions::resetDelete() {
	// to do: reset all achievements

	// delete saved game
	SQLite3Wrapper savedGameDB("saved_game.db");
	savedGameDB.exe(std::string("DELETE FROM saved_game;"));

	// reset all levels
	SQLite3Wrapper levelsDB("levels.db");
	levelsDB.exe(std::string("UPDATE level SET complete='0',complete_easy='0',complete_medium='0',complete_hard='0',perfect='0';"));

	// reset all achievements
	Achievements::getInstance()->resetAchievements();

	// log it
	IGLog("SceneOptions reset all user data");

	// redirect to menu
	this->removeAllChildren();
	IGDirector::getInstance()->switchScene(new SceneMenu());
}

void SceneOptions::resetCancel() {
	this->removeChildByTag(OptionsTagResetBlackBackground);
	this->removeChildByTag(OptionsTagResetBackground);
	this->removeChildByTag(OptionsTagResetButtonYes);
	this->removeChildByTag(OptionsTagResetButtonCancel);
	resetConfirmUp = false;
}

bool SceneOptions::touch(s3ePointerTouchEvent* event) {
	// handle other touches first
	if(IGNode::touch(event))
		return true;
	
	// if reset confirm dialog is up, do no more
	if(resetConfirmUp)
		return false;
	
	// translate coordinates
	float x = IGDistorter::getInstance()->distortBackX((float)event->m_x);
	float y = IGDistorter::getInstance()->distortBackY((float)event->m_y);
	
	// touching down
	if(event->m_Pressed) {
		// sound?
		if(y > 120 && y <= 176) {
			if(Settings::getInstance()->soundEnabled) {
				spriteSound->changeImage("options_sound_off");
				Settings::getInstance()->soundEnabled = false;
				Sounds::getInstance()->unloadSounds();
			} else {
				spriteSound->changeImage("options_sound_on");
				Settings::getInstance()->soundEnabled = true;
				Sounds::getInstance()->loadSounds();
			}
			Settings::getInstance()->save();
			Sounds::getInstance()->playClick();
			return true;
		}

		// music?
		if(y > 176 && y <= 232) {
			if(Settings::getInstance()->musicEnabled) {
				spriteMusic->changeImage("options_music_off");
				Settings::getInstance()->musicEnabled = false;
				Sounds::getInstance()->stopMusic();
			} else {
				spriteMusic->changeImage("options_music_on");
				Settings::getInstance()->musicEnabled = true;
				Sounds::getInstance()->gameplayPlaying = false;
				if(GameData::getInstance()->optionsReturnsToGame)
					Sounds::getInstance()->startMusicGameplay();
				else
					Sounds::getInstance()->startMusicMenu();
			}
			Settings::getInstance()->save();
			Sounds::getInstance()->playClick();
			return true;
		}
		
		// shake?
		if(y > 232 && y <= 288) {
			if(Settings::getInstance()->shakeToRestart) {
				spriteShake->changeImage("options_shake_off");
				Settings::getInstance()->shakeToRestart = false;
			} else {
				spriteShake->changeImage("options_shake_on");
				Settings::getInstance()->shakeToRestart = true;
			}
			Settings::getInstance()->save();
			Sounds::getInstance()->playClick();
			return true;
		}
	}

	return false;
}
