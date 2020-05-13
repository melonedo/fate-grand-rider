#include "GameScene.h"
#include "cocos2d.h"
#include "DataSet.h"
#include "constants.h"
using namespace cocos2d;



bool GameScene::init() {
  if (!Scene::initWithPhysics()) return false;

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

  } else {
    CCASSERT(false, "Only debug set is supported now");
  }
}

