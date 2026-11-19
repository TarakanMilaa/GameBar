#include "PauseScene.h"
#include "MainMenuScene.h"

USING_NS_CC;

Scene* PauseScene::createScene()
{
	auto scene = Scene::create();
	auto layer = PauseScene::create();
	scene->addChild(layer);
	return scene;
}

bool PauseScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size size = Director::getInstance()->getWinSize();

	// Фон екрана паузи (підстав свою назву файлу)
	auto background = Sprite::create("PauseBG.png");
	background->setPosition(Vec2(size.width / 2, size.height / 2));
	this->addChild(background, 0);

	// Кнопка "Продовжити"
	auto normalResume = Sprite::create("ContinueButton.png");
	auto selectedResume = Sprite::create("ContinueButton.png");
	selectedResume->setColor(Color3B(150, 150, 150));
	auto resumeItem = MenuItemSprite::create(normalResume, selectedResume, CC_CALLBACK_1(PauseScene::menuResumeCallback, this));
	resumeItem->setScale(0.17f);
	resumeItem->setPosition(Vec2(size.width / 2, size.height * 0.60f));

	// Кнопка "В меню"
	auto normalMenu = Sprite::create("ToMenuButton.png");
	auto selectedMenu = Sprite::create("ToMenuButton.png");
	selectedMenu->setColor(Color3B(150, 150, 150));
	auto menuItem = MenuItemSprite::create(normalMenu, selectedMenu, CC_CALLBACK_1(PauseScene::menuToMenuCallback, this));
	menuItem->setScale(0.17f);
	menuItem->setPosition(Vec2(size.width / 2, size.height * 0.40f));

	auto menu = Menu::create(resumeItem, menuItem, NULL);
	menu->setPosition(Vec2(0, 0));
	this->addChild(menu, 1);

	// Повернення в гру на клавішу Esc
	auto keyListener = EventListenerKeyboard::create();
	keyListener->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event* event)
		{
			if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
			{
				this->menuResumeCallback(nullptr);
			}
		};
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyListener, this);

	return true;
}

void PauseScene::menuResumeCallback(Ref* pSender)
{
	// Закриваємо PauseScene — гра миттєво продовжується з того ж місця
	Director::getInstance()->popScene();
}

void PauseScene::menuToMenuCallback(Ref* pSender)
{
	// Спочатку знімаємо паузу зі стека, потім замінюємо GameScene на головне меню
	Director::getInstance()->popScene();
	Director::getInstance()->replaceScene(MainMenuScene::createScene());
}