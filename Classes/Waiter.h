#pragma once
#include "cocos2d.h"

class Waiter : public cocos2d::Sprite
{
public:
    static Waiter* create();
    virtual bool init();
    virtual void update(float dt);

    // Метод отримання кадрів зі спрайт-листа
    cocos2d::Vector<cocos2d::SpriteFrame*> getAnimation(const char* format, int count);

    // Прослуховувачі
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

private:
    bool go_l = false; // Чи йдемо вліво
    bool go_r = false; // Чи йдемо вправо
    bool isWalking = false; // Чи йдемо

    cocos2d::Size size; // Розміри вікна
    cocos2d::Animation* walkAnimation = nullptr; // Анімація
    std::string idleFrameName; // Листок спрайтів
};