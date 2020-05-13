#pragma once
#include "cocos2d.h"
#include "json/document.h"
#include "Hero.h"
using namespace cocos2d;
// 全局的数据集
class DataSet {
 public:
  // 用法同cocos2d-x里面其他的singleton类
  static DataSet* getInstance();

  // 返回数据集game-data.json
  // 请一定要用const&来接受，这个类是不能复制的。
  const rapidjson::Document& getConfig() const { return _config; }

  // 从map_dir加载一张地图，并不往上面放生物
  static cocos2d::TMXTiledMap* load_map(const std::string& map_dir);

  // 加载一个人物
  static Hero* load_hero(const std::string& hero_name);

  // 加载一个动画，读取对应的interval作为间隔，然后依次读取frame中的图片
  static Animation* load_animation(const rapidjson::Value& animation_obj);

  // 加载一张32x32图片
  static SpriteFrame* load_frame(const std::string& frame_dir);

  // 获取缩放倍数，由于实在是太常用了这里直接预加载
  float getGlobaZoomScale() const { return _global_zoom_scale; }

 private:
  DataSet() = default;
  void init();

  rapidjson::Document _config;
  float _global_zoom_scale;
};