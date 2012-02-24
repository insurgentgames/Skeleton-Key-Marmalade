#include "scene_achievements.h"
#include "IwResManager.h"
#include "settings.h"
#include "sounds.h"
#include "scene_menu.h"
#include "achievements.h"

// menu button
AchievementsButtonMenu::AchievementsButtonMenu() {
	imageNormalName = std::string("achievements_menu");
	imageNormal = IGResourceManager::getInstance()->getImage(imageNormalName);
	imageSelectedName = std::string("achievements_menu2");
	imageSelected = IGResourceManager::getInstance()->getImage(imageSelectedName);
	image = imageNormal;
	this->set(IGPoint(160,452));
	touchSize = IGRect(98,56);
	z = 2;
	tag = 0;
}
void AchievementsButtonMenu::buttonPressed() {
	Sounds::getInstance()->playClick();
}
void AchievementsButtonMenu::buttonReleased() {
	parent->removeAllChildren();
	IGDirector::getInstance()->switchScene(new SceneMenu());
}

// back button
AchievementsButtonBack::AchievementsButtonBack() {
	imageNormalName = std::string("achievements_back");
	imageNormal = IGResourceManager::getInstance()->getImage(imageNormalName);
	imageSelectedName = std::string("achievements_back2");
	imageSelected = IGResourceManager::getInstance()->getImage(imageSelectedName);
	image = imageNormal;
	this->set(IGPoint(55,452));
	touchSize = IGRect(111,56);
	z = 2;
	tag = 0;
}
void AchievementsButtonBack::buttonPressed() {
	Sounds::getInstance()->playClick();
}
void AchievementsButtonBack::buttonReleased() {
	((SceneAchievements*)parent)->pageBack();
}

// next button
AchievementsButtonNext::AchievementsButtonNext() {
	imageNormalName = std::string("achievements_next");
	imageNormal = IGResourceManager::getInstance()->getImage(imageNormalName);
	imageSelectedName = std::string("achievements_next2");
	imageSelected = IGResourceManager::getInstance()->getImage(imageSelectedName);
	image = imageNormal;
	this->set(IGPoint(265,452));
	touchSize = IGRect(111,56);
	z = 2;
	tag = 0;
}
void AchievementsButtonNext::buttonPressed() {
	Sounds::getInstance()->playClick();
}
void AchievementsButtonNext::buttonReleased() {
	((SceneAchievements*)parent)->pageNext();
}

// achievements scene
SceneAchievements::SceneAchievements() {
	IGScene::IGScene();
	IGLog("SceneAchievements init");
	
	// load the resources
	IwGetResManager()->LoadGroup("achievements.group");
	
	// the background
	IGSprite* spriteBackground = new IGSprite("background_wood", IGPoint(160,240), 0);
	this->addChild(spriteBackground);

	// header
	IGSprite* spriteHeader = new IGSprite("achievements_header", IGPoint(160,41), 1);
	this->addChild(spriteHeader);

	// the menu items
	buttonBack = new AchievementsButtonBack();
	buttonBack->setOpacity(64);
	AchievementsButtonMenu* buttonMenu = new AchievementsButtonMenu();
	buttonNext = new AchievementsButtonNext();
	this->addChild(buttonBack);
	this->addChild(buttonMenu);
	this->addChild(buttonNext);

	// pages
	pageNumber = 0;
	numPages = (int)(ACHIEVEMENT_NUM / 5);
	if(ACHIEVEMENT_NUM % 5 > 0)
		numPages++;

	// display achievements
	updateAchievements();
}

SceneAchievements::~SceneAchievements() {
}

void SceneAchievements::pageBack() {
	if(pageNumber > 0) {
		pageNumber--;
		if(pageNumber == 0)
			buttonBack->setOpacity(64);
		buttonNext->setOpacity(255);
		updateAchievements();
	}
}

void SceneAchievements::pageNext() {
	if(pageNumber < numPages-1) {
		pageNumber++;
		if(pageNumber == numPages-1)
			buttonNext->setOpacity(64);
		buttonBack->setOpacity(255);
		updateAchievements();
	}
}

void SceneAchievements::updateAchievements() {
	int i;
	// start by getting rid of all the old achievements
	for(i=0; i<4*5; i++)
		removeChildByTag(AchievementsTagItems+i);

	// figure out what achievements need displaying
	int start = pageNumber*5;
	int end = start+5;
	if(end >= ACHIEVEMENT_NUM)
		end = ACHIEVEMENT_NUM;

	// display them
	int curTag = AchievementsTagItems;
	int curItem = 0;
	for(i=start; i<end; i++) {
		Achievement a = Achievements::getInstance()->getAchievement(i);
		
		// row background
		IGSprite* spriteRowBackground = new IGSprite("achievements_row_background", IGPoint(160,116+68*curItem), 5, curTag);
		this->addChild(spriteRowBackground);
		curTag++;

		// name and description
		IGLabel* labelName = new IGLabel("font_gabriola_16b", a.name.c_str(), IGPoint(196,105+68*curItem), IGRect(232,34), 6, curTag);
		labelName->setColor(255,255,255,255);
		this->addChild(labelName);
		curTag++;
		IGLabel* labelDescription = new IGLabel("font_gabriola_14", a.description.c_str(), IGPoint(196,127+68*curItem), IGRect(232,34), 6, curTag);
		labelDescription->setColor(196,207,226,255);
		this->addChild(labelDescription);
		curTag++;

		// trophy
		if(a.achieved) {
			IGSprite* spriteTrophy = new IGSprite("achievements_trophy", IGPoint(44,116+68*curItem), 6, curTag);
			this->addChild(spriteTrophy);
		} else {
			IGSprite* spriteTrophy = new IGSprite("achievements_locked_trophy", IGPoint(44,116+68*curItem), 6, curTag);
			this->addChild(spriteTrophy);

			// dim everything out
			labelName->setOpacity(128);
			labelDescription->setOpacity(128);
			spriteTrophy->setOpacity(128);
		}
		curTag++;

		curItem++;
	}
}
