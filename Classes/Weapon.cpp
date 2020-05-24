#include "Weapon.h"
#include "cocos2d.h"
#include "DataSet.h"
#include "GameScene.h"
#include "CCVector.h"
using namespace cocos2d;

/***武器——弓***/
Bow* Bow::create(const std::string& name) {
  Bow* bow = new Bow;
  bow->Weapon::init();
  bow->setName(name);
  const auto& data = DataSet::getConfig()["weapon"][name.c_str()];
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
  log("%f,%f %f", offset.x, offset.y, angle);
}

void Bow::fire(Vec2 offset) {
  auto new_arrow1 = Sprite::create();
  auto new_arrow2 = Sprite::create();
  auto new_arrow3 = Sprite::create();
  auto v1 = Vec2((offset.y*5 + offset.x*12) / 15, (offset.y*12 - offset.x*5) / 15);
  auto v2 = Vec2((offset.x * 12 - offset.y *5) / 15, (offset.y * 12 + offset.x * 5) / 15);
  new_arrow1->setRotation(-offset.getAngle() * 180 / M_PI + _arrow->getRotation() + 22);
  new_arrow1->setSpriteFrame(_arrow->getSpriteFrame());
  new_arrow1->setAnchorPoint(_arrow->getAnchorPoint());
  new_arrow1->setPosition((_owner->getPosition()));
  new_arrow2->setRotation(-offset.getAngle() * 180 / M_PI +
                          _arrow->getRotation());
  new_arrow2->setSpriteFrame(_arrow->getSpriteFrame());
  new_arrow2->setAnchorPoint(_arrow->getAnchorPoint());
  new_arrow2->setPosition((_owner->getPosition()));
  new_arrow3->setRotation(-offset.getAngle() * 180 / M_PI +
                          _arrow->getRotation()-22);
  new_arrow3->setSpriteFrame(_arrow->getSpriteFrame());
  new_arrow3->setAnchorPoint(_arrow->getAnchorPoint());
  new_arrow3->setPosition((_owner->getPosition()));
  new_arrow1->setVisible(true);
  new_arrow2->setVisible(true);
  new_arrow3->setVisible(true);
  getScene()->addChild(new_arrow1);
  getScene()->addChild(new_arrow2);
  getScene()->addChild(new_arrow3);
  Vec2 speed1 = _arrowSpeed * (v1) / offset.getLength();
  Vec2 speed2 = _arrowSpeed * (offset) / offset.getLength();
  Vec2 speed3 = _arrowSpeed * (v2) / offset.getLength();
  Vec2 delta1 = speed1 / _arrowSpeed;
  Vec2 delta2 = speed2 / _arrowSpeed;
  Vec2 delta3 = speed3 / _arrowSpeed;
  new_arrow1->runAction(RepeatForever::create(MoveBy::create(1, speed1)));
  new_arrow2->runAction(RepeatForever::create(MoveBy::create(1, speed2)));
  new_arrow3->runAction(RepeatForever::create(MoveBy::create(1, speed3)));
  // 碰撞检测
  auto space = GameScene::getRunningScene()->getPhysicsSpace()->getSpace();
  auto collision_detect = [space, new_arrow1, delta1](float) {
    if (cpSpaceSegmentQueryFirst(
            space, chipmunk::cpvFromVec2(new_arrow1->getPosition()),
            chipmunk::cpvFromVec2(new_arrow1->getPosition() + delta1), 1,
            CP_SHAPE_FILTER_ALL, nullptr)) {
      new_arrow1->stopAllActions();
      new_arrow1->unscheduleAllCallbacks();
    }
  };
  new_arrow1->schedule(collision_detect, 0, "collistion_detect");
  auto collision_detect2 = [space, new_arrow2, delta2](float) {
    if (cpSpaceSegmentQueryFirst(
            space, chipmunk::cpvFromVec2(new_arrow2->getPosition()),
            chipmunk::cpvFromVec2(new_arrow2->getPosition() + delta2), 1,
            CP_SHAPE_FILTER_ALL, nullptr)) {
      new_arrow2->stopAllActions();
      new_arrow2->unscheduleAllCallbacks();
    }
  };
  new_arrow2->schedule(collision_detect2, 0, "collistion_detect");
  auto collision_detect3 = [space, new_arrow3, delta3](float) {
    if (cpSpaceSegmentQueryFirst(
            space, chipmunk::cpvFromVec2(new_arrow3->getPosition()),
            chipmunk::cpvFromVec2(new_arrow3->getPosition() + delta3), 1,
            CP_SHAPE_FILTER_ALL, nullptr)) {
      new_arrow3->stopAllActions();
      new_arrow3->unscheduleAllCallbacks();
    }
  };
  new_arrow3->schedule(collision_detect3, 0, "collistion_detect");
}

/***武器——矛***/
Spear* Spear::create(const std::string& name) { 
  Spear* spear = new Spear;
  spear->Weapon::init();
  spear->setName(name);
  const auto& data = DataSet::getConfig()["weapon"][name.c_str()];
  const auto& spear_data = data["spear"];
  spear->_spearAngleOffset = spear_data["angle-offset"].GetFloat();
  spear->setSpriteFrame(
      DataSet::load_frame(spear_data["frame"].GetString(), kWeaponResolution));
  const auto& anchor_data = spear_data["anchor"].GetArray();
  spear->_spearSpeed = spear_data["speed"].GetFloat();
  spear->setAnchorPoint(
      Vec2(anchor_data[0].GetFloat(), anchor_data[1].GetFloat()));

  return spear;
}

void Spear::pointTo(Vec2 offset) {
  float angle_offset = isFlippedX() ? 180 - _spearAngleOffset : _spearAngleOffset;
  float angle = -offset.getAngle() * 180 / M_PI + angle_offset;
  this->setRotation(angle);
  log("%f,%f %f", offset.x, offset.y, angle);
}

void Spear::fire(Vec2 offset) {
  Vec2 speed = _spearSpeed * offset / offset.getLength();
  Vec2 delta = speed / _spearSpeed;
  auto flipxAction = FlipX::create(true);
  auto moveBy = MoveBy::create(0.3f, speed);
  auto action = Sequence::create(moveBy, flipxAction, moveBy->reverse(), NULL);
  runAction(action);
}
