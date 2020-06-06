#include "Weapon.h"
#include "cocos2d.h"
#include "DataSet.h"
#include "GameScene.h"
#include "Interaction.h"
using namespace cocos2d;

Bow* Bow::create(const std::string& name) {
  Bow* bow = new Bow;
  bow->Weapon::init();
  bow->setName(name);
  const auto& data = DataSet::getConfig()["weapons"][name.c_str()];
  const auto& bow_data = data["bow"];
  bow->_bowAngleOffset = bow_data["angle-offset"].GetFloat();
  bow->setSpriteFrame(
      DataSet::load_frame(bow_data["frame"].GetString(), kWeaponResolution));

  const auto& arrow_data = data["arrow"];
  bow->_arrow = Sprite::create();
  bow->_arrow->setSpriteFrame(DataSet::load_frame(arrow_data["frame"].GetString()));
  bow->_arrow->setRotation(arrow_data["angle-offset"].GetFloat());
  bow->_arrowSpeed = arrow_data["speed"].GetFloat();

  const auto& anchor_data = bow_data["anchor"].GetArray();
  bow->setAnchorPoint(
      Vec2(anchor_data[0].GetFloat(), anchor_data[1].GetFloat()));
  const auto& anchor_data2 = arrow_data["anchor"];
  bow->_arrow->setAnchorPoint(
      Vec2(anchor_data2[0].GetFloat(), anchor_data2[1].GetFloat()));

  return bow;
}

void Bow::pointTo(Vec2 offset) {
  float angle_offset = isFlippedX() ? 180 - _bowAngleOffset : _bowAngleOffset;
  float angle = -offset.getAngle() * 180 / M_PI + angle_offset;

  this->setRotation(angle);
}

void Bow::fire(Vec2 offset) {
  auto new_arrow = Sprite::create();
  new_arrow->setRotation(-offset.getAngle() * 180 / M_PI + _arrow->getRotation());
  new_arrow->setSpriteFrame(_arrow->getSpriteFrame());
  new_arrow->setAnchorPoint(_arrow->getAnchorPoint());
  new_arrow->setPosition(_owner->getPosition());
  new_arrow->setVisible(true);
  getScene()->addChild(new_arrow);
  Vec2 speed = _arrowSpeed * offset / offset.getLength();
  Vec2 delta = speed / _arrowSpeed;
  new_arrow->runAction(RepeatForever::create(MoveBy::create(1, speed)));
  // 碰撞检测
  auto space = GameScene::getRunningScene()->getPhysicsSpace();
  auto filter = _owner->getBody().getFilter();
  auto collision_detect = [space, new_arrow, delta, filter](float) {
    if (auto target = space->querySegmentFirst(new_arrow->getPosition(),
                                   new_arrow->getPosition() + delta, filter)) {
      new_arrow->stopAllActions();
      new_arrow->unscheduleAllCallbacks();
      getInteraction(target)->attack(new_arrow, 1);
    }
  };
  new_arrow->schedule(collision_detect, 0, "collision_detect");
}