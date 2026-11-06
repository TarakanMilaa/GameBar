#include "GameScene.h"
#include "MainMenuScene.h"

USING_NS_CC;

Scene* GameScene::createScene() {
    return GameScene::create();
}

bool GameScene::init() {
    if (!Scene::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto background = Sprite::create("rulesBG.png");
    background->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
    background->setScaleX(visibleSize.width / background->getContentSize().width);
    background->setScaleY(visibleSize.height / background->getContentSize().height);
    this->addChild(background, -1);

    auto backItem = MenuItemImage::create(
        "BackButton.png",
        "BackButton.png",
        CC_CALLBACK_1(GameScene::menuBackCallback, this));
    backItem->setPosition(Vec2(origin.x + visibleSize.width * 0.85f, origin.y + visibleSize.height * 0.12f));
    backItem->setScale(0.5f);

    auto menu = Menu::create(backItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    int i;

    return true;
}

void GameScene::menuBackCallback(Ref* pSender) {
    auto scene = MainMenuScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene));
}