#pragma once
#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>
#include "sqlite3_wrapper.h"
#include "game_data.h"

class Settings {
public:
	// return the instance
	static Settings* getInstance();

	// the settings
	bool soundEnabled;
	bool musicEnabled;
	bool shakeToRestart;
	int difficulty;
	int loadCount;
	
	// constructor and destructor
	Settings();
	~Settings();
	static void shutdown();

	// load, save
	void load();
	void save();
	
private:
	static Settings* instance;
};

#endif // SETTINGS_H
