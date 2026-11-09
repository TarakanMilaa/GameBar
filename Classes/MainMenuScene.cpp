#include "MainMenuScene.h"
#include "SetupScene.h"
#include "RulesScene.h"

USING_NS_CC;

Scene* MainMenuScene::createScene()
{
    auto scene = Scene::create();
    auto layer = MainMenuScene::create();
    scene->addChild(layer);
    return scene;
}

bool MainMenuScene::init()
{
    if (!Layer::init())
    {
        return false;
    }

    Size size = Director::getInstance()->getWinSize();

    auto background = Sprite::create("MainMenu.png");
    background->setPosition(Vec2(size.width / 2, size.height / 2));
    this->addChild(background);

    float boardX = size.width * 0.69f;

    // (êíîïêà) ïåğåäàº êàğòèíêè çâè÷àéíîãî ³ íàòèñíóòîãî ñòàíó, ïğèâ'ÿçóº ôóíêö³ş menuPlayCallback
    auto normalPlay = Sprite::create("PlayButton.png");
    auto selectedPlay = Sprite::create("PlayButton.png");
    selectedPlay->setColor(Color3B(150, 150, 150)); // çàòåìíåííÿ íàòèñíóòî¿
    auto playItem = MenuItemSprite::create(normalPlay, selectedPlay, CC_CALLBACK_1(MainMenuScene::menuPlayCallback, this));
    playItem->setScale(0.5f);
    playItem->setPosition(Vec2(boardX, size.height * 0.65f));

    auto normalRules = Sprite::create("RegulationsButton.png");
    auto selectedRules = Sprite::create("RegulationsButton.png");
    selectedRules->setColor(Color3B(150, 150, 150));
    auto rulesItem = MenuItemSprite::create(normalRules, selectedRules, CC_CALLBACK_1(MainMenuScene::menuRulesCallback, this));
    rulesItem->setScale(0.5f);
    rulesItem->setPosition(Vec2(boardX, size.height * 0.5f));

    auto normalExit = Sprite::create("ExitButton.png");
    auto selectedExit = Sprite::create("ExitButton.png");
    selectedExit->setColor(Color3B(150, 150, 150));
    auto exitItem = MenuItemSprite::create(normalExit, selectedExit, CC_CALLBACK_1(MainMenuScene::menuExitCallback, this));
    exitItem->setScale(0.5f);
    exitItem->setPosition(Vec2(size.width - 250, 100));

    // îá'ºäíàííÿ âñ³õ êíîïîê â îá'ºêò Ìåíş
    auto menu = Menu::create(playItem, rulesItem, exitItem, NULL);
    menu->setPosition(Vec2(0, 0)); // ïîçèö³¿ 0, áî îá'ºêòè ìàşòü êîîğäèíàòè
    this->addChild(menu, 1);

    return true;
}

void MainMenuScene::menuPlayCallback(Ref* pSender) // "Ãğàòè"
{
    auto scene = SetupScene::createScene(); // ñòâîğşº íîâó ñöåíó
    Director::getInstance()->replaceScene(scene); // ïîâíà çàì³íà ñöåíè íà ³íøó
}

void MainMenuScene::menuRulesCallback(Ref* pSender) // "Ïğàâèëà"
{
    auto scene = RulesScene::createScene();
    Director::getInstance()->replaceScene(scene);
}

void MainMenuScene::menuExitCallback(Ref* pSender) // "Âèõ³ä"
{
    Director::getInstance()->end();
}