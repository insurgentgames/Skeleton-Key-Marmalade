#include "settings.h"
#include "ig2d/ig_global.h"

Settings* Settings::instance = NULL;

Settings* Settings::getInstance() {
	if(instance == NULL)
		instance = new Settings();
	return instance;
}

Settings::Settings() {
	IGLog("Settings init");
	this->load();
}

Settings::~Settings() {
}

void Settings::shutdown() {
	if(instance != NULL)
		delete instance;
}

void Settings::load() {
	SQLite3Wrapper db("settings.db");
	db.exe("SELECT * FROM settings");
	std::vector<std::string>::iterator i = db.vdata.begin();

	// sound
	std::string strSound = *i;
	if(strSound == "0")
		soundEnabled = false;
	else
		soundEnabled = true;
	
	// music
	++i;
	std::string strMusic = *i;
	if(strMusic == "0")
		musicEnabled = false;
	else
		musicEnabled = true;

	// shake to restart
	++i;
	std::string strShake = *i;
	if(strShake == "0")
		shakeToRestart = false;
	else
		shakeToRestart = true;

	// difficulty
	++i;
	std::string strDifficulty = *i;
	if(strDifficulty == "0")
		difficulty = GameDifficultyEasy;
	else if(strDifficulty == "1")
		difficulty = GameDifficultyMedium;
	else
		difficulty = GameDifficultyHard;
	
	// load count
	++i;
	std::string strLoadCount = *i;
	loadCount = atoi(strLoadCount.c_str());
}

void Settings::save() {
	SQLite3Wrapper db("settings.db");
	char buffer[100];
	sprintf(buffer, "UPDATE settings SET sound='%i',music='%i',shake='%i',difficulty='%i',load_count='%i'", 
		(soundEnabled ? 1 : 0), (musicEnabled ? 1 : 0), (musicEnabled ? 1 : 0), difficulty, loadCount);
	db.exe(std::string(buffer));
}
