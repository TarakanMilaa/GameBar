#pragma once
#include "cocos2d.h"

class Cocktail : public cocos2d::Sprite
{
public:
    static Cocktail* create();
    virtual bool init();
    virtual void update(float dt);

    // Метод підйому в руці та кидка барменом
    void throwFromBartender(cocos2d::Vec2 handStartPos, float bartenderX);
};