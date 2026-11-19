#include "GameScene.h"
#include "GameOverScene.h"
#include "DataManager.h"
// ТУТ БРАКУЄ КОДУ ІРИ:
// #include "Waiter.h"
// #include "Bartender.h"
// #include "Cocktail.h"

USING_NS_CC;
using namespace std;

Scene* GameScene::createScene()
{
	// Створення сцени з використанням фізичних законів[cite: 2]
	auto scene = Scene::createWithPhysics(); [cite:1]

		// Встановлення глобальної сили тяжіння[cite: 2]
		scene->getPhysicsWorld()->setGravity(Vec2(0.0f, -98.0f)); [cite:2]

		// Для відображення контурів фізичних форм (дебаг) - розкоментуйте за потреби[cite: 2]
		// scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);[cite: 2]

		auto layer = GameScene::create();
	scene->addChild(layer);

	return scene; [cite:1]
}

bool GameScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size size = Director::getInstance()->getWinSize(); [cite:35]

		DataManager::getInstance()->resetGameData();

	// Фон (потрібно додати картинку GameBG.png у Resources)
	auto background = Sprite::create("GameBG.png");
	if (background) {
		background->setPosition(Vec2(size.width / 2, size.height / 2)); [cite:35]
			this->addChild(background, -1);
	}

	// Текстові мітки[cite: 18]
	_scoreLabel = Label::createWithTTF(u8"Рахунок: 0", "fonts/Marker Felt.ttf", 32); [cite:18]
		_scoreLabel->setPosition(Vec2(120, size.height - 40));
	this->addChild(_scoreLabel, 10);

	_livesLabel = Label::createWithTTF(u8"Життя: 3", "fonts/Marker Felt.ttf", 32); [cite:18]
		_livesLabel->setPosition(Vec2(size.width - 100, size.height - 40));
	this->addChild(_livesLabel, 10);

	// Створення фізичної підлоги для виявлення пропущених коктейлів
	auto ground = Node::create();
	auto groundBody = PhysicsBody::createBox(Size(size.width, 20), PhysicsMaterial(0.1f, 1.0f, 0.0f)); [cite:3]
		groundBody->setDynamic(false); [cite:4] // Статичне тіло[cite: 4]
		groundBody->setCategoryBitmask(0x4); // Категорія підлоги[cite: 6]
	groundBody->setContactTestBitmask(0x2); // Реагує на коктейлі (яким ви задасте 0x2)[cite: 9]
	ground->setPhysicsBody(groundBody); [cite:3]
		ground->setPosition(Vec2(size.width / 2, 0));
	ground->setTag(3); // Тег підлоги[cite: 8]
	this->addChild(ground);

	// ТУТ БРАКУЄ КОДУ ІРИ:
	// 1. Створення об'єкта Bartender і додавання його на сцену
	// 2. Створення об'єкта Waiter і додавання його на сцену
	// (Не забудьте призначити Waiter-у: setTag(1), setCategoryBitmask(0x1), setContactTestBitmask(0x2))

	// Детектування колізій через EventDispatcher[cite: 7]
	auto contactListener = EventListenerPhysicsContact::create(); [cite:7]
		contactListener->onContactBegin = CC_CALLBACK_1(GameScene::onCollision, this); [cite:7]
		this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this); [cite:7]

		// Запуск оновлення сцени кожного кадру[cite: 71]
		this->scheduleUpdate(); [cite:71]

		// Таймер генерації коктейлів (частота залежить від вибраної складності)
		int speedLevel = DataManager::getInstance()->getGameSpeed();
	float spawnRate = 2.0f / speedLevel;
	this->schedule(schedule_selector(GameScene::spawnCocktail), spawnRate); [cite:71]

		return true;
}

void GameScene::spawnCocktail(float dt)
{
	// ТУТ БРАКУЄ КОДУ ІРИ:
	// Тут потрібно викликати метод генерації з класу Bartender.
	// Коли її клас Cocktail буде створюватися, він обов'язково повинен мати:
	// body->setCategoryBitmask(0x2);[cite: 6]
	// body->setContactTestBitmask(0x1 | 0x4); // щоб спрацьовував onContactBegin з офіціантом (1) і підлогою (4)[cite: 9]
	// sprite->setTag(2);[cite: 8]
}

void GameScene::update(float dt)
{
	// Оновлення тексту на екрані
	_scoreLabel->setString(u8"Рахунок: " + to_string(DataManager::getInstance()->score));
	_livesLabel->setString(u8"Життя: " + to_string(DataManager::getInstance()->lives));

	// Перевірка на програш
	if (DataManager::getInstance()->lives <= 0)
	{
		// Зберігаємо рекорд і переходимо на екран програшу
		DataManager::getInstance()->saveHighScore();
		auto scene = GameOverScene::createScene();
		Director::getInstance()->replaceScene(scene); [cite:36]
	}
}

// Ідентифікація колізій під час контакту[cite: 7, 8]
bool GameScene::onCollision(PhysicsContact& contact)
{
	auto nodeA = contact.getShapeA()->getBody()->getNode(); [cite:8]
		auto nodeB = contact.getShapeB()->getBody()->getNode(); [cite:8]

		if (!nodeA || !nodeB) return false;

	int tagA = nodeA->getTag(); [cite:8]
		int tagB = nodeB->getTag(); [cite:8]

		// Теги: 1 - Waiter, 2 - Cocktail, 3 - Ground

		// Колізія: Офіціант і Коктейль
		if ((tagA == 1 && tagB == 2) || (tagA == 2 && tagB == 1))
		{
			// ТУТ БРАКУЄ КОДУ ІРИ:
			// Перевірка типу коктейлю (хороший чи поганий) і нарахування балів або життів.
			// Поки що за замовчуванням просто даємо +10 очок:
			DataManager::getInstance()->score += 10;

			// Видалення коктейлю зі сцени
			if (tagA == 2) nodeA->removeFromParentAndCleanup(true); [cite:7]
			else nodeB->removeFromParentAndCleanup(true); [cite:7]
		}

	// Колізія: Коктейль і Підлога (пропущений напій)
	if ((tagA == 2 && tagB == 3) || (tagA == 3 && tagB == 2))
	{
		DataManager::getInstance()->lives -= 1;

		// Видалення коктейлю зі сцени
		if (tagA == 2) nodeA->removeFromParentAndCleanup(true); [cite:7]
		else nodeB->removeFromParentAndCleanup(true); [cite:7]
	}

	return true; [cite:7]
}