#include "GameScene.h"
#include "Waiter.h"
#include "Bartender.h"
#include "Cocktail.h"
#include "GameOverScene.h"

USING_NS_CC;
using namespace std;

Scene* GameScene::createScene()
{
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setGravity(Vec2(0.0f, -98.0f));

    auto layer = GameScene::create();
   layer->SetPhysicsWorld(scene->getPhysicsWorld());
    scene->addChild(layer);
    return scene;
}

bool GameScene::init()
{
    if (!Scene::init())
    {
        return false;
    }

    Size size = Director::getInstance()->getWinSize();

    // фон
    auto background = Sprite::create("GameBG.png");
    background->setPosition(Vec2(size.width / 2, size.height / 2));
    this->addChild(background, 0);

    // Офіціант
    auto waiter = Waiter::create();
    waiter->setPosition(Vec2(size.width / 2, 200));
    waiter->setScale(0.2f);
    this->addChild(waiter, 3);

    // Бармен
    auto bartender = Bartender::create();
    bartender->setScale(0.1f);
    bartender->setPosition(Vec2(210, 450));
    this->addChild(bartender, 1);

    // Нижній шар фону
    auto buttonBackground = Sprite::create("GameBG.png", Rect(0, 590, 1703, 340));
    float heightButtonBG = buttonBackground->getBoundingBox().size.height;

    buttonBackground->setPosition(Vec2(size.width / 2, heightButtonBG / 2 - 8));
    this->addChild(buttonBackground, 2);

    // Створення слухача фізики
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(GameScene::onCollision, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

    // Лічильник очок
    score = 0;
    scoreLabel = Label::createWithSystemFont("Score: 0", "Arial", 36);
    scoreLabel->setAnchorPoint(Vec2(0, 0.5f));
    scoreLabel->setPosition(540, 750);
    scoreLabel->setTextColor(cocos2d::Color4B(255, 212, 132, 255));
    this->addChild(scoreLabel, 2);

    // Серця
    lives = 3;
    for (int i = 0; i < 3; i++)
    {
        auto heart = Sprite::create("heart.png");
        heart->setScale(0.2f);
        heart->setPosition(120 + (i * 100), 50);
        this->addChild(heart, 5);
        hearts.pushBack(heart);
    }

    return true;
}


bool GameScene::onCollision(PhysicsContact& contact)
{
    auto nodeA = contact.getShapeA()->getBody()->getNode();
    auto nodeB = contact.getShapeB()->getBody()->getNode();
    if (!nodeA || !nodeB) return false;

    int tagA = nodeA->getTag();
    int tagB = nodeB->getTag();

    // 1. Офіціант (1) зловив ХОРОШИЙ коктейль (2) -> +1 очко
    if ((tagA == 1 && tagB == 2) || (tagA == 2 && tagB == 1))
    {
        Node* cocktailNode = (tagA == 2) ? nodeA : nodeB;
        cocktailNode->setTag(0);
        cocktailNode->setVisible(false);

        cocktailNode->scheduleOnce([cocktailNode](float dt) {
            cocktailNode->removeFromParentAndCleanup(true);
            }, 0.0f, "remove_cocktail");

        score++;
        scoreLabel->setString("Score: " + std::to_string(score));
    }
    // 2. Офіціант (1) зловив КОКТЕЙЛЬ СМЕРТІ (3) -> -1 життя
    else if ((tagA == 1 && tagB == 3) || (tagA == 3 && tagB == 1))
    {
        Node* cocktailNode = (tagA == 3) ? nodeA : nodeB;
        cocktailNode->setTag(0);
        cocktailNode->setVisible(false);

        cocktailNode->scheduleOnce([cocktailNode](float dt) {
            cocktailNode->removeFromParentAndCleanup(true);
            }, 0.0f, "remove_bad_cocktail");

        this->loseLife();
    }
    // 3. Офіціант зловив цілющий коктейль -> +1 життя
    else if ((tagA == TAG_WAITER && tagB == TAG_HEAL_COCKTAIL) ||
        (tagA == TAG_HEAL_COCKTAIL && tagB == TAG_WAITER))
    {
        Node* cocktailNode = (tagA == TAG_HEAL_COCKTAIL) ? nodeA : nodeB;
        cocktailNode->setTag(TAG_NONE);
        cocktailNode->setVisible(false);

        cocktailNode->scheduleOnce([cocktailNode](float dt) {
            cocktailNode->removeFromParentAndCleanup(true);
            }, 0.0f, "remove_heal_cocktail");

        this->gainLife();
    }

    return true;
}

void GameScene::loseLife()
{
    if (lives <= 0) return;

    lives--;

    // Замінюємо картинку відповідного серця на розбите
    if (lives >= 0 && lives < hearts.size())
    {
        hearts.at(lives)->setTexture("brokenheart.png"); // підстав назву свого файлу розбитого серця!
    }

    // Якщо життя закінчилися — переходимо на сцену GameOver
	// Якщо життя закінчилися — зберігаємо рахунок і переходимо на сцену GameOver
	if (lives <= 0)
	{
		auto userDef = UserDefault::getInstance();

		// Зберігаємо рахунок за поточну гру
		userDef->setIntegerForKey("LastScore", score);

		// Беремо поточну швидкість (перевір, щоб ключ збігався з тим, що в SetupScene.cpp!)
		int speed = userDef->getIntegerForKey("GameSpeed", 1);
		string playerName = userDef->getStringForKey("PlayerName", u8"Гість");

		string scoreKey = "BestScore_" + std::to_string(speed);
		string nameKey = "BestPlayer_" + std::to_string(speed);

		int currentBest = userDef->getIntegerForKey(scoreKey.c_str(), 0);
		if (score > currentBest)
		{
			userDef->setIntegerForKey(scoreKey.c_str(), score);
			userDef->setStringForKey(nameKey.c_str(), playerName);
		}

		userDef->flush();

		auto gameOverScene = GameOverScene::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(0.5f, gameOverScene));
	}
}

void GameScene::gainLife()
{
    // Відновлюємо життя, тільки якщо їх менше максимуму (3) і гра ще не програна
    if (lives <= 0 || lives >= 3) return;

    // Оскільки перед втратою ми робили lives-- і брали hearts.at(lives),
    // то зараз поточне значення lives якраз вказує на перше розбите серце!
    if (lives >= 0 && lives < hearts.size())
    {
        hearts.at(lives)->setTexture("heart.png"); 
    }

    lives++;
}