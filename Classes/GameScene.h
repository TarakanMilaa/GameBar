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

    // Збереження вказівника на фізичний світ (лекція Л11-Л12, стор. 38)
    void SetPhysicsWorld(cocos2d::PhysicsWorld* world) { sceneWorld = world; }

    // Метод виявлення колізій (лекція Л11-Л12, стор. 43)
    bool onCollision(cocos2d::PhysicsContact& contact);

    CREATE_FUNC(GameScene);

private:
    cocos2d::PhysicsWorld* sceneWorld = nullptr;
    Waiter* _waiter = nullptr;
    Bartender* _bartender = nullptr;
};