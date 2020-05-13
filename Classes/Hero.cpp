#include "Hero.h"
#include "cocos2d.h"
#include "DataSet.h"
using namespace cocos2d;

void Hero::setMoveSpeed(float vx, float vy) { this->scheduleUpdate(); }

bool Hero::init() {
  if (!Mob::init()) return false;

  scheduleUpdate();

  const auto& config = DataSet::getInstance()->getConfig();
  assert(config["heroes"].IsObject());
  assert(config["heroes"].HasMember("sample-man"));
  log("%s", getHeroName());
  assert(config["heroes"].HasMember(getHeroName()));
  // 加载站立和行走动画
  const auto& data =
      DataSet::getInstance()->getConfig()["heroes"][getHeroName()];
  _walkAnimation = DataSet::load_animation(data["walk"]);
  _walkAnimation->setLoops(-1);
  _standAnimation = DataSet::load_animation(data["stand"]);
  _standAnimation->setLoops(-1);

  
  this->runAction(Animate::create(_standAnimation));

  return true;
}

void Hero::update(float delta) {
  this->setPosition(this->getPosition() + Vec2(_vx, _vy) * delta);
}