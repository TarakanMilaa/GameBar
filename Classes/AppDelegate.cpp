#include "AppDelegate.h"
#include "MainMenuScene.h" 

#define USE_AUDIO_ENGINE 1

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#elif USE_SIMPLE_AUDIO_ENGINE
#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;
#endif

USING_NS_CC;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
#if USE_AUDIO_ENGINE
    AudioEngine::end();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::end();
#endif
}

void AppDelegate::initGLContextAttrs()
{
    GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8, 0 };
    GLView::setGLContextAttrs(glContextAttrs);
}

static int register_all_packages()
{
    return 0;
}

bool AppDelegate::applicationDidFinishLaunching() {
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if (!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("GameBar", cocos2d::Rect(0, 0, 1702, 924));
#else
        glview = GLViewImpl::create("GameBar");
#endif
        director->setOpenGLView(glview);
    }

    director->setDisplayStats(false);
    director->setAnimationInterval(1.0f / 60);

    glview->setDesignResolutionSize(1702, 924, ResolutionPolicy::SHOW_ALL);

    register_all_packages();

    auto scene = MainMenuScene::createScene(); 
    director->runWithScene(scene);

#if USE_AUDIO_ENGINE
    AudioEngine::play2d("tavern_music.mp3", true, 0.3f); //ÌÓÇÈÊÀ
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->playBackgroundMusic("tavern_music.mp3", true);
    SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.4f);
#endif

    return true;
}

void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::pauseAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    SimpleAudioEngine::getInstance()->pauseAllEffects();
#endif
}

void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::resumeAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    SimpleAudioEngine::getInstance()->resumeAllEffects();
#endif
}