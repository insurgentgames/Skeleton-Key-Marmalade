#pragma once
#ifndef ACHIEVEMENTS_H
#define ACHIEVEMENTS_H

#include <string>
#include "sqlite3_wrapper.h"

// the achievements
typedef enum {
	AchievementBeginTheHunt = 0,
	AchievementKnockKnock = 1,
	AchievementTheDoorman = 2,
	AchievementTheDoorToKnowhere = 3,
	AchievementPerfectionist = 4,
	AchievementProfessionalPerfectionist = 5,
	AchievementEnterTheDarkness = 6,
	AchievementEnjoyTheSun = 7,
	AchievementArrrgh = 8,
	AchievementTreasureHunter = 9,
	AchievementAnAdventurerIsYou = 10,
	AchievementIntrepidExplorer = 11,
	AchievementMastermind = 12
} AchievementNames;

#define ACHIEVEMENT_NUM 13

class Achievement {
public:
	Achievement(std::string _name, std::string _description, bool _achieved);
	std::string name;
	std::string description;
	bool achieved;
};

class Achievements {
public:
	// return the instance
	static Achievements* getInstance();
	
	// constructor and destructor
	Achievements();
	~Achievements();
	static void shutdown();

	// achievements
	bool achieved[ACHIEVEMENT_NUM];
	bool unlockAchievement(int achievementId);
	Achievement getAchievement(int achievementId);
	void resetAchievements();

	// load
	void load();
	
private:
	static Achievements* instance;
};

class AchievementData {
public:
	// return the instance
	static AchievementData* getInstance();
	
	// constructor and destructor
	AchievementData();
	~AchievementData();
	static void shutdown();

	// data
	int doorsOpened;
	void save();
	
private:
	static AchievementData* instance;
};

#endif // ACHIEVEMENTS_H
