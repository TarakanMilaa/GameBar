#include "MainMenuScene.h"
#include "GameScene.h"

USING_NS_CC;

Scene* MainMenuScene::createScene() {
    return MainMenuScene::create();
}

bool MainMenuScene::init() {
    if (!Scene::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto background = Sprite::create("MainMenu.png");
    background->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
    background->setScaleX(visibleSize.width / background->getContentSize().width);
    background->setScaleY(visibleSize.height / background->getContentSize().height);
    this->addChild(background, -1);

    auto playItem = MenuItemImage::create(
        "PlayButton.png",
        "PlayButton.png",
        CC_CALLBACK_1(MainMenuScene::menuPlayCallback, this));
    playItem->setPosition(Vec2(origin.x + visibleSize.width * 0.69f, origin.y + visibleSize.height * 0.63f));
    playItem->setScale(0.5f);

    auto rulesItem = MenuItemImage::create(
        "RegulationsButton.png",
        "RegulationsButton.png",
        CC_CALLBACK_1(MainMenuScene::menuRulesCallback, this));
    rulesItem->setPosition(Vec2(origin.x + visibleSize.width * 0.69f, origin.y + visibleSize.height * 0.48f));
    rulesItem->setScale(0.5f);

    auto exitItem = MenuItemImage::create(
        "ExitButton.png",
        "ExitButton.png",
        CC_CALLBACK_1(MainMenuScene::menuExitCallback, this));
    exitItem->setPosition(Vec2(origin.x + visibleSize.width * 0.85f, origin.y + visibleSize.height * 0.12f));
    exitItem->setScale(0.5f);

    auto menu = Menu::create(playItem, rulesItem, exitItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    return true;
}

void MainMenuScene::menuPlayCallback(Ref* pSender) {
    auto scene = GameScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene));
}

void MainMenuScene::menuRulesCallback(Ref* pSender) {
    auto scene = GameScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene));
}

void MainMenuScene::menuExitCallback(Ref* pSender) {
    Director::getInstance()->end();
    int i;
}

