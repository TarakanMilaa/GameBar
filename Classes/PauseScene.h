#ifndef PAUSESCENE_H
#define PAUSESCENE_H

#include "cocos2d.h"

class PauseScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();

	void menuResumeCallback(cocos2d::Ref* pSender);
	void menuToMenuCallback(cocos2d::Ref* pSender);

	CREATE_FUNC(PauseScene);
};

#endif