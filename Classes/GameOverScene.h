#ifndef GAMEOVERSCENE_H
#define GAMEOVERSCENE_H

#include "cocos2d.h"

class GameOverScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();

	void menuRetryCallback(cocos2d::Ref* pSender);
	void menuMainCallback(cocos2d::Ref* pSender);

	CREATE_FUNC(GameOverScene);
};

#endif#pragma once
