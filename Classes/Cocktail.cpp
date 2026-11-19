#include "Cocktail.h"
#include "GameScene.h"

USING_NS_CC;

Cocktail* Cocktail::create()
{
    Cocktail* pRet = new(std::nothrow) Cocktail();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    delete pRet;
    return nullptr;
}

bool Cocktail::init()
{
    // 1. Завантажуємо всі три спрайт-листи
    auto cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("cocktail.plist");
    cache->addSpriteFramesWithFile("poison.plist");
    cache->addSpriteFramesWithFile("health.plist");

    // 2. Дізнаємося поточну кількість життів гравця
    int currentLives = 3;
    auto runningScene = Director::getInstance()->getRunningScene();
    if (runningScene)
    {
        // GameScene is a child of the physics scene, not the running scene itself
        for (auto child : runningScene->getChildren())
        {
            auto gameScene = dynamic_cast<GameScene*>(child);
            if (gameScene)
            {
                currentLives = gameScene->getLives();
                break;
            }
        }
    }

    // 3. Вибираємо тип коктейлю (0..9):
    // 0-6: хороший (70%), 7-8: поганий (20%), 9: коктейль життя (10%, тільки якщо lives < 3)
    int typeChance = cocos2d::random(0, 9);
    int tagToSet = TAG_GOOD_COCKTAIL;
    std::string frameName;

    if (typeChance == 9 && currentLives < 3)
    {
        // Коктейль життя
        isBad = true;
        tagToSet = TAG_HEAL_COCKTAIL;
        frameName = "health.png";
    }
    else if (typeChance == 7 || typeChance == 8)
    {
        // Коктейль смерті
        isBad = true;
        tagToSet = TAG_BAD_COCKTAIL;
        int randomIndex = cocos2d::random(1, 3);
        frameName = StringUtils::format("poison%d.png", randomIndex);
    }
    else
    {
        // Хороший коктейль (0-6, або якщо випало 9 при повних 3 серцях)
        isBad = false;
        tagToSet = TAG_GOOD_COCKTAIL;
        int randomIndex = cocos2d::random(1, 10);
        frameName = StringUtils::format("cocktail%d.png", randomIndex);
    }

    // 4. Ініціалізуємо спрайт вибраним кадром зі спрайт-листа
    if (!Sprite::initWithSpriteFrameName(frameName))
    {
        return false;
    }

    // 5. Створюємо фізичне тіло коктейлю
    Size bodySize = Size(this->getContentSize().width * 0.06f, this->getContentSize().height * 0.12f);
    auto physics_cocktail = PhysicsBody::createBox(bodySize, PhysicsMaterial(0.0f, 1.0f, 0.0f));
    physics_cocktail->setDynamic(false);
    physics_cocktail->setCategoryBitmask(0x1);
    physics_cocktail->setCollisionBitmask(0x1);
    physics_cocktail->setContactTestBitmask(true);
    this->setPhysicsBody(physics_cocktail);

    // 6. Встановлюємо тег для колізій та запускаємо update(dt)
    this->setTag(tagToSet);
    this->scheduleUpdate();

    return true;
}

void Cocktail::throwFromBartender(Vec2 handStartPos, float bartenderX)
{
    // Ставимо коктейль у початкову позицію рук бармена
    this->setPosition(handStartPos);

    // 1. Підіймаємо коктейль разом із руками бармена під час замаху
    auto liftWithHands = MoveBy::create(0.3f, Vec2(0, 45));

    // 2. Після замаху вмикаємо фізику та задаємо швидкість кидка
    auto launch = CallFunc::create([this, bartenderX]() {
        auto visibleSize = Director::getInstance()->getVisibleSize();

        // 1. Вибираємо випадкову цільову точку X строго всередині екрана
        float margin = 150.0f; // відступ від країв сцени
        float noDropRadius = 180.0f; // зона біля бармена, куди не кидаємо
        float targetX = 0.0f;

        do {
            targetX = cocos2d::random(margin, visibleSize.width - margin);
        } while (std::abs(targetX - bartenderX) < noDropRadius);

        // 2. Висота підкидання вгору
        float vy = cocos2d::random(150.0f, 230.0f);

        // 3. Розраховуємо точну горизонтальну швидкість vx, щоб приземлитися в targetX:
        // При гравітації -98.0f коктейль летить до підлоги приблизно 4.0 - 4.5 секунди
        float flightTime = 4.6f;
        float vx = (targetX - this->getPositionX()) / flightTime;

        this->getPhysicsBody()->setDynamic(true);
        this->getPhysicsBody()->setVelocity(Vec2(vx, vy));
        this->getPhysicsBody()->applyTorque((vx > 0) ? -2000 : 2000);
        });

    // Послідовність: підйом у руці -> кидок
    auto seq = Sequence::create(liftWithHands, launch, nullptr);
    this->runAction(seq);
}

void Cocktail::update(float dt)
{
    if (this->getPositionY() < -100.0f)
    {
        // Знімаємо життя ТІЛЬКИ якщо пропущений коктейль НЕ поганий (!isBad)
        if (!isBad && onMissCallback)
        {
            auto callback = onMissCallback;
            onMissCallback = nullptr;
            callback();
        }
        this->removeFromParentAndCleanup(true);
    }
}