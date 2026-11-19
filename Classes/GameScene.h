#pragma once
#include "cocos2d.h"
#include "Waiter.h"
#include "Bartender.h"
#include "Cocktail.h"

class GameScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();

    // Збереження вказівника на фізичний світ
    void SetPhysicsWorld(cocos2d::PhysicsWorld* world) { sceneWorld = world; }

    // Метод виявлення колізій
    bool onCollision(cocos2d::PhysicsContact& contact);

    void loseLife();

    CREATE_FUNC(GameScene);

private:
    cocos2d::PhysicsWorld* sceneWorld = nullptr;
    Waiter* _waiter = nullptr;
    Bartender* _bartender = nullptr;

    int score = 0;
    int lives = 3;
    cocos2d::Label* scoreLabel = nullptr;
    cocos2d::Vector<cocos2d::Sprite*> hearts;
};