#include "GameScene.h"
#include "cocos2d.h"
#include "DataSet.h"
#include "constants.h"
#include "UI.h"
#include "Physics.h"
#include "MonsterManager.h"
using namespace cocos2d;

// 测试用的ui
void addSampleUI(StaticNode*);


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
  addSampleUI(static_node);

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
    auto a = debug_set["weapon"].GetString();
    hero->pickWeapon(DataSet::load_weapon(debug_set["weapon"].GetString()));

    //加载UI
    /*auto node = StaticNode::create();
    this->addChild(node,0,"static");
    UISprite::addUI(node);*/


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

void addSampleUI(StaticNode* node) {
  // 在（10,10）的位置放个标签（左下对齐）
  auto label =
      Label::createWithSystemFont("10,10", "Microsoft YaHei", 20, Size::ZERO,
                                  TextHAlignment::LEFT, TextVAlignment::BOTTOM);
  label->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
  label->setPosition(10, 10);
  label->setGlobalZOrder(kMapPriorityUI);
  node->addChild(label);

  // 在顶部放个标签
  label =
      Label::createWithSystemFont("top", "Microsoft YaHei", 20, Size::ZERO,
                                  TextHAlignment::LEFT, TextVAlignment::TOP);
  label->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
  label->setPosition(0, node->getVisibleSize().height);
  label->setGlobalZOrder(kMapPriorityUI);
  node->addChild(label);

  const auto& data = DataSet::getConfig()["UI"]["bars"];

  auto bgBars = cocos2d::Sprite::create(data["bg-bars"].GetString());
  bgBars->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
  bgBars->setPosition(0, node->getVisibleSize().height);
  bgBars->setGlobalZOrder(kUserInterfaceBackground);
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
  healthbar->setPosition(40, node->getVisibleSize().height - 1);
  healthbar->setBackgroundTexture(data["bar"].GetString());
  healthbar->setForegroundTexture(data["health-progress"].GetString());
  healthbar->setTotalProgress(120.0f);
  healthbar->setCurrentProgress(22.0f);
  node->addChild(healthbar,kBars);

  auto shieldbar = UIBar::create();
  shieldbar->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
  shieldbar->setPosition(40, node->getVisibleSize().height - 11);
  shieldbar->setBackgroundTexture(data["bar"].GetString());
  shieldbar->setForegroundTexture(data["shield-progress"].GetString());
  shieldbar->setTotalProgress(120.0f);
  shieldbar->setCurrentProgress(22.0f);
  node->addChild(shieldbar, kBars);

  auto magicbar = UIBar::create();
  magicbar->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
  magicbar->setPosition(40, node->getVisibleSize().height - 21);
  magicbar->setBackgroundTexture(data["bar"].GetString());
  magicbar->setForegroundTexture(data["magic-progress"].GetString());
  magicbar->setTotalProgress(120.0f);
  magicbar->setCurrentProgress(22.0f);
  node->addChild(magicbar, kBars);
