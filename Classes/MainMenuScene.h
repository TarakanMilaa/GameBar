#ifndef MAINMENUSCENE_H
#define MAINMENUSCENE_H

#include "cocos2d.h"

class MainMenuScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene(); // об'єкт сцени
    virtual bool init(); // метод початкових налаштувань екрану

    // зворотній виклик при натисканні елемента меню (всі чотири)
    void menuPlayCallback(cocos2d::Ref* pSender);
    void menuRulesCallback(cocos2d::Ref* pSender);
    void menuExitCallback(cocos2d::Ref* pSender);
    void menuStatsCallback(cocos2d::Ref* pSender); 

    CREATE_FUNC(MainMenuScene); // виділення пам'яті, виклик init(), керування пам'яттю
};

#endif