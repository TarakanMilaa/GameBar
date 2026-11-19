#include "GameOverScene.h"

USING_NS_CC;

Scene* GameOverScene::createScene()
{
    return GameOverScene::create();
}

bool GameOverScene::init()
{
    if (!Scene::init())
    {
        return false;
    }

    return true;
}