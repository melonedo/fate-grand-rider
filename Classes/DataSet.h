#pragma once
#include "cocos2d.h"
#include "json/document.h"
// 全局的数据集
class DataSet {
 public:
  // 用法同cocos2d-x里面其他的singleton类
  static DataSet* getInstance();

  // 返回数据集game-data.json
  // 请一定要用const&来接受，这个类是不能复制的。
  static const rapidjson::Document& getConfig() { return getInstance()->_config; }

  // 从map_dir加载一张地图，并不往上面放生物
  static cocos2d::TMXTiledMap* load_map(std::string map_dir);

 private:
  DataSet() = default;
  void init();

  rapidjson::Document _config;
};