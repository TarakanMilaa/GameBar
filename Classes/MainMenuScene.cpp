#include "MainMenuScene.h"
#include "SetupScene.h"
#include "RulesScene.h"
#include "StatsScene.h"
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

    // (кнопка) передає картинки звичайного і натиснутого стану, прив'язує функцію menuPlayCallback
    auto normalPlay = Sprite::create("PlayButton.png");
    auto selectedPlay = Sprite::create("PlayButton.png");
    selectedPlay->setColor(Color3B(150, 150, 150)); // затемнення натиснутої
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

    // (нова кнопка) "Статистика" на барній стійці зліва з програмним затемненням
    auto normalStats = Sprite::create("StatButton.png");
    auto selectedStats = Sprite::create("StatButton.png");
    selectedStats->setColor(Color3B(150, 150, 150)); // затемнення натиснутої
    auto statsItem = MenuItemSprite::create(normalStats, selectedStats, CC_CALLBACK_1(MainMenuScene::menuStatsCallback, this));
    statsItem->setScale(0.13f); // масштабуємо, щоб вона акуратно виглядала на стійці
    // розміщуємо на барній стійці зліва (приблизні координати на основі візуального аналізу інтер'єру)
    statsItem->setPosition(Vec2(170, 360));

    // об'єднання всіх кнопок в об'єкт Меню (додано statsItem)
    auto menu = Menu::create(playItem, rulesItem, exitItem, statsItem, NULL);
    menu->setPosition(Vec2(0, 0)); // позиції 0, бо об'єкти мають координати
    this->addChild(menu, 1);

    return true;
}

void MainMenuScene::menuPlayCallback(Ref* pSender) // "Грати"
{
    auto scene = SetupScene::createScene(); // створює нову сцену
    Director::getInstance()->replaceScene(scene); // повна заміна сцени на іншу
}

void MainMenuScene::menuRulesCallback(Ref* pSender) // "Правила"
{
    auto scene = RulesScene::createScene();
    Director::getInstance()->replaceScene(scene);
}

void MainMenuScene::menuExitCallback(Ref* pSender) // "Вихід"
{
    Director::getInstance()->end();
}

void MainMenuScene::menuStatsCallback(Ref* pSender) // "Статистика"
{
    auto scene = StatsScene::createScene();
    Director::getInstance()->replaceScene(scene);
}