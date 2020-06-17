#include "GameScene.h"
#include "cocos2d.h"
#include "DataSet.h"
#include "constants.h"
#include "UI.h"
#include "Physics.h"
using namespace cocos2d;

// 添加ui（以静态节点）
void addUI(StaticNode*);


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

  // 物理空间
  _space = std::make_shared<chipmunk::Space>();



  // 首先判断是不是用测试集

  if (config["use-debug-mode"].GetBool()) {
    const auto& debug_set = config["debug-set"].GetObject();

    // 加载地图
    auto map_dir = debug_set["map"].GetString();
    auto map = DataSet::load_map(map_dir, _rooms);

    if (config["show-physics-debug-boxes"].GetBool()) {
      this->getPhysicsWorld()->setDebugDrawMask(~0);
    }

    this->addChild(map);

    // 加载角色
    auto hero = DataSet::load_hero(debug_set["hero"].GetString());
    auto spawn = map->getObjectGroup("obj")->getObject("spawn");
    hero->setPosition(spawn["x"].asFloat(), spawn["y"].asFloat());
    hero->setName("hero");
    this->addChild(hero, kMapPrioritySprite, kTagHero);
    hero->registerUserInput();

    // 配上武器
    hero->pickWeapon(DataSet::load_weapon(debug_set["weapon"].GetString()));

    // 静态节点
    auto static_node = StaticNode::create();
    this->addChild(static_node, 0, "static");
    addUI(static_node);

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

bool StaticNode::init() {
  if (!Node::init()) return false;
  _visibleSize = designResolutionSize / DataSet::getGlobaZoomScale();
  return true;
}

const Size& StaticNode::getVisibleSize() const { return _visibleSize; }


void addUI(StaticNode* node) {
  const auto& data = DataSet::getConfig()["UI"]["bars"];

  auto bgBars = cocos2d::Sprite::create(data["bg-bars"].GetString());
  bgBars->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
  bgBars->setPosition(0, node->getVisibleSize().height);
  bgBars->setGlobalZOrder(kMapPriorityUI);
  node->addChild(bgBars);

  auto health = cocos2d::Sprite::create(data["health"].GetString());
  health->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
  health->setPosition(3, node->getVisibleSize().height - 1);
  health->setGlobalZOrder(kUserInterfaceBars);
  node->addChild(health);

  auto shield = cocos2d::Sprite::create(data["shield"].GetString());
  shield->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
  shield->setPosition(3, node->getVisibleSize().height - 11);
  shield->setGlobalZOrder(kUserInterfaceBars);
  node->addChild(shield);

  auto magic = cocos2d::Sprite::create(data["magic"].GetString());
  magic->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
  magic->setPosition(3, node->getVisibleSize().height - 21);
  magic->setGlobalZOrder(kUserInterfaceBars);
  node->addChild(magic);

  auto hero =
      static_cast<Hero*>(GameScene::getRunningScene()->getChildByTag(kTagHero));

  auto healthbar = UIBar::create();
  healthbar->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
  healthbar->setPosition(30, node->getVisibleSize().height - 5);
  healthbar->setBackgroundTexture(data["bar"].GetString());
  healthbar->setForegroundTexture(data["health-progress"].GetString());
  healthbar->setTotalProgress(hero->getTotalHp());
  healthbar->setCurrentProgress(hero->getHp());
  healthbar->setLocalZOrder(kUserInterfaceBars);
  node->addChild(healthbar, kUserInterfaceBars, kTagHealth);

  auto shieldbar = UIBar::create();
  shieldbar->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
  shieldbar->setPosition(30, node->getVisibleSize().height - 15);
  shieldbar->setBackgroundTexture(data["bar"].GetString());
  shieldbar->setForegroundTexture(data["shield-progress"].GetString());
  shieldbar->setTotalProgress(hero->getTotalSe());
  shieldbar->setCurrentProgress(hero->getSe());
  node->addChild(shieldbar, kUserInterfaceBars, kTagShield);

  auto magicbar = UIBar::create();
  magicbar->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
  magicbar->setPosition(30, node->getVisibleSize().height - 25);
  magicbar->setBackgroundTexture(data["bar"].GetString());
  magicbar->setForegroundTexture(data["magic-progress"].GetString());
  magicbar->setTotalProgress(hero->getTotalMp());
  magicbar->setCurrentProgress(hero->getMp());
  node->addChild(magicbar, kUserInterfaceBars, kTagMagic);

  auto weaponbg = Sprite::create(data["bg-weapon"].GetString());
  weaponbg->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
  weaponbg->setPosition(node->getVisibleSize().width-50, 50);
  weaponbg->setGlobalZOrder(kMapPriorityUI);
  node->addChild(weaponbg);
}