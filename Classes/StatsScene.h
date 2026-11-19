#ifndef STATSSCENE_H
#define STATSSCENE_H

#include "cocos2d.h"

class StatsScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();

	void menuBackCallback(cocos2d::Ref* pSender);

	CREATE_FUNC(StatsScene);
};

#endif
