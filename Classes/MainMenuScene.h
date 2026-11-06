#ifndef __MAIN_MENU_SCENE_H__
#define __MAIN_MENU_SCENE_H__

#include "cocos2d.h"

class MainMenuScene : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();
    virtual bool init();

    void menuPlayCallback(cocos2d::Ref* pSender);
    void menuRulesCallback(cocos2d::Ref* pSender);
    void menuExitCallback(cocos2d::Ref* pSender);

    CREATE_FUNC(MainMenuScene);
};

#endif