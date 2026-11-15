#include "Cocktail.h"

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
    // 1. Завантажуємо спрайт-лист коктейлів
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("cocktail.plist");

    // 2. Вибираємо випадковий коктейль із 10
    int randomIndex = cocos2d::random(1, 10);
    std::string frameName = StringUtils::format("cocktail%d.png", randomIndex);

    if (!Sprite::initWithSpriteFrameName(frameName))
    {
        return false;
    }

    // 3. Створюємо фізичне тіло коктейлю (лекція Л11-Л12, стор. 38, 41, 43)
    Size bodySize = Size(this->getContentSize().width * 0.06f, this->getContentSize().height * 0.12f);
    auto physics_cocktail = PhysicsBody::createBox(bodySize, PhysicsMaterial(0.0f, 1.0f, 0.0f));
    physics_cocktail->setDynamic(false); // поки в руках бармена, гравітація не діє
    physics_cocktail->setCategoryBitmask(0x1);
    physics_cocktail->setCollisionBitmask(0x1);
    physics_cocktail->setContactTestBitmask(true);
    this->setPhysicsBody(physics_cocktail);

    // Встановлюємо тег 2 для ідентифікації під час
    this->setTag(2);

    // Запускаємо оновлення update(dt)
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

        // 1. Вибираємо випадкову цільову точку X строго всередині екрана (з відступом 100 пікс. від країв)
        float margin = 90.0f;
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
    // Якщо коктейль впав нижче екрана — видаляємо його зі сцени
    if (this->getPositionY() < -100.0f)
    {
        this->removeFromParentAndCleanup(true);
    }
}