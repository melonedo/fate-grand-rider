#include "GameScene.h"
#include "cocos2d.h"
#include "DataSet.h"
#include "constants.h"
#include "UI.h"
#include "Physics.h"
#include "MonsterManager.h"
using namespace cocos2d;

// ui
void addUI(StaticNode*);


bool GameScene::init() {

  bool result;
  if (DataSet::getShowPhysicsDebugBoxes()) {
    result = Scene::initWithPhysics();
  }
  else {
    result = Scene::init();
  }
  if (!result) return false;

  runningGameScene = this;


  //暂停
  auto m_pause = PauseGame::create();
  this->addChild(m_pause);
  const auto& config = DataSet::getConfig();

  // 缩放
  this->setScale(config["global-zoom-scale"].GetFloat());

  // 物理空间
  _space = std::make_shared<chipmunk::Space>();

  // 静态节点
  auto static_node = StaticNode::create();
  this->addChild(static_node, 0, "static");
  addUI(static_node);

  // 首先判断是不是用测试集

  if (config["use-debug-mode"].GetBool()) {
    const auto& debug_set = config["debug-set"].GetObject();

    // 加载地图
    auto map_dir = debug_set["map"].GetString();
    auto map = DataSet::loadMap(map_dir, _rooms);

    if (config["show-physics-debug-boxes"].GetBool()) {
      this->getPhysicsWorld()->setDebugDrawMask(~0);
    }

    this->addChild(map);

    // 加载角色
    auto hero = DataSet::loadHero(debug_set["hero"].GetString());
    auto spawn = map->getObjectGroup("obj")->getObject("spawn");
    hero->setPosition(spawn["x"].asFloat(), spawn["y"].asFloat());
    hero->setName("hero");
    this->addChild(hero, kMapPrioritySprite, kTagHero);
    hero->registerUserInput();

    // 配上武器
    auto a = debug_set["weapon"].GetString();
    hero->pickWeapon(DataSet::loadWeapon(debug_set["weapon"].GetString()));


    return true;
  }
  else {
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

// UIBar下属有两个Sprite，用这个方法设置一下
void setChildrenGlobalZOrder(Node*, float);

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
  health->setGlobalZOrder(kBars);
  node->addChild(health);

  auto shield = cocos2d::Sprite::create(data["shield"].GetString());
  shield->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
  shield->setPosition(3, node->getVisibleSize().height - 11);
  shield->setGlobalZOrder(kBars);
  node->addChild(shield);

  auto magic = cocos2d::Sprite::create(data["magic"].GetString());
  magic->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
  magic->setPosition(3, node->getVisibleSize().height - 21);
  magic->setGlobalZOrder(kBars);
  node->addChild(magic);

  auto healthbar = UIBar::create();
  healthbar->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
  healthbar->setPosition(30, node->getVisibleSize().height - 5);
  healthbar->setBackgroundTexture(data["bar"].GetString());
  healthbar->setForegroundTexture(data["health-progress"].GetString());
  healthbar->setTotalProgress(120.0f);
  healthbar->setCurrentProgress(22.0f);
  setChildrenGlobalZOrder(healthbar, kBars + 1);
  node->addChild(healthbar,kBars,kTagHealth);

  auto shieldbar = UIBar::create();
  shieldbar->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
  shieldbar->setPosition(30, node->getVisibleSize().height - 15);
  shieldbar->setBackgroundTexture(data["bar"].GetString());
  shieldbar->setForegroundTexture(data["shield-progress"].GetString());
  shieldbar->setTotalProgress(120.0f);
  shieldbar->setCurrentProgress(22.0f);
  setChildrenGlobalZOrder(shieldbar, kBars + 1);
  node->addChild(shieldbar, kBars, kTagShield);

  auto magicbar = UIBar::create();
  magicbar->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
  magicbar->setPosition(30, node->getVisibleSize().height - 25);
  magicbar->setBackgroundTexture(data["bar"].GetString());
  magicbar->setForegroundTexture(data["magic-progress"].GetString());
  magicbar->setTotalProgress(120.0f);
  magicbar->setCurrentProgress(22.0f);
  setChildrenGlobalZOrder(magicbar, kBars + 1);
  node->addChild(magicbar,kBars,kTagMagic);

  auto weaponbg = Sprite::create(data["bg-weapon"].GetString());
  weaponbg->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
  weaponbg->setPosition(node->getVisibleSize().width-50, 50);
  weaponbg->setGlobalZOrder(kUserInterfaceBackground);
  node->addChild(weaponbg);
}

void setChildrenGlobalZOrder(Node* node, float zorder) {
  for (auto child : node->getChildren()) {
    child->setGlobalZOrder(zorder);
  }
}
