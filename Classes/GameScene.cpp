#include "GameScene.h"
#include "Waiter.h"
#include "Bartender.h"
#include "Cocktail.h"
#include "GameOverScene.h"
#include "PauseScene.h"

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

    /*/  нопка паузи
    auto normalPause = Sprite::create("PauseButton.png");
    auto selectedPause = Sprite::create("PauseButton.png");
    selectedPause->setColor(Color3B(150, 150, 150));
    auto pauseItem = MenuItemSprite::create(normalPause, selectedPause, CC_CALLBACK_1(GameScene::menuPauseCallback, this));
    pauseItem->setScale(0.3f);
    pauseItem->setPosition(Vec2(size.width - 60, size.height - 60));

    auto pauseMenu = Menu::create(pauseItem, NULL);
    pauseMenu->setPosition(Vec2(0, 0));
    this->addChild(pauseMenu, 10);*/

    // ќф≥ц≥ант
    auto waiter = Waiter::create();
    waiter->setPosition(Vec2(size.width / 2, 200));
    waiter->setScale(0.2f);
    this->addChild(waiter, 3);

    // Ѕармен
    auto bartender = Bartender::create();
    bartender->setScale(0.1f);
    bartender->setPosition(Vec2(210, 450));
    this->addChild(bartender, 1);

    // Ќижн≥й шар фону
    auto buttonBackground = Sprite::create("GameBG.png", Rect(0, 590, 1703, 340));
    float heightButtonBG = buttonBackground->getBoundingBox().size.height;

    buttonBackground->setPosition(Vec2(size.width / 2, heightButtonBG / 2 - 8));
    this->addChild(buttonBackground, 2);

    // —творенн€ слухача ф≥зики
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(GameScene::onCollision, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

    // Ћ≥чильник очок
    score = 0;
    scoreLabel = Label::createWithTTF("Score: 0", "fonts/PressStart2P-Regular.ttf", 26);
    scoreLabel->setAnchorPoint(Vec2(0, 0.5f));
    scoreLabel->setPosition(540, 750);
    scoreLabel->setTextColor(cocos2d::Color4B(255, 212, 132, 255));
    this->addChild(scoreLabel, 2);

    // —ерц€
    lives = 3;
    for (int i = 0; i < 3; i++)
    {
        auto heart = Sprite::create("heart.png");
        heart->setScale(0.2f);
        heart->setPosition(120 + (i * 100), 50);
        this->addChild(heart, 5);
        hearts.pushBack(heart);
    }

    // ѕауза на клав≥шу Esc
    auto keyListener = EventListenerKeyboard::create();
    keyListener->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event* event)
        {
            if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
            {
                this->menuPauseCallback(nullptr);
            }
        };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyListener, this);

    return true;
}

bool GameScene::onCollision(PhysicsContact& contact)
{
    auto nodeA = contact.getShapeA()->getBody()->getNode();
    auto nodeB = contact.getShapeB()->getBody()->getNode();
    if (!nodeA || !nodeB) return false;

    int tagA = nodeA->getTag();
    int tagB = nodeB->getTag();

    // 1. ќф≥ц≥ант (1) зловив ’ќ–ќЎ»… коктейль (2) -> +1 очко
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
    // 2. ќф≥ц≥ант (1) зловив  ќ “≈…Ћ№ —ћ≈–“≤ (3) -> -1 житт€
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
    // 3. ќф≥ц≥ант зловив ц≥лющий коктейль -> +1 житт€
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

    // «ам≥нюЇмо картинку в≥дпов≥дного серц€ на розбите
    if (lives >= 0 && lives < hearts.size())
    {
        hearts.at(lives)->setTexture("brokenheart.png");
    }

    // якщо житт€ зак≥нчилис€ Ч збер≥гаЇмо рахунок ≥ переходимо на сцену GameOver
    if (lives <= 0)
    {
        auto userDef = UserDefault::getInstance();

        // «бер≥гаЇмо рахунок за поточну гру
        userDef->setIntegerForKey("LastScore", score);

        // Ѕеремо поточну швидк≥сть
        int speed = userDef->getIntegerForKey("GameSpeed", 1);
        string playerName = userDef->getStringForKey("PlayerName", u8"√≥сть");

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
    if (lives <= 0 || lives >= 3) return;

    if (lives >= 0 && lives < hearts.size())
    {
        hearts.at(lives)->setTexture("heart.png");
    }

    lives++;
}

void GameScene::menuPauseCallback(Ref* pSender)
{
    //  ладемо PauseScene поверх GameScene Ч гра автоматично заморожуЇтьс€
    Director::getInstance()->pushScene(PauseScene::createScene());
}