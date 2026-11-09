#include "SetupScene.h"
#include "MainMenuScene.h"
#include "GameScene.h" 
#include "DataManager.h" 

using namespace std;

USING_NS_CC;

Scene* SetupScene::createScene()
{
    auto scene = Scene::create();
    auto layer = SetupScene::create();
    scene->addChild(layer);
    return scene;
}

bool SetupScene::init()
{
    if (!Layer::init())
    {
        return false;
    }

    Size size = Director::getInstance()->getWinSize();
    _selectedSpeed = 1; // рівень 1 за замовчуванням 

    // фон
    auto background = Sprite::create("SetupBG.png");
    background->setPosition(Vec2(size.width / 2, size.height / 2));
    this->addChild(background);

    float boardCenterX = size.width / 2 + 100; // розрахунок для центру дошки

    // спрайт тексту "Введіть ім'я героя"
    auto namePrompt = Sprite::create("InputName.png"); 
    namePrompt->setScale(1.0f);
    namePrompt->setPosition(Vec2(boardCenterX, size.height - 180));
    this->addChild(namePrompt, 2);

    // Рамка 
    auto inputFrame = Sprite::create("FrameText.png");
    inputFrame->setScale(0.2f);
    // Було - 210, стало - 260 (опустили нижче)
    inputFrame->setPosition(Vec2(boardCenterX, size.height - 260));
    this->addChild(inputFrame, 1);

    // Поле введення
    _nameInput = ui::TextField::create(u8"Введіть ім'я...", "arial", 28); // підказка в рамці
    _nameInput->setPosition(Vec2(boardCenterX, size.height - 260)); // поле по центру рамки
    _nameInput->setMaxLengthEnabled(true); // виклик ліміту на к-сть символів
    _nameInput->setMaxLength(12); // максимум 12 символів
    _nameInput->setPlaceHolderColor(Color3B(200, 200, 200)); // сірий колір підказки
    _nameInput->setTextColor(Color4B(255, 215, 0, 255)); // жовтий для введеного тексту
    _nameInput->setTouchEnabled(true); // дозволяємо клікати по полю, щоб активувати
    _nameInput->setCursorEnabled(true); // відображення курсора під час введення 
    this->addChild(_nameInput, 10);

    // Опущено нижче
    float checkboxesY = size.height - 500;

    // спрайт тексту "Оберіть складність гри"
    auto speedPrompt = Sprite::create("InputLvl.png");
    speedPrompt->setScale(1.0f); 
    speedPrompt->setPosition(Vec2(boardCenterX, checkboxesY + 90));
    this->addChild(speedPrompt, 2);

    // лямбда функція для генерації чекбоксів (аби не дублювати код)
    auto createSpeedCheckbox = [&]() {
        return ui::CheckBox::create("check_box_normal.png", "check_box_normal_press.png", "check_box_active.png",
            "check_box_normal_disable.png", "check_box_active_disable.png");
        };

    // Чекбокс 1
    _speed1 = createSpeedCheckbox(); // створення через ф-ю
    _speed1->setScale(0.15f);
    _speed1->setPosition(Vec2(boardCenterX - 180, checkboxesY)); // лівіше від центру
    _speed1->setSelected(true); // встановлення галочки (за замовчуванням)
    _speed1->addEventListener(CC_CALLBACK_2(SetupScene::onSpeedCheckboxClick, this)); // прив'язка ф-ї що прцює від кліку
    this->addChild(_speed1);

    // Спрайт цифри 1 
    auto speed1Sprite = Sprite::create("LvlOne.png");
    speed1Sprite->setScale(0.4f); 
    speed1Sprite->setPosition(Vec2(boardCenterX - 180, checkboxesY - 60));
    this->addChild(speed1Sprite);

    // Чекбокс 2
    _speed2 = createSpeedCheckbox();
    _speed2->setScale(0.15f);
    _speed2->setPosition(Vec2(boardCenterX, checkboxesY)); // по центру
    _speed2->addEventListener(CC_CALLBACK_2(SetupScene::onSpeedCheckboxClick, this)); // прив'язка кліку
    this->addChild(_speed2);

    // Спрайт цифри 2
    auto speed2Sprite = Sprite::create("LvlTwo.png");
    speed2Sprite->setScale(0.4f); 
    speed2Sprite->setPosition(Vec2(boardCenterX, checkboxesY - 60));
    this->addChild(speed2Sprite);

    // Чекбокс 3
    _speed3 = createSpeedCheckbox();
    _speed3->setScale(0.15f);
    _speed3->setPosition(Vec2(boardCenterX + 180, checkboxesY)); // правіше від центру
    _speed3->addEventListener(CC_CALLBACK_2(SetupScene::onSpeedCheckboxClick, this)); // прив'язка кліку
    this->addChild(_speed3);

    // Спрайт цифри 3
    auto speed3Sprite = Sprite::create("LvlThree.png");
    speed3Sprite->setScale(0.4f); 
    speed3Sprite->setPosition(Vec2(boardCenterX + 180, checkboxesY - 60));
    this->addChild(speed3Sprite);

    // старт
    auto normalStart = Sprite::create("StartButton.png"); // спрайт звичайного стану
    auto selectedStart = Sprite::create("StartButton.png"); // спрайт натиснутого стану
    selectedStart->setColor(Color3B(150, 150, 150)); // затемнення другої картинки (імітуємо натискання)
    // створення кнопки з двома станами і функцією старту
    auto startItem = MenuItemSprite::create(normalStart, selectedStart, CC_CALLBACK_1(SetupScene::menuStartCallback, this));
    startItem->setScale(0.5f);
    startItem->setPosition(Vec2(boardCenterX, checkboxesY - 150));

    // назад
    auto normalBack = Sprite::create("BackButton.png"); // спрайт звичайного стану
    auto selectedBack = Sprite::create("BackButton.png"); // спрайт натиснутого стану
    selectedBack->setColor(Color3B(150, 150, 150)); // затемнення другої картинки (імітуємо натискання)
    // створення кнопки і прив'язка ф-ї повернення
    auto backItem = MenuItemSprite::create(normalBack, selectedBack, CC_CALLBACK_1(SetupScene::menuBackCallback, this));
    backItem->setScale(0.35f);
    backItem->setPosition(Vec2(size.width - 150, 90));

    // групуємо старт і назад в меню
    auto menu = Menu::create(startItem, backItem, NULL);
    menu->setPosition(Vec2(0, 0));  // позиції 0, бо об'єкти мають координати
    this->addChild(menu, 10);

    return true;
}
// ф-я яка викликається при кліку на чекбокс
void SetupScene::onSpeedCheckboxClick(Ref* sender, ui::CheckBox::EventType type)
{
    if (type == ui::CheckBox::EventType::SELECTED) // якщо поставили галочку
    {
        // примусове зняття галочки
        _speed1->setSelected(false);
        _speed2->setSelected(false);
        _speed3->setSelected(false);

        auto clicked = (ui::CheckBox*)sender; // визначає на який чекбокс клікнули
        clicked->setSelected(true); // лишається галочка тільки на ньому

        // якщо _ то запис _
        if (clicked == _speed1) _selectedSpeed = 1;
        else if (clicked == _speed2) _selectedSpeed = 2;
        else if (clicked == _speed3) _selectedSpeed = 3;
    }
    else if (type == ui::CheckBox::EventType::UNSELECTED) // якщо зняти галочку з вибраного чекбокса
    {
        auto clicked = (ui::CheckBox*)sender; // знаходить цей чекбокс
        clicked->setSelected(true); // повертає галочку
    }
}

// ф-я яка викликається при кліку на старт
void SetupScene::menuStartCallback(Ref* pSender)
{
    string playerName = _nameInput->getString(); // зчитує введений в поле текст
    if (playerName.empty()) { // перевірка чи рядок порожній
        playerName = "Гість"; // якщо нічого не введено, то встановлюється дефолт ім'я
    }

    UserDefault::getInstance()->setStringForKey("PlayerName", playerName); // зберігає ім'я у файл налаштувань гри
    UserDefault::getInstance()->setIntegerForKey("GameSpeed", _selectedSpeed); // зберігає швидкість
    UserDefault::getInstance()->flush(); // примусове збереження даних на диск

    auto scene = GameScene::createScene(); // створюється ігрова сцена
    Director::getInstance()->replaceScene(scene); // перехід на ігрову сцену
}

// ф-я яка викликається при кліку на назад
void SetupScene::menuBackCallback(Ref* pSender)
{
    auto scene = MainMenuScene::createScene(); 
    Director::getInstance()->replaceScene(scene); // повертається на головне меню
}