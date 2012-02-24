#include "scene_instructions.h"
#include "IwResManager.h"
#include "settings.h"
#include "sounds.h"
#include "scene_menu.h"

// menu button
InstructionsButtonMenu::InstructionsButtonMenu() {
	imageNormalName = std::string("instructions_menu");
	imageNormal = IGResourceManager::getInstance()->getImage(imageNormalName);
	imageSelectedName = std::string("instructions_menu2");
	imageSelected = IGResourceManager::getInstance()->getImage(imageSelectedName);
	image = imageNormal;
	this->set(IGPoint(160,452));
	touchSize = IGRect(98,56);
	z = 2;
	tag = 0;
}
void InstructionsButtonMenu::buttonPressed() {
	Sounds::getInstance()->playClick();
}
void InstructionsButtonMenu::buttonReleased() {
	parent->removeAllChildren();
	IGDirector::getInstance()->switchScene(new SceneMenu());
}

// back button
InstructionsButtonBack::InstructionsButtonBack() {
	imageNormalName = std::string("instructions_back");
	imageNormal = IGResourceManager::getInstance()->getImage(imageNormalName);
	imageSelectedName = std::string("instructions_back2");
	imageSelected = IGResourceManager::getInstance()->getImage(imageSelectedName);
	image = imageNormal;
	this->set(IGPoint(55,452));
	touchSize = IGRect(111,56);
	z = 2;
	tag = 0;
}
void InstructionsButtonBack::buttonPressed() {
	Sounds::getInstance()->playClick();
}
void InstructionsButtonBack::buttonReleased() {
	((SceneInstructions*)parent)->pageBack();
}

// next button
InstructionsButtonNext::InstructionsButtonNext() {
	imageNormalName = std::string("instructions_next");
	imageNormal = IGResourceManager::getInstance()->getImage(imageNormalName);
	imageSelectedName = std::string("instructions_next2");
	imageSelected = IGResourceManager::getInstance()->getImage(imageSelectedName);
	image = imageNormal;
	this->set(IGPoint(265,452));
	touchSize = IGRect(111,56);
	z = 2;
	tag = 0;
}
void InstructionsButtonNext::buttonPressed() {
	Sounds::getInstance()->playClick();
}
void InstructionsButtonNext::buttonReleased() {
	((SceneInstructions*)parent)->pageNext();
}

// instructions scene
SceneInstructions::SceneInstructions() {
	IGScene::IGScene();
	IGLog("SceneInstructions init");

	// load the resources
	IwGetResManager()->LoadGroup("instructions.group");
	
	// the background
	IGSprite* spriteBackground = new IGSprite("background_forest_dark", IGPoint(160,240), 0);
	this->addChild(spriteBackground);

	// header
	IGSprite* spriteHeader = new IGSprite("instructions_header", IGPoint(160,41), 1);
	this->addChild(spriteHeader);

	// the menu items
	buttonBack = new InstructionsButtonBack();
	buttonBack->setOpacity(64);
	InstructionsButtonMenu* buttonMenu = new InstructionsButtonMenu();
	buttonNext = new InstructionsButtonNext();
	this->addChild(buttonBack);
	this->addChild(buttonMenu);
	this->addChild(buttonNext);

	// the page sprite
	pageNumber = 1;
	spritePage = new IGSprite("instructions_page1", IGPoint(160,253));
	this->addChild(spritePage);
}

SceneInstructions::~SceneInstructions() {
}

void SceneInstructions::pageBack() {
	if(pageNumber > 1) {
		pageNumber--;
		if(pageNumber == 1)
			buttonBack->setOpacity(64);
		buttonNext->setOpacity(255);
		changePageImage();
	}
}

void SceneInstructions::pageNext() {
	if(pageNumber < 4) {
		pageNumber++;
		if(pageNumber == 4)
			buttonNext->setOpacity(64);
		buttonBack->setOpacity(255);
		changePageImage();
	}
}

void SceneInstructions::changePageImage() {
	switch(pageNumber) {
		case 1:
			spritePage->changeImage("instructions_page1");
			break;
		case 2:
			spritePage->changeImage("instructions_page2");
			break;
		case 3:
			spritePage->changeImage("instructions_page3");
			break;
		case 4:
			spritePage->changeImage("instructions_page4");
			break;
	}
}
