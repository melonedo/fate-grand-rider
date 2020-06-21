#pragma once
#include "cocos2d.h"
#include <vector>
#include <string>
#include "json/document.h"
#include "Map.h"

// 地图（关卡管理器）
class LevelManager {
 public:
  LevelManager() = default;
  LevelManager(const rapidjson::Value& levels);
  // 获取下一张地图。
  // 第一次会获取第一张，第二次会获取第二张，以此类推。
  // 若已经获取完所有地图，返回nullptr。
  cocos2d::TMXTiledMap* getMap(std::vector<Room>& rooms) const;
 private:
  std::vector<std::string> _levels;
  mutable size_t _currentLevel = 0;
};