#include "DataSet.h"
#include "cocos2d.h"
#include "json/document.h"
#include "constants.h"
using namespace cocos2d;

DataSet* DataSet::getInstance() {
  static DataSet* share_dataset = nullptr;
  if (share_dataset == nullptr) {
    // 不搞什么错误检测了，有什么直接用c++默认的exception
    share_dataset = new DataSet;
    share_dataset->init();
  }
  return share_dataset;
}

void DataSet::init() {
  auto manager = FileUtils::getInstance();
  auto data_str = manager->getStringFromFile("game-config.json");
  _config.Parse(data_str.c_str());
  CCASSERT(_config.IsObject(),
           "The format is broken or contain non-ascii characters?");
  _globalZoomScale = _config["global-zoom-scale"].GetFloat();
  _showPhysicsDebugBoxes = _config["show-physics-debug-boxes"].GetBool();
}

static void addCollisionBoxForTile(Sprite*);

TMXTiledMap* DataSet::load_map(const std::string& map_dir) {
  auto map = TMXTiledMap::create(map_dir);

  // 断言
  CCASSERT(map, "Failed to load tiled map");
  CCASSERT(map->getLayer("fg") && map->getLayer("bg") && map->getLayer("meta"),
           "The map does not have layers fg, bg, obj and meta");

  map->getLayer("bg")->setLocalZOrder(kMapPriorityBackground);
  map->getLayer("fg")->setLocalZOrder(kMapPriorityForeground);
  auto meta_layer = map->getLayer("meta");
  meta_layer->setVisible(false);
  //auto obj_layer = map->getObjectGroup("obj");

  chipmunk::initPhysicsForMap(map);

  return map;
}



// 各人物的名字和对应的构造函数
const static std::unordered_map<std::string, std::function<Hero*()>> kHeroSet{
    {"sample-man", SampleHero::create}};

Hero* DataSet::load_hero(const std::string& hero_name) {
  const auto& hero_data =
      DataSet::getConfig()["heroes"][hero_name.c_str()].GetObject();
  Hero* hero = kHeroSet.at(hero_name)();
  return hero;
}

SpriteFrame* DataSet::load_frame(const std::string& frame_dir) {
  auto frame = SpriteFrame::create(
      frame_dir, Rect(0, 0, kSpriteResolution, kSpriteResolution));
  CCASSERT(frame, "Unable to load frame");
  return frame;
}

Animation* DataSet::load_animation(const rapidjson::Value& animation_obj) {
  auto animation = Animation::create();
  animation->setDelayPerUnit(animation_obj["interval"].GetFloat());

  for (const auto& frame_dir : animation_obj["frames"].GetArray()) {
    animation->addSpriteFrame(load_frame(frame_dir.GetString()));
  }
  return animation;
}