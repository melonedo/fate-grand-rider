#include "Weapon.h"
#include "cocos2d.h"
#include "DataSet.h"
#include "GameScene.h"
#include "CCVector.h"
using namespace cocos2d;
using namespace std;

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
  bow->_bowNumber = bow_data["number"].GetInt();
  const auto& arrow_data = data["arrow"];
  bow->_arrow = Sprite::create();
  bow->_arrow->setSpriteFrame(DataSet::load_frame(arrow_data["frame"].GetString()));
  bow->_arrow->setRotation(arrow_data["angle-offset"].GetFloat());
  bow->_arrowSpeed = arrow_data["speed"].GetFloat();

  if (bow->_bowNumber == 1) {
    const auto& arrow_data2 = data["arrow2"];
    bow->_arrow2 = Sprite::create();
    bow->_arrow2->setSpriteFrame(
        DataSet::load_frame(arrow_data2["frame"].GetString()));
    bow->_arrow2->setRotation(arrow_data2["angle-offset"].GetFloat());
  }

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
  auto v1 = Vec2((offset.y * 5 + offset.x * 12) / 15,
                 (offset.y * 12 - offset.x * 5) / 15);
  auto v2 = Vec2((offset.x * 12 - offset.y * 5) / 15,
                 (offset.y * 12 + offset.x * 5) / 15);
  Sprite* arrows[3];
  Sprite* arrows2;
  if (_bowNumber == 1) {
    arrows2 = Sprite::create();
    arrows2->setSpriteFrame(_arrow2->getSpriteFrame());
    arrows2->setRotation(-offset.getAngle() * 180 / M_PI +
                         _arrow2->getRotation());
    arrows2->setVisible(true);
    getScene()->addChild(arrows2);
  }

  for (int i = 0; i < _bowNumber; i++) {
    arrows[i] = Sprite::create();
    arrows[i]->setSpriteFrame(_arrow->getSpriteFrame());
    arrows[i]->setAnchorPoint(_arrow->getAnchorPoint());
    arrows[i]->setPosition((_owner->getPosition()));
    Vec2 speed;
    auto space = GameScene::getRunningScene()->getPhysicsSpace()->getSpace();
    if (i == 0) {
      arrows[i]->setRotation(-offset.getAngle() * 180 / M_PI +
                             _arrow->getRotation());
      speed = _arrowSpeed * (offset) / offset.getLength();
    } else if (i == 1) {
      arrows[i]->setRotation(-offset.getAngle() * 180 / M_PI +
                             _arrow->getRotation() + 22);
      speed = _arrowSpeed * (v1) / offset.getLength();
    } else if (i == 2) {
      arrows[i]->setRotation(-offset.getAngle() * 180 / M_PI +
                             _arrow->getRotation() - 22);
      speed = _arrowSpeed * (v2) / offset.getLength();
    }
    Vec2 delta = speed / _arrowSpeed;
    arrows[i]->setVisible(true);
    getScene()->addChild(arrows[i]);
    arrows[i]->runAction(RepeatForever::create(MoveBy::create(1, speed)));
    auto& lambdaArrow = arrows[i];
    if (_bowNumber == 3) {
      auto collision_detect = [space, lambdaArrow, delta](float) {
        if (cpSpaceSegmentQueryFirst(
                space, chipmunk::cpvFromVec2(lambdaArrow->getPosition()),
                chipmunk::cpvFromVec2(lambdaArrow->getPosition() + delta), 1,
                CP_SHAPE_FILTER_ALL, nullptr)) {
          lambdaArrow->stopAllActions();
          lambdaArrow->unscheduleAllCallbacks();
        }
      };
      arrows[i]->schedule(collision_detect, 0, "collistion_detect");
    } else {
      auto& lambdaArrow2 = arrows2;
      auto collision_detect = [space, lambdaArrow, delta, lambdaArrow2,
                               offset](float) {
        if (cpSpaceSegmentQueryFirst(
                space, chipmunk::cpvFromVec2(lambdaArrow->getPosition()),
                chipmunk::cpvFromVec2(lambdaArrow->getPosition() + delta), 1,
                CP_SHAPE_FILTER_ALL, nullptr)) {
          lambdaArrow2->setPosition(lambdaArrow->getPosition());
          Blink* blink = Blink::create(1.0f, 1);
          lambdaArrow2->runAction(blink);
          lambdaArrow->stopAllActions();
          lambdaArrow->unscheduleAllCallbacks();
        }
      };
      arrows2->schedule(collision_detect, 0, "collistion_detect");
    }
  }
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

/***武器——法阵***/
Magic* Magic::create(const std::string& name) {
  Magic* magic = new Magic;
  magic->Weapon::init();
  magic->setName(name);
  const auto& data = DataSet::getConfig()["weapon"][name.c_str()];
  const auto& magic_data = data["magicball"];
  magic->setSpriteFrame(
      DataSet::load_frame(magic_data["frame"].GetString(), kWeaponResolution));
  const auto& anchor_data = magic_data["anchor"].GetArray();
  magic->setAnchorPoint(
      Vec2(anchor_data[0].GetFloat(), anchor_data[1].GetFloat()));

  const auto& magic_data2 = data["magic"];
  magic->_magicSquare = Sprite::create();
  kSpriteResolution = 150;
  magic->_magicSquare->setSpriteFrame(
      DataSet::load_frame(magic_data2["frame"].GetString()));
  const auto& anchor_data2 = magic_data2["anchor"].GetArray();
  magic->_magicSquare->setAnchorPoint(
      Vec2(anchor_data2[0].GetFloat(), anchor_data2[1].GetFloat()));
  kSpriteResolution = 32;
  return magic;
}

void Magic::pointTo(Vec2 offset) {
  
}

void Magic::fire(Vec2 offset) { 
  Sprite* magicSquare;
  magicSquare = Sprite::create();
  magicSquare->setSpriteFrame(_magicSquare->getSpriteFrame());
  magicSquare->setAnchorPoint(_magicSquare->getAnchorPoint());
  magicSquare->setPosition((_owner->getPosition()));
  magicSquare->setVisible(true);
 getScene()->addChild(magicSquare);
  magicSquare->runAction(RotateBy::create(5.0f,360));
 DelayTime* delayTime = DelayTime::create(5.0f);
  FadeOut*fadeout = FadeOut::create(1.0f);
  Sequence* action = Sequence::create(
     delayTime,
                      fadeout, NULL);
  magicSquare->runAction(action);
}