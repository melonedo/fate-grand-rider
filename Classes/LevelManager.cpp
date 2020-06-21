#include "LevelManager.h"
#include "Endgame.h"
#include "DataSet.h"
using namespace cocos2d;

LevelManager::LevelManager(const rapidjson::Value& levels) {
  for (auto&& level : levels.GetArray()) {
    _levels.push_back(level.GetString());
  }
}

TMXTiledMap* LevelManager::getMap(std::vector<Room>& rooms) const {
  if (_currentLevel == _levels.size()) {
    return nullptr;
  } else {
    return DataSet::loadMap(_levels.at(_currentLevel++), rooms);
  }
}