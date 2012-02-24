#include "achievements.h"
#include "ig2d/ig_global.h"
#include "sounds.h"

Achievement::Achievement(std::string _name, std::string _description, bool _achieved) {
	name = _name;
	description = _description;
	achieved = _achieved;
}

Achievements* Achievements::instance = NULL;

Achievements* Achievements::getInstance() {
	if(instance == NULL)
		instance = new Achievements();
	return instance;
}

Achievements::Achievements() {
	IGLog("Achievements init");
	this->load();
}

Achievements::~Achievements() {
}

void Achievements::shutdown() {
	if(instance != NULL)
		delete instance;
}

bool Achievements::unlockAchievement(int achievementId) {
	if(achieved[achievementId] == false) {
		IGLog("Achievements unlocking achievement");
		achieved[achievementId] = true;
		
		// save the achievement
		SQLite3Wrapper db("achievements.db");
		char buffer[100];
		sprintf(buffer, "UPDATE achievement SET achieved='1' WHERE num='%i'; ", achievementId);
		db.exe(buffer);

		// play the unlock sound
		Sounds::getInstance()->playUnlockAchievement();

		return true;
	}
	return false;
}

Achievement Achievements::getAchievement(int achievementId) {
	// get data from database
	SQLite3Wrapper db("achievements.db");
	char buffer[100];
	sprintf(buffer, "SELECT name,description,achieved FROM achievement WHERE num='%i';", achievementId);
	db.exe(buffer);
	std::vector<std::string>::iterator i = db.vdata.begin();
	std::string stringName = *i;
	++i;
	std::string stringDescription = *i;
	++i;
	std::string stringAchieved = *i;
	int intAchieved = atoi(stringAchieved.c_str());
	bool boolAchieved = false;
	if(intAchieved)
		boolAchieved = true;

	// turn it into an Achievement object
	return Achievement(stringName, stringDescription, boolAchieved);
}

void Achievements::resetAchievements() {
	for(int i=0; i<ACHIEVEMENT_NUM; i++) {
		achieved[i] = false;
	}
	SQLite3Wrapper achievementsDB("achievements.db");
	achievementsDB.exe(std::string("UPDATE achievement SET achieved='0'; UPDATE achievement_data SET doors_opened='0',perfect_levels='0',consecutive_levels='0';"));
}

void Achievements::load() {
	SQLite3Wrapper db("achievements.db");
	db.exe("SELECT achieved FROM achievement");
	std::vector<std::string>::iterator i = db.vdata.begin();
	
	for(int j=0; j<ACHIEVEMENT_NUM; j++) {
		std::string str = *i;
		if(str == "0")
			achieved[j] = false;
		else
			achieved[j] = true;
		++i;
	}
}

AchievementData* AchievementData::instance = NULL;

AchievementData* AchievementData::getInstance() {
	if(instance == NULL)
		instance = new AchievementData();
	return instance;
}

AchievementData::AchievementData() {
	IGLog("AchievementData init");

	// load achievement data
	SQLite3Wrapper db("achievements.db");
	db.exe("SELECT doors_opened FROM achievement_data");
	std::vector<std::string>::iterator i = db.vdata.begin();
	std::string stringDoorsOpened = *i;

	// convert to int
	doorsOpened = atoi(stringDoorsOpened.c_str());
}

AchievementData::~AchievementData() {
}

void AchievementData::shutdown() {
	if(instance != NULL)
		delete instance;
}

void AchievementData::save() {
	SQLite3Wrapper db("achievements.db");
	char buffer[100];
	sprintf(buffer, "UPDATE achievement_data SET doors_opened='%i'; ", doorsOpened);
	db.exe(buffer);
}
