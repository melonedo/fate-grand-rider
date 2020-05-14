#include "Hero.h"
#include "cocos2d.h"
#include "DataSet.h"
#include <unordered_map>
#include <unordered_set>
using namespace cocos2d;


bool Hero::init() {
  if (!Mob::init()) return false;
  // 衍生类如果不需要这些直接重写即可
  scheduleUpdate();
  loadAnimation();
  registerUserInput();
  return true;
}

void Hero::loadAnimation() {
  const auto& config = DataSet::getConfig();
  assert(config["heroes"].IsObject());
  assert(config["heroes"].HasMember("sample-man"));
  log("%s", getHeroName());
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
    } else if (code == EventKeyboard::KeyCode::KEY_D) {
      this->setFlippedX(false);
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
  this->setPosition(this->getPosition() + _speed * delta);
  // 滚动屏幕（Size和Vec没有减法只有加法，所以倒过来）
  this->getScene()->setPosition(-this->getPosition() *
                                    DataSet::getGlobaZoomScale() +
                                designResolutionSize);
}