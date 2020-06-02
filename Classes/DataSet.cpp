#include "DataSet.h"
#include "cocos2d.h"
#include "json/document.h"
#include "constants.h"
#include "Hero.h"
#include "Interaction.h"
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

const static std::unordered_map<std::string, int> kTagSet{
    {"wall", kTagWall}, {"interactable", kTagInteractable}};

const static std::unordered_map<std::string, std::function<Interaction*()>>
    kInteractionSet{{"hide", HideSpot::create}};

TMXTiledMap* DataSet::load_map(const std::string& map_dir) {
  auto map = TMXTiledMap::create(map_dir);

  // 断言
  CCASSERT(map, "Failed to load tiled map");
  CCASSERT(map->getLayer("fg") && map->getLayer("bg") && map->getLayer("meta"),
           "The map does not have layers fg, bg, obj and meta");

  map->getLayer("bg")->setGlobalZOrder(kMapPriorityBackground);
  map->getLayer("fg")->setGlobalZOrder(kMapPriorityForeground);
  auto meta_layer = map->getLayer("meta");
  meta_layer->setVisible(false);
  
  // 加入标签
  auto layer_size = meta_layer->getLayerSize();
  auto map_size = map->getMapSize();
  CCASSERT(map_size.width == layer_size.width &&
               map_size.height == layer_size.height,
           "Size of map and meta layer must be the same.");
  for (int x = 0; x < map_size.width; x++) {
    for (int y = 0; y < map_size.height; y++) {
      Vec2 pos(x, y);
      auto prop = map->getPropertiesForGID(meta_layer->getTileGIDAt(pos));
      if (!prop.isNull()) {
        auto value_map = prop.asValueMap();
        auto tile = meta_layer->getTileAt(pos);
        chipmunk::initPhysicsForTile(tile);
        auto type = value_map.at("type").asString();
        tile->setTag(kTagSet.at(type));
        if (type == "interactable") {
          auto intraction =
              kInteractionSet.at(value_map.at("interaction").asString())();
          intraction->setName("interaction");
          tile->addComponent(intraction);
        }
      }
    }
  }

  return map;
}

// 各人物的名字和对应的构造函数
const static std::unordered_map<std::string, std::function<Hero*()>> kHeroSet{
    {"sample-man", SampleHero::create}};

Hero* DataSet::load_hero(const std::string& hero_name) {
  const auto& hero_data =
      DataSet::getConfig()["heroes"][hero_name.c_str()].GetObject();
  Hero* hero = kHeroSet.at(hero_name)();
  hero->setTag(kTagHero);
  return hero;
}

SpriteFrame* DataSet::load_frame(const std::string& frame_dir, int size) {
  auto frame = SpriteFrame::create(frame_dir, Rect(0, 0, size, size));
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

// 各种类的武器和对应的构造函数
/*const static std::unordered_map<std::string,
                                std::function<Weapon*(const std::string&)>>
    kWeaponSet{{"bow", Bow::create}};*/

/*const static std::unordered_map<std::string,
                                std::function<Weapon*(const std::string&)>>
    kWeaponSet{{"magicball", Magic::create}};*/

const static std::unordered_map<std::string,
                                std::function<Weapon*(const std::string&)>>
    kWeaponSet{{"darts", Darts::create}};

/*const static std::unordered_map<std::string,
                                std::function<Weapon*(const std::string&)>>
    kWeaponSet{{"spear", Spear::create}};*/

Weapon* DataSet::load_weapon(const std::string& weapon_name) {
  const auto& weapon_data = getConfig()["weapon"][weapon_name.c_str()];
  return kWeaponSet.at(weapon_data["type"].GetString())(weapon_name);
}
