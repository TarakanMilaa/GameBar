#ifndef SETUPSCENE_H
#define SETUPSCENE_H

#include "cocos2d.h"
#include "ui/CocosGUI.h" 

class SetupScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();

    // Метод для обробки вибору швидкості (щоб працювали як одна група)
    void onSpeedCheckboxClick(cocos2d::Ref* sender, cocos2d::ui::CheckBox::EventType type);

    // Кнопки дій
    void menuStartCallback(cocos2d::Ref* pSender);
    void menuBackCallback(cocos2d::Ref* pSender);

    CREATE_FUNC(SetupScene);

private:
    cocos2d::ui::TextField* _nameInput; // Поле введення
    cocos2d::ui::CheckBox* _speed1;    // Чекбокс швидкості 1
    cocos2d::ui::CheckBox* _speed2;    // Чекбокс швидкості 2
    cocos2d::ui::CheckBox* _speed3;    // Чекбокс швидкості 3
    int _selectedSpeed;                // Змінна для збереження вибору (1, 2, 3)
};

#endif#pragma once
