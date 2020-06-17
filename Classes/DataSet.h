#pragma once
#include "cocos2d.h"
#include "json/document.h"
#include "Hero.h"
#include "Weapon.h"
#include "Map.h"
#include "Monster.h"
using namespace cocos2d;
// 全局的数据集
class DataSet {
 public:
  // 用法同cocos2d-x里面其他的singleton类
  static DataSet* getInstance();

  // 返回数据集game-data.json
  // 请一定要用const&来接受，这个类是不能复制的。
  static const rapidjson::Document& getConfig() {
    return getInstance()->_config;
  }

  // 所有load系列的函数的功能主要都是加载对应的资源，或者负责把字符串映射到具体的对象上

  // 从map_dir加载一张地图，并不往上面放生物
  static cocos2d::TMXTiledMap* loadMap(const std::string& map_dir, std::vector<Room>& rooms);

  // 加载一个人物
  static Hero* loadHero(const std::string& hero_name);

  //加载一个怪物
  static Monster* loadMonster(const std::string& monster_name);

  // 加载一个动画，读取对应的interval作为间隔，然后依次读取frame中的图片
  static Animation* loadAnimation(const rapidjson::Value& animation_obj);

  // 加载一张图片
  static SpriteFrame* loadFrame(const std::string& frame_dir,
                                 int size = kSpriteResolution);

  // 加载一个武器
  static Weapon* loadWeapon(const std::string& weapon_name);

  // 加载一个互动
  static Interaction* loadInteraction(const std::string& interaction_name,
                                       const cocos2d::Vec2& position,
                                       const cocos2d::ValueMap& property,
                                       chipmunk::Body&& body);

  // 一些常用玩意就预加载了
  static float getGlobaZoomScale() { return getInstance()->_globalZoomScale; }
  static bool getShowPhysicsDebugBoxes() {
    return getInstance()->_showPhysicsDebugBoxes;
  }

 private:
  DataSet() = default;
  void init();

  rapidjson::Document _config;
  float _globalZoomScale;
  bool _showPhysicsDebugBoxes;
};
