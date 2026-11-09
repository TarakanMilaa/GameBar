#pragma once
#ifndef RULESSCENE_H
#define RULESSCENE_H

#include "cocos2d.h"

class RulesScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init(); //метод розміщення елементів на екрані

    void menuBackCallback(cocos2d::Ref* pSender); // зворотній виклик натискання кнопки

    CREATE_FUNC(RulesScene); // створює метод create() і керує пам'яттю
};

#endif 