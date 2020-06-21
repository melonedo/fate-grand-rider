#include "GameScene.h"
#include "DataSet.h"
#include "MonsterManager.h"
#include "Physics.h"
#include "UI.h"
#include "cocos2d.h"
#include "constants.h"
#include "Endgame.h"
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

  //暂停
  auto pause = PauseGame::create();
  this->addChild(pause);

  const auto& config = DataSet::getConfig();

  // 缩放
  this->setScale(config["global-zoom-scale"].GetFloat());

  // 物理空间
  _space = std::make_shared<chipmunk::Space>();

  Hero* hero;
  // 判断是不是用测试集
  if (config["use-debug-mode"].GetBool()) {
    const auto& debug_set = config["debug-set"].GetObject();
    // 加载地图
    _levelManager = LevelManager(debug_set["maps"]);
    // 加载角色
    hero = DataSet::loadHero(debug_set["hero"].GetString());
    // 配上武器
    hero->pickWeapon(DataSet::loadWeapon(debug_set["weapon"].GetString()));
  } else {
    const auto& start_set = config["start-set"];
    _levelManager = LevelManager(start_set["maps"]);
    hero = DataSet::loadHero(start_set["hero"].GetString());
    hero->pickWeapon(DataSet::loadWeapon(start_set["weapon"].GetString()));
  }

  hero->setName("hero");
  this->addChild(hero, kMapPrioritySprite, kTagHero);
  hero->registerUserInput();

  if (config["show-physics-debug-boxes"].GetBool()) {
    this->getPhysicsWorld()->setDebugDrawMask(~0);
  }

  // 静态节点
  auto static_node = StaticNode::create();
  this->addChild(static_node, 0, "static");
  addUI(static_node);
  _node = static_node;

  scheduleUpdate();

  this->nextLevel();

  srand(std::chrono::system_clock::now().time_since_epoch().count());

  return true;
}

void GameScene::nextLevel() {
  auto map = _levelManager.getMap(_rooms);
  if (map == nullptr) {
    // 通关拉
    Director::getInstance()->replaceScene(EndScene::create(true));
    return;
  }

  this->removeChildByName("map");
  map->setName("map");
  this->addChild(map);

  auto hero = this->getChildByTag(kTagHero);
  auto spawn = map->getObjectGroup("obj")->getObject("spawn");
  hero->setPosition(spawn["x"].asFloat(), spawn["y"].asFloat());
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
  health->setPosition(7, node->getVisibleSize().height - 10);
  health->setGlobalZOrder(kUserInterfaceBars);
  node->addChild(health);

  auto shield = cocos2d::Sprite::create(data["shield"].GetString());
  shield->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
  shield->setPosition(7, node->getVisibleSize().height - 35);
  shield->setGlobalZOrder(kUserInterfaceBars);
  node->addChild(shield);

  auto magic = cocos2d::Sprite::create(data["magic"].GetString());
  magic->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
  magic->setPosition(7, node->getVisibleSize().height - 60);
  magic->setGlobalZOrder(kUserInterfaceBars);
  node->addChild(magic);

  auto hero =
      static_cast<Hero*>(GameScene::getRunningScene()->getChildByTag(kTagHero));

  auto healthbar = UIBar::create();
  healthbar->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
  healthbar->setPosition(125, node->getVisibleSize().height - 20);
  healthbar->setBackgroundTexture(data["bar"].GetString());
  healthbar->setForegroundTexture(data["health-progress"].GetString());
  healthbar->setTotalProgress(hero->getTotalHp());
  healthbar->setCurrentProgress(hero->getHp());
  setChildrenGlobalZOrder(healthbar, kUserInterfaceBars + 1);
  node->addChild(healthbar, kUserInterfaceBars);
  healthbar->setTag(kTagHealth);

  auto shieldbar = UIBar::create();
  shieldbar->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
  shieldbar->setPosition(125, node->getVisibleSize().height - 45);
  shieldbar->setBackgroundTexture(data["bar"].GetString());
  shieldbar->setForegroundTexture(data["shield-progress"].GetString());
  shieldbar->setTotalProgress(hero->getTotalSe());
  shieldbar->setCurrentProgress(hero->getSe());
  setChildrenGlobalZOrder(shieldbar, kUserInterfaceBars + 1);
  node->addChild(shieldbar, kUserInterfaceBars, kTagShield);

  auto magicbar = UIBar::create();
  magicbar->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
  magicbar->setPosition(125, node->getVisibleSize().height - 70);
  magicbar->setBackgroundTexture(data["bar"].GetString());
  magicbar->setForegroundTexture(data["magic-progress"].GetString());

  magicbar->setTotalProgress(hero->getTotalMp());
  magicbar->setCurrentProgress(hero->getMp());
  setChildrenGlobalZOrder(magicbar, kUserInterfaceBars + 1);
  node->addChild(magicbar, kUserInterfaceBars, kTagMagic);

  auto weaponbg = Sprite::create(data["bg-weapon"].GetString());
  weaponbg->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
  weaponbg->setPosition(node->getVisibleSize().width - 50, 50);
  weaponbg->setGlobalZOrder(kMapPriorityUI);
  node->addChild(weaponbg);
}

void setChildrenGlobalZOrder(Node* node, float zorder) {
  for (auto child : node->getChildren()) {
    child->setGlobalZOrder(zorder);
  }
}

void GameScene::update(float dt) {
  auto hero =
      static_cast<Hero*>(GameScene::getRunningScene()->getChildByTag(kTagHero));
  auto healthbar = static_cast<UIBar*>(
      GameScene::getRunningScene()->getStaticNode()->getChildByTag(kTagHealth));
  auto shieldbar = static_cast<UIBar*>(
      GameScene::getRunningScene()->getStaticNode()->getChildByTag(kTagShield));
  auto magicbar = static_cast<UIBar*>(
      GameScene::getRunningScene()->getStaticNode()->getChildByTag(kTagMagic));
  healthbar->setCurrentProgress(hero->getHp());
  shieldbar->setCurrentProgress(hero->getSe());
  magicbar->setCurrentProgress(hero->getMp());
}