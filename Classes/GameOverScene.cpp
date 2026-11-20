#include "GameOverScene.h"
#include "MainMenuScene.h"
#include "GameScene.h"

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

	// Готове зображення фону (перевір, щоб назва збігалася з твоїм файлом)
	auto background = Sprite::create("GameOverBG.png");
	background->setPosition(Vec2(size.width / 2, size.height / 2));
	this->addChild(background, 0);

	// Центр чорної дошки "Результати" по осі X
	float boardCenterX = size.width * 0.65f;

	// Зчитуємо ім'я гравця та рахунок за поточну гру з UserDefault
	string playerName = UserDefault::getInstance()->getStringForKey("PlayerName", u8"Гість");
	int finalScore = UserDefault::getInstance()->getIntegerForKey("LastScore", 0);

	// Шлях до піксельного шрифту
	string fontPath = "fonts/PressStart2P-Regular.ttf";

	// Виведення імені гравця (усередині дошки під словом "Результати")
	auto nameLabel = Label::createWithTTF(u8"Гравець: " + playerName, fontPath, 24);
	nameLabel->setTextColor(Color4B(255, 255, 255, 255));
	nameLabel->setPosition(Vec2(boardCenterX, size.height * 0.58f));
	this->addChild(nameLabel, 1);

	// Виведення рахунку
	auto scoreLabel = Label::createWithTTF(u8"Ваш рахунок: " + std::to_string(finalScore), fontPath, 28);
	scoreLabel->setTextColor(Color4B(255, 215, 0, 255));
	scoreLabel->setPosition(Vec2(boardCenterX, size.height * 0.48f));
	this->addChild(scoreLabel, 1);

	// Кнопка "Грати знову" (RestartButton.png) — зліва під дошкою
	auto normalRestart = Sprite::create("RestartButton.png");
	auto selectedRestart = Sprite::create("RestartButton.png");
	selectedRestart->setColor(Color3B(150, 150, 150));
	auto restartItem = MenuItemSprite::create(normalRestart, selectedRestart, CC_CALLBACK_1(GameOverScene::menuRestartCallback, this));
	restartItem->setScale(0.35f);
	restartItem->setPosition(Vec2(boardCenterX, size.height * 0.29f));

	// Кнопка "В меню" (ToMenuButton.png) — справа під дошкою
	auto normalMenu = Sprite::create("ToMenuButton.png");
	auto selectedMenu = Sprite::create("ToMenuButton.png");
	selectedMenu->setColor(Color3B(150, 150, 150));
	auto menuItem = MenuItemSprite::create(normalMenu, selectedMenu, CC_CALLBACK_1(GameOverScene::menuBackCallback, this));
	menuItem->setScale(0.1f);
	menuItem->setPosition(Vec2(size.width - 160, 70));

	// Об'єднуємо обидві кнопки в меню
	auto menu = Menu::create(restartItem, menuItem, NULL);
	menu->setPosition(Vec2(0, 0));
	this->addChild(menu, 1);

	return true;
}

void GameOverScene::menuRestartCallback(Ref* pSender)
{
	auto scene = GameScene::createScene();
	Director::getInstance()->replaceScene(scene);
}

void GameOverScene::menuBackCallback(Ref* pSender)
{
	auto scene = MainMenuScene::createScene();
	Director::getInstance()->replaceScene(scene);
}