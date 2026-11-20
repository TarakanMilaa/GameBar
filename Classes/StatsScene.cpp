#include "StatsScene.h"
#include "MainMenuScene.h"

USING_NS_CC;
using namespace std;

Scene* StatsScene::createScene()
{
	auto scene = Scene::create();
	auto layer = StatsScene::create();
	scene->addChild(layer);
	return scene;
}

bool StatsScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size size = Director::getInstance()->getWinSize();

	// Готове зображення фону (підстав свою назву файлу)
	auto background = Sprite::create("StatBG.png");
	background->setPosition(Vec2(size.width / 2, size.height / 2));
	this->addChild(background, 0);

	auto userDef = UserDefault::getInstance();

	// Зчитуємо рекорди для кожного з 3 рівнів швидкості
	int bestScore1 = userDef->getIntegerForKey("BestScore_1", 0);
	string bestName1 = userDef->getStringForKey("BestPlayer_1", u8"---");

	int bestScore2 = userDef->getIntegerForKey("BestScore_2", 0);
	string bestName2 = userDef->getStringForKey("BestPlayer_2", u8"---");

	int bestScore3 = userDef->getIntegerForKey("BestScore_3", 0);
	string bestName3 = userDef->getStringForKey("BestPlayer_3", u8"---");

	// Шлях до шрифту (якщо закинула в папку Resources/fonts/, зміни на "fonts/PressStart2P-Regular.ttf")
	string fontPath = "fonts/PressStart2P-Regular.ttf";

	float boardCenterX = size.width * 0.56f; // центр самої дошки (підкрути 0.55f - 0.57f за потреби)

	// Виводимо результати (координати Y потім легко підженеш під свою картинку)
	auto level1Label = Label::createWithTTF(
		u8"Рівень 1 (Повільний): " + bestName1 + u8" - " + to_string(bestScore1),
		fontPath, 24);
	level1Label->setTextColor(Color4B(255, 255, 255, 255));
	level1Label->setPosition(Vec2(boardCenterX, size.height * 0.68f));
	this->addChild(level1Label, 1);

	auto level2Label = Label::createWithTTF(
		u8"Рівень 2 (Середній): " + bestName2 + u8" - " + to_string(bestScore2),
		fontPath, 24);
	level2Label->setTextColor(Color4B(255, 230, 150, 255));
	level2Label->setPosition(Vec2(boardCenterX, size.height * 0.56f));
	this->addChild(level2Label, 1);

	auto level3Label = Label::createWithTTF(
		u8"Рівень 3 (Швидкий): " + bestName3 + u8" - " + to_string(bestScore3),
		fontPath, 24);
	level3Label->setTextColor(Color4B(255, 180, 120, 255));
	level3Label->setPosition(Vec2(boardCenterX, size.height * 0.44f));
	this->addChild(level3Label, 1);

	// Кнопка повернення в меню
	auto normalBack = Sprite::create("ToMenuButton.png");
	auto selectedBack = Sprite::create("ToMenuButton.png");
	selectedBack->setColor(Color3B(150, 150, 150));
	auto backItem = MenuItemSprite::create(normalBack, selectedBack, CC_CALLBACK_1(StatsScene::menuBackCallback, this));
	backItem->setScale(0.1f);
	backItem->setPosition(Vec2(size.width / 2, size.height * 0.16f));

	auto menu = Menu::create(backItem, NULL);
	menu->setPosition(Vec2(0, 0));
	this->addChild(menu, 1);

	return true;
}

void StatsScene::menuBackCallback(Ref* pSender)
{
	auto scene = MainMenuScene::createScene();
	Director::getInstance()->replaceScene(scene);
}