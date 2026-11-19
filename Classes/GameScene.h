#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "cocos2d.h"

class GameScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();

	// Ігровий цикл для оновлення сцени[cite: 70]
	virtual void update(float dt) override;

	// Метод для спавну коктейлів[cite: 71]
	void spawnCocktail(float dt);

	// Зворотній виклик для ідентифікації колізій[cite: 7]
	bool onCollision(cocos2d::PhysicsContact& contact);

	CREATE_FUNC(GameScene);

private:
	cocos2d::Label* _scoreLabel;
	cocos2d::Label* _livesLabel;
};

#endif