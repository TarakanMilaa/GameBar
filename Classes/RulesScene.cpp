#include "RulesScene.h"
#include "MainMenuScene.h"

USING_NS_CC;

Scene* RulesScene::createScene()
{
    auto scene = Scene::create();
    auto layer = RulesScene::create();
    scene->addChild(layer);
    return scene;
}

bool RulesScene::init()
{
    if (!Layer::init())
    {
        return false;
    }

    Size size = Director::getInstance()->getWinSize();

    auto background = Sprite::create("rulesBG.png");
    background->setPosition(Vec2(size.width / 2, size.height / 2));
    this->addChild(background);

    // (кнопка) передає картинки звичайного і натиснутого стану, прив'язує функцію
    auto normalBack = Sprite::create("BackButton.png");
    auto selectedBack = Sprite::create("BackButton.png");
    selectedBack->setColor(Color3B(150, 150, 150)); // затемнення натиснутої
    auto backItem = MenuItemSprite::create(normalBack, selectedBack, CC_CALLBACK_1(RulesScene::menuBackCallback, this));
    backItem->setScale(0.5f);
    // розміщуємо кнопку справа внизу під дошкою
    backItem->setPosition(Vec2(size.width - 250, 100));

    // створення менб з кнопкою
    auto menu = Menu::create(backItem, NULL);
    menu->setPosition(Vec2(0, 0));  // позиції 0, бо об'єкт має координати
    this->addChild(menu, 1);

    return true;
}

void RulesScene::menuBackCallback(Ref* pSender)
{
    auto scene = MainMenuScene::createScene(); // сцена головного меню
    Director::getInstance()->replaceScene(scene); // заміна поточної сцени правил на сцену меню
}