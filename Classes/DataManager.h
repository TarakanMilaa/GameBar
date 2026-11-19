#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <string>
#include "cocos2d.h"

class DataManager
{
public:
	static DataManager* getInstance();

	int score;
	int lives;

	// Отримання даних, які збережені в SetupScene
	std::string getPlayerName();
	int getGameSpeed();

	// Робота з рахунком та збереженням рекордів
	void resetGameData();
	void saveHighScore();
	int getHighScore();

private:
	DataManager();
	static DataManager* _instance;
};

#endif#pragma once
