#include "Bartender.h"
#include "Cocktail.h"
#include "GameScene.h"

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

    // Зчитуємо рівень гри
    int gameSpeed = UserDefault::getInstance()->getIntegerForKey("GameSpeed", 1);

    float throwInterval = 2.5f; // інтервал між кидками
    float frameDelay = 0.1f; // затримка між кадрами анімації

    if (gameSpeed == 2)
    {
        throwInterval = 2.0f;
        frameDelay = 0.08f;
    }
    else if (gameSpeed == 3)
    {
        throwInterval = 1.5f;
        frameDelay = 0.06f;
    }

    throwAnimation = Animation::createWithSpriteFrames(frames, frameDelay);
    throwAnimation->retain();

    this->schedule(schedule_selector(Bartender::throwCocktail), throwInterval);

    idleFrame = frames.front();
    idleFrame->retain();

    return true;
}

void Bartender::throwCocktail(float dt)
{
    auto cocktail = Cocktail::create();
    cocktail->onMissCallback = [this]() {
        auto scene = dynamic_cast<GameScene*>(this->getParent());
        if (scene)
        {
            scene->loseLife();
        }
        };
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