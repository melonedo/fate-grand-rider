#include "GameScene.h"
#include "cocos2d.h"
#include "DataSet.h"
#include "constants.h"
using namespace cocos2d;

#include "Physics.h"

bool GameScene::init() {
  bool result;
  if (DataSet::getShowPhysicsDebugBoxes()) {
    result = Scene::initWithPhysics();
  } else {
    result = Scene::init();
  }
  if (!result) return false;

  runningGameScene = this;

  const auto& config = DataSet::getConfig();

  // 缩放
  this->setScale(config["global-zoom-scale"].GetFloat());

  // 首先判断是不是用测试集

  if (config["use-debug-mode"].GetBool()) {
    const auto& debug_set = config["debug-set"].GetObject();

    // 加载地图
    auto map_dir = debug_set["map"].GetString();
    auto map = DataSet::load_map(map_dir);

    if (config["show-physics-debug-boxes"].GetBool()) {
      this->getPhysicsWorld()->setDebugDrawMask(~0);
    }

    this->addChild(map);

    // 加载角色
    auto hero = DataSet::load_hero(debug_set["hero"].GetString());
    auto spawn = map->getObjectGroup("obj")->getObject("spawn");
    hero->setPosition(spawn["x"].asFloat(), spawn["y"].asFloat());
    this->addChild(hero, kMapPrioritySprite);

    return true;
  } else {
    CCASSERT(false, "Only debug set is supported now");
    return false;
  }
}

GameScene* GameScene::runningGameScene = nullptr;

GameScene::~GameScene() {
  if (runningGameScene == this) {
    runningGameScene = nullptr;
  }
}