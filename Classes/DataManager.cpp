#include "DataManager.h"

USING_NS_CC;
using namespace std;

DataManager* DataManager::_instance = nullptr;

DataManager* DataManager::getInstance()
{
	if (_instance == nullptr)
	{
		_instance = new DataManager();
	}
	return _instance;
}

DataManager::DataManager()
{
	score = 0;
	lives = 3;
}

void DataManager::resetGameData()
{
	score = 0;
	lives = 3;
}

string DataManager::getPlayerName()
{
	// Зчитуємо збережене ім'я з налаштувань
	return UserDefault::getInstance()->getStringForKey("PlayerName", "Гість");
}

int DataManager::getGameSpeed()
{
	// Зчитуємо збережену швидкість
	return UserDefault::getInstance()->getIntegerForKey("GameSpeed", 1);
}

void DataManager::saveHighScore()
{
	int currentHighScore = getHighScore();
	if (score > currentHighScore)
	{
		UserDefault::getInstance()->setIntegerForKey("HighScore", score);
		UserDefault::getInstance()->setStringForKey("HighScorer", getPlayerName());
		UserDefault::getInstance()->flush();
	}
}

int DataManager::getHighScore()
{
	return UserDefault::getInstance()->getIntegerForKey("HighScore", 0);
}