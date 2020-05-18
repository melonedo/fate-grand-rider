#include "Hero.h"
#include "cocos2d.h"
#include "DataSet.h"
#include <unordered_map>
#include <unordered_set>
#include "GameScene.h"
#include "Interaction.h"
using namespace cocos2d;


bool Hero::init() {
  if (!Mob::init()) return false;
  // 衍生类如果不需要这些直接重写即可
  scheduleUpdate();
  loadAnimation();
  registerUserInput();
  // 手的位置
  const auto& hand_data =
      DataSet::getConfig()["heroes"][getHeroName()]["hand-pos"];
  _handPos = Vec2(hand_data[0].GetFloat(), hand_data[1].GetFloat());
  return true;
}

void Hero::loadAnimation() {
  const auto& config = DataSet::getConfig();
  assert(config["heroes"].IsObject());
  assert(config["heroes"].HasMember(getHeroName()));
  // 加载站立和行走动画
  const auto& data = DataSet::getConfig()["heroes"][getHeroName()];
  _walkAnimation = DataSet::load_animation(data["walk"]);
  _walkAnimation->setLoops(-1);
  _standAnimation = DataSet::load_animation(data["stand"]);
  _standAnimation->setLoops(-1);

  // 不播放的话英雄看不见
  auto animate = Animate::create(_standAnimation);
  animate->setTag(kTagStandAnimation);
  this->runAction(animate);
}

void Hero::setMoveSpeed(float vx, float vy) { _speed = Vec2(vx, vy); }

void Hero::registerUserInput() {
  _speedScale = DataSet::getConfig()["hero-speed-scale"].GetFloat();

  auto keyboard_listener = EventListenerKeyboard::create();
  using namespace std::placeholders;
  keyboard_listener->onKeyPressed = std::bind(&Hero::onKeyPressed, this, _1, _2);
  keyboard_listener->onKeyReleased = std::bind(&Hero::onKeyReleased, this, _1, _2);
  this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(
      keyboard_listener, this);

  auto mouse_listener = EventListenerMouse::create();
  mouse_listener->onMouseMove = [this](EventMouse* event) {
    this->_weapon->pointTo(Vec2(event->getCursorX(), event->getCursorY()) -
                           designResolutionSize / 2);
  };
  mouse_listener->onMouseDown = [this](EventMouse* event) {
    this->_weapon->fire(Vec2(event->getCursorX(), event->getCursorY()) -
                        designResolutionSize / 2);
  };
  this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(
      mouse_listener, this);
}

const std::unordered_map<EventKeyboard::KeyCode, Vec2>
    Hero::kWasdDirections{{EventKeyboard::KeyCode::KEY_W, {0, 1}},
                          {EventKeyboard::KeyCode::KEY_A, {-1, 0}},
                          {EventKeyboard::KeyCode::KEY_S, {0, -1}},
                          {EventKeyboard::KeyCode::KEY_D, {1, 0}}};

void Hero::onKeyPressed(EventKeyboard::KeyCode code, Event*) {
  if (kWasdDirections.count(code) != 0) {
    _wasdPressed.insert(code);
    if (_wasdPressed.size() == 1) {
      this->stopActionByTag(kTagStandAnimation);
      auto animate = Animate::create(_walkAnimation);
      animate->setTag(kTagWalkingAnimation);
      this->runAction(animate);
    }
    if (code == EventKeyboard::KeyCode::KEY_A) {
      this->setFlippedX(true);
      _weapon->setFlippedX(true);
      _weapon->setPositionNormalized(Vec2(1 - _handPos.x, _handPos.y));
    } else if (code == EventKeyboard::KeyCode::KEY_D) {
      this->setFlippedX(false);
      _weapon->setFlippedX(false);
      _weapon->setPositionNormalized(_handPos);
    }
    updateSpeed();
  }
}

void Hero::onKeyReleased(EventKeyboard::KeyCode code, Event*) {
  if (kWasdDirections.count(code) != 0) {
    _wasdPressed.erase(code);
    updateSpeed();
    if (_wasdPressed.size() == 0) {
      this->stopActionByTag(kTagWalkingAnimation);
      auto animate = Animate::create(_standAnimation);
      animate->setTag(kTagStandAnimation);
      this->runAction(animate);
    }
  } else if (code == EventKeyboard::KeyCode::KEY_SPACE &&
             _interacting != nullptr) {
    _interacting->dialog();
  }
}

void Hero::updateSpeed() {
  Vec2 speed;
  for (auto key : _wasdPressed) {
    speed += kWasdDirections.at(key);
  }
  this->_speed = speed * _speedScale;
}


void Hero::update(float delta) {
  auto old_pos = this->getPosition();
  auto new_pos = old_pos + _speed * delta;


  // 碰撞检测
  chipmunk::Space::PointQueryInfo info;
  auto space = GameScene::getRunningScene()->getPhysicsSpace();
  auto result = space->queryPointNearest(new_pos, kSpriteResolution / 4,
                                         _body.getFilter(), &info);
  if (result != nullptr) {
    // 贴墙
    new_pos = info.point + info.grad * (kSpriteResolution / 4);

    // 尝试互动
    switch (result->getTag()) {
      case kTagInteractable: {
        auto interaction =
            dynamic_cast<Interaction*>(result->getComponent("interaction"));
        if (interaction != _interacting) {
          interaction->touch();
          _interacting = interaction;
        }
        break;
      }
      default:
        break;
    }
  } else if (_speed.length() != 0) {
    // 取消互动
    _interacting = nullptr;
  }

  // 滚动屏幕（Size和Vec没有减法只有加法，所以倒过来）
  auto scene = this->getScene();
  float scale = DataSet::getGlobaZoomScale();
  scene->setPosition(-new_pos * scale + designResolutionSize / 2 * scale);
  this->setPosition(new_pos);
}

void Hero::pickWeapon(Weapon* weapon) {
  _weapon = weapon;
  weapon->setVisible(true);
  weapon->setPositionNormalized(_handPos);
  //weapon->setPosition(Vec2(100, 100));
  this->addChild(weapon, 1);
  weapon->_owner = this;
}