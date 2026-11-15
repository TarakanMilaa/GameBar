#include "GameScene.h"
#include "Waiter.h"
#include "Bartender.h"
#include "Cocktail.h"

USING_NS_CC;

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

    return true;
}


bool GameScene::onCollision(PhysicsContact& contact)
{
    auto nodeA = contact.getShapeA()->getBody()->getNode();
    auto nodeB = contact.getShapeB()->getBody()->getNode();
    if (!nodeA || !nodeB) return false;

    int tagA = nodeA->getTag();
    int tagB = nodeB->getTag();

    if ((tagA == 1 && tagB == 2) || (tagA == 2 && tagB == 1))
    {
        Node* cocktailNode = (tagA == 2) ? nodeA : nodeB;
        cocktailNode->stopAllActions();
        cocktailNode->removeFromParentAndCleanup(true);
    }

    return true;
}