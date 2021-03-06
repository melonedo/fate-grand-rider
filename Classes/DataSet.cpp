#include "DataSet.h"
#include "Hero.h"
#include "Interaction.h"
#include "Item.h"
#include "Map.h"
#include "Pause.h"
#include "cocos2d.h"
#include "constants.h"
#include "json/document.h"
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

const static std::unordered_map<
    std::string,
    std::function<Interaction*(const Vec2&, const ValueMap&, chipmunk::Body&&)>>
    kInteractionSet{
        {"wall", NoInteraction::load}, {"hide", HideSpot::load},
        {"chest", Chest::load},        {"gate", Gate::load},
        {"target", Target::load},      {"item-chest", ItemChest::load},
        {"teleport", Teleport::load}};

Interaction* DataSet::loadInteraction(const std::string& interaction_name,
                                      const Vec2& position,
                                      const ValueMap& property,
                                      chipmunk::Body&& body) {
  auto create_func = kInteractionSet.at(interaction_name);
  return create_func(position, property, std::move(body));
}

TMXTiledMap* DataSet::loadMap(const std::string& map_dir,
                              std::vector<Room>& rooms) {
  auto map = TMXTiledMap::create(map_dir);
  rooms = std::move(processMap(map));
  return map;
}

// 各人物的名字和对应的构造函数
const static std::unordered_map<std::string, std::function<Hero*()>> kHeroSet{
    {"sample-man", SampleHero::create}};

Hero* DataSet::loadHero(const std::string& hero_name) {
  Hero* hero = kHeroSet.at(hero_name)();
  hero->setTag(kTagHero);
  return hero;
}

//各怪物的名字和对应的构造函数
const static std::unordered_map<std::string, std::function<Monster*()>>
    kMonsterSet{{"sample-monster", SampleMonster::create},
                {"knife-monster", KnifeMonster::create}};

Monster* DataSet::loadMonster(const std::string& monster_name) {
  Monster* monster = kMonsterSet.at(monster_name)();
  // monster->setTag(kTagMonster);
  return monster;
}

SpriteFrame* DataSet::loadFrame(const std::string& frame_dir, int size) {
  auto frame = SpriteFrame::create(frame_dir, Rect(0, 0, size, size));
  CCASSERT(frame, "Unable to load frame");
  return frame;
}

Animation* DataSet::loadAnimation(const rapidjson::Value& animation_obj) {
  auto animation = Animation::create();
  animation->setDelayPerUnit(animation_obj["interval"].GetFloat());

  for (const auto& frame_dir : animation_obj["frames"].GetArray()) {
    animation->addSpriteFrame(loadFrame(frame_dir.GetString()));
  }
  return animation;
}

// 各种类的武器和对应的构造函数
const static std::unordered_map<std::string,
                                std::function<Weapon*(const std::string&)>>
    kWeaponSet{{"magicball", Magic::createweapon},
               {"bow", Bow::createweapon},
               {"darts", Darts::createweapon},
               {"spear", Spear::createweapon},
               {"blinkbow", BlinkBow::createweapon},
               {"redball", RedBall::create},
               {"knife", Knife::create}};

Weapon* DataSet::loadWeapon(const std::string& weapon_name) {
  const auto& weapon_data = getConfig()["weapon"][weapon_name.c_str()];
  auto res = kWeaponSet.at(weapon_data["type"].GetString())(weapon_name);
  res->setName(weapon_name);
  return res;
}


//各种类的道具及其对应的构造函数
const static std::unordered_map<std::string,
                                std::function<Item*(const std::string&)>>
    kItemSet{{"health-bottle", HealthBottle::create},
             {"magic-bottle", MagicBottle::create}};

Item* DataSet::loadItem(const std::string& item_name) {
  const auto& item_data = getConfig()["item"][item_name.c_str()];
  auto res = kItemSet.at(item_data["type"].GetString())(item_name);
  res->setName(item_name);
  return res;
}