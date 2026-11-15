#include "Bartender.h"
#include "Cocktail.h"

USING_NS_CC;

Bartender* Bartender::create()
{
    Bartender* pRet = new(std::nothrow) Bartender();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    delete pRet;
    return nullptr;
}

// Метод отримання кадрів зі спрайт-листа
Vector<SpriteFrame*> Bartender::getAnimation(const char* format, int count)
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

bool Bartender::init()
{
    // Завантажуємо спрайт-лист
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("barman.plist");

    // Витягуємо кадри анімації
    auto frames = getAnimation("Barman_%02d.png", 6);

    if (!Sprite::initWithSpriteFrame(frames.front()))
    {
        return false;
    }

    // Створюємо анімацію
    throwAnimation = Animation::createWithSpriteFrames(frames, 0.1f);
    throwAnimation->retain();

    // Запускаємо  кидок кожні 2.5 секунди 
    this->schedule(schedule_selector(Bartender::throwCocktail), 2.5f);

    idleFrame = frames.front();
    idleFrame->retain();

    return true;
}

void Bartender::throwCocktail(float dt)
{
    auto cocktail = Cocktail::create();
    cocktail->setScale(0.06f);
    this->getParent()->addChild(cocktail, 3);
    cocktail->throwFromBartender(this->getPosition() + Vec2(30, 20), this->getPositionX());

    Animate* animate = Animate::create(throwAnimation);
    auto resetPose = CallFunc::create([this]() {
        this->setSpriteFrame(idleFrame);
        });

    auto seq = Sequence::create(animate, resetPose, nullptr);
    this->runAction(seq);
}