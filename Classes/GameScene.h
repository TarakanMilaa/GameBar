#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "cocos2d.h"

class GameScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();

    CREATE_FUNC(GameScene);
};

#endif 
