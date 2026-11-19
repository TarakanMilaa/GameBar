#include "GameOverScene.h"
#include "MainMenuScene.h"

USING_NS_CC;

Scene* GameOverScene::createScene()
{
    auto scene = Scene::create();
    auto layer = GameOverScene::create();
    scene->addChild(layer);
    return scene;
}

bool GameOverScene::init()
{
    if (!Layer::init())
    {
        return false;
    }

    Size size = Director::getInstance()->getWinSize();

    auto background = LayerColor::create(Color4B(30, 20, 15, 255));
    this->addChild(background);

    auto title = Label::createWithSystemFont("GAME OVER", "Arial", 72);
    title->setTextColor(Color4B(255, 212, 132, 255));
    title->setPosition(Vec2(size.width / 2, size.height / 2 + 60));
    this->addChild(title, 1);

    // Back to main menu, same button as in RulesScene
    auto normalBack = Sprite::create("BackButton.png");
    auto selectedBack = Sprite::create("BackButton.png");
    selectedBack->setColor(Color3B(150, 150, 150));
    auto backItem = MenuItemSprite::create(normalBack, selectedBack, CC_CALLBACK_1(GameOverScene::menuBackCallback, this));
    backItem->setScale(0.5f);
    backItem->setPosition(Vec2(size.width / 2, size.height / 2 - 120));

    auto menu = Menu::create(backItem, NULL);
    menu->setPosition(Vec2(0, 0));
    this->addChild(menu, 1);

    return true;
}

void GameOverScene::menuBackCallback(Ref* pSender)
{
    Director::getInstance()->replaceScene(MainMenuScene::createScene());
}
