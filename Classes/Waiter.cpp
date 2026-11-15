#include "Waiter.h"

USING_NS_CC;

const int TAG_WALK_ANIM = 1;

Waiter* Waiter::create()
{
    Waiter* pRet = new(std::nothrow) Waiter();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    delete pRet;
    return nullptr;
}

// Метод отримання кадрів зі спрайт-листа
Vector<SpriteFrame*> Waiter::getAnimation(const char* format, int count)
{
    auto spritecache = SpriteFrameCache::getInstance();
    Vector<SpriteFrame*> animFrames;
    char str[100];
    for (int i = 1; i <= count; i++)
    {
        sprintf(str, format, i);
        animFrames.pushBack(spritecache->getSpriteFrameByName(str));
    }
    return animFrames;
}

bool Waiter::init()
{
    // Спрайт-лист
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("waiter.plist");

    // Розбиваємо на 10 кадрів спрайт-лист
    auto frames = getAnimation("Waiter_%02d.png", 10);

    if (!Sprite::initWithSpriteFrame(frames.front()))
    {
        return false;
    }

    size = Director::getInstance()->getVisibleSize();

    // Анімація спрайта
    walkAnimation = Animation::createWithSpriteFrames(frames, 1.0f / 10);
    walkAnimation->retain();

    // Фізичне тіло додаємо
    auto physics_waiter = PhysicsBody::createBox(this->getContentSize(), PhysicsMaterial(0.1f, 1.0f, 0.0f));
    physics_waiter->setDynamic(false);
    physics_waiter->setCategoryBitmask(0x1);
    physics_waiter->setCollisionBitmask(0x1);
    physics_waiter->setContactTestBitmask(true);
    this->setPhysicsBody(physics_waiter);
    this->setTag(1);

    // Прослуховувачі клавіш
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(Waiter::onKeyPressed, this);
    listener->onKeyReleased = CC_CALLBACK_2(Waiter::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    this->scheduleUpdate();

    return true;
}

// Натискання клавіш
void Waiter::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    if (keyCode == EventKeyboard::KeyCode::KEY_D) go_r = true;
    if (keyCode == EventKeyboard::KeyCode::KEY_A) go_l = true;
}

// Відтиск клавіш
void Waiter::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    if (keyCode == EventKeyboard::KeyCode::KEY_D) go_r = false;
    if (keyCode == EventKeyboard::KeyCode::KEY_A) go_l = false;
}

void Waiter::update(float dt)
{
    Point p = this->getPosition();

    // Рух вправо та вліво
    if (go_r)
    {
        this->setPosition(Vec2(p.x + 6, p.y));
        this->setFlippedX(false); // персонаж дивиться вправо
    }
    if (go_l)
    {
        this->setPosition(Vec2(p.x - 6, p.y));
        this->setFlippedX(true);  // персонаж дивиться вліво
    }


    float halfWidth = this->getBoundingBox().size.width / 2; // Кількість px половини спрайта

    // Встановлюємо межі за які не можна виходити
    if (this->getPositionX() < halfWidth)
    {
        this->setPositionX(halfWidth);
    }
    if (this->getPositionX() > size.width - halfWidth)
    {
        this->setPositionX(size.width - halfWidth);
    }

    // Запуск та зупинка анімації
    if (go_r || go_l)
    {
        if (!isWalking)
        {
            isWalking = true;
            Animate* animate = Animate::create(walkAnimation);
            auto repeat = RepeatForever::create(animate);
            repeat->setTag(TAG_WALK_ANIM);
            this->runAction(repeat);
        }
    }
    else
    {
        if (isWalking)
        {
            isWalking = false;
            this->stopAllActionsByTag(TAG_WALK_ANIM);
            this->setSpriteFrame("Waiter_01.png");
        }
    }
}