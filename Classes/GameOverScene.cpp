#include "GameOverScene.h"
#include "SetupScene.h"
#include "MainMenuScene.h"
#include "DataManager.h"

USING_NS_CC;
using namespace std;

Scene* GameOverScene::createScene()
{
	auto scene = Scene::create();
		auto layer = GameOverScene::create();
	scene->addChild(layer);
	return scene;
}

bool GameOverScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size size = Director::getInstance()->getWinSize(); 

		auto background = Sprite::create("GameOverBG.png");
	if (background) {
		background->setPosition(Vec2(size.width / 2, size.height / 2)); 
			this->addChild(background);
	}

	// Текстові надписи[cite: 18]
	string pName = DataManager::getInstance()->getPlayerName();
	int pScore = DataManager::getInstance()->score;
	auto scoreLabel = Label::createWithTTF(pName + u8", ваш рахунок: " + to_string(pScore), "fonts/Marker Felt.ttf", 36); 
		scoreLabel->setPosition(Vec2(size.width / 2, size.height / 2 + 50));
	this->addChild(scoreLabel);

	string bestName = UserDefault::getInstance()->getStringForKey("HighScorer", "Невідомо");
	int bestScore = DataManager::getInstance()->getHighScore();
	auto topLabel = Label::createWithTTF(u8"Рекорд: " + bestName + " - " + to_string(bestScore), "fonts/Marker Felt.ttf", 28); 
		topLabel->setPosition(Vec2(size.width / 2, size.height / 2 - 10));
	topLabel->setColor(Color3B(255, 215, 0)); // Жовтий колір[cite: 97]
	this->addChild(topLabel);

	// Кнопка "Спробувати ще" (стиль як у вас в MainMenu)
	auto normalRetry = Sprite::create("StartButton.png"); // Замініть на свою картинку
	auto selectedRetry = Sprite::create("StartButton.png");
	selectedRetry->setColor(Color3B(150, 150, 150)); 
		auto retryItem = MenuItemSprite::create(normalRetry, selectedRetry, CC_CALLBACK_1(GameOverScene::menuRetryCallback, this));
	retryItem->setScale(0.5f); 
		retryItem->setPosition(Vec2(size.width / 2, size.height / 2 - 120));

	// Кнопка "Головне меню"
	auto normalMain = Sprite::create("BackButton.png"); // Замініть на свою картинку
	auto selectedMain = Sprite::create("BackButton.png");
	selectedMain->setColor(Color3B(150, 150, 150)); 
		auto mainItem = MenuItemSprite::create(normalMain, selectedMain, CC_CALLBACK_1(GameOverScene::menuMainCallback, this));
	mainItem->setScale(0.5f);
		mainItem->setPosition(Vec2(size.width / 2, size.height / 2 - 200));

	auto menu = Menu::create(retryItem, mainItem, NULL); 
		menu->setPosition(Vec2(0, 0)); 
		this->addChild(menu, 1); 

		return true;
}

void GameOverScene::menuRetryCallback(Ref* pSender)
{
	// Замінює сцену на налаштування[cite: 36]
	auto scene = SetupScene::createScene();
	Director::getInstance()->replaceScene(scene);
}

void GameOverScene::menuMainCallback(Ref* pSender)
{
	// Повертає в головне меню[cite: 36]
	auto scene = MainMenuScene::createScene();
	Director::getInstance()->replaceScene(scene); 
}