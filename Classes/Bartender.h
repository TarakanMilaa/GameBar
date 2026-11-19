#pragma once
#include "cocos2d.h"

class Bartender : public cocos2d::Sprite
{
public:
    static Bartender* create();
    virtual bool init();

    // Кадр спокою
    cocos2d::SpriteFrame* idleFrame = nullptr;

    // Метод отримання кадрів зі спрайт-листа
    cocos2d::Vector<cocos2d::SpriteFrame*> getAnimation(const char* format, int count);

    // Метод, який викликатиметься для кидка
    void throwCocktail(float dt);

    // Колбек-функція, через яку бармен скаже сцені GameScene: "Я кинув коктейль!"
    std::function<void(cocos2d::Vec2)> onThrowCallback = nullptr;

private:
    cocos2d::Animation* throwAnimation = nullptr;
};