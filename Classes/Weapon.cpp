#include "Weapon.h"
#include "cocos2d.h"
#include "DataSet.h"
#include "GameScene.h"
#include "CCVector.h"
using namespace cocos2d;
using namespace std;

/***武器——弓1***/
BlinkBow* BlinkBow::create(const std::string& name) {
  BlinkBow* blinkbow = new BlinkBow;
  blinkbow->Weapon::init();
  blinkbow->setName(name);
  const auto& data = DataSet::getConfig()["weapon"][name.c_str()];

  const auto& blinkbow_data = data["bow"];
  blinkbow->_bowAngleOffset = blinkbow_data["angle-offset"].GetFloat();
  blinkbow->setSpriteFrame(DataSet::load_frame(
      blinkbow_data["frame"].GetString(), kWeaponResolution));
  blinkbow->_bowNumber = blinkbow_data["number"].GetInt();
  blinkbow->_angleConstant = blinkbow_data["angleconstant"].GetInt();
  blinkbow->_hurt = blinkbow_data["hurt"].GetFloat();

  blinkbow->_hurt = blinkbow_data["hurt"].GetInt();

  const auto& arrow_data = data["arrow"];
  blinkbow->_arrow = Sprite::create();
  blinkbow->_arrow->setSpriteFrame(
      DataSet::load_frame(arrow_data["frame"].GetString()));
  blinkbow->_arrow->setRotation(arrow_data["angle-offset"].GetFloat());
  blinkbow->_arrowSpeed = arrow_data["speed"].GetFloat();

    const auto& arrow_data2 = data["arrow2"];
  blinkbow->_arrow2 = Sprite::create();
    blinkbow->_arrow2->setSpriteFrame(
        DataSet::load_frame(arrow_data2["frame"].GetString()));
  blinkbow->_arrow2->setRotation(arrow_data2["angle-offset"].GetFloat());

  const auto& anchor_data = blinkbow_data["anchor"].GetArray();
  blinkbow->setAnchorPoint(
      Vec2(anchor_data[0].GetFloat(), anchor_data[1].GetFloat()));

  const auto& anchor_data2 = arrow_data["anchor"];
  blinkbow->_arrow->setAnchorPoint(
      Vec2(anchor_data2[0].GetFloat(), anchor_data2[1].GetFloat()));

  return blinkbow;
}

void BlinkBow::fire(Vec2 offset) {
  Sprite* arrows;   // 飞行中的箭
  Sprite* arrows2;  // 爆炸的箭
  int hurt = _hurt;
  arrows2 = Sprite::create();
  arrows2->setSpriteFrame(_arrow2->getSpriteFrame());
  arrows2->setAnchorPoint(
      _arrow->getAnchorPoint());  // 用另一个的，防止同步出错问题
  arrows2->setRotation(-offset.getAngle() * 180 / M_PI +
                       _arrow2->getRotation());
  arrows2->setVisible(false);
  getScene()->addChild(arrows2);

  arrows = Sprite::create();
  arrows->setSpriteFrame(_arrow->getSpriteFrame());
  arrows->setAnchorPoint(_arrow->getAnchorPoint());
  arrows->setPosition((_owner->getPosition()));
  Vec2 speed;
  auto space = GameScene::getRunningScene()->getPhysicsSpace();
  arrows->setRotation(-offset.getAngle() * 180 / M_PI + _arrow->getRotation());
  speed = _arrowSpeed * (offset) / offset.getLength();
  Vec2 delta = speed / _arrowSpeed;
  arrows->setVisible(true);
  getScene()->addChild(arrows);
  arrows->runAction(RepeatForever::create(MoveBy::create(1, speed)));
  auto& lambdaArrow = arrows;
  auto& lambdaArrow2 = arrows2;
  auto filter = _owner->getBody().getFilter();
  auto collision_detect = [space, lambdaArrow, lambdaArrow2, delta,
                           filter, hurt = _hurt](float) {
    if (auto target = space->querySegmentFirst(
            lambdaArrow->getPosition(), lambdaArrow->getPosition() + delta,
            filter)) {
      // 撤销已有的动作
      lambdaArrow->cleanup();
      // 闪烁
      lambdaArrow2->setPosition(lambdaArrow->getPosition());
      lambdaArrow2->setVisible(true);
      Vector<FiniteTimeAction*> seq{DelayTime::create(1),
                                    CallFunc::create([lambdaArrow2] {
                                      lambdaArrow2->removeFromParent();
                                    })};
      lambdaArrow2->runAction(Sequence::create(seq));
      getInteraction(target)->attack(lambdaArrow, hurt);
    }
  };
  lambdaArrow->schedule(collision_detect, 0, "collistion_detect");
}

/***武器——弓2***/
void Bow::pointTo(Vec2 offset) {
  float angle_offset = isFlippedX() ? 180 - _bowAngleOffset : _bowAngleOffset;
  float angle = -offset.getAngle() * 180 / M_PI + angle_offset;
  this->setRotation(angle);
  log("%f,%f %f", offset.x, offset.y, angle);
}

void Weapon::drop() {
  if (_owner) {
    this->removeFromParent();
    this->setPosition(_owner->getPosition());
    _owner = nullptr;
  }
  GameScene::getRunningScene()->addChild(this, kMapPriorityBackground);
  this->addComponent(DroppedWeapon::create(this));
}

Bow* Bow::create(const std::string& name) {
  Bow* bow = new Bow;
  bow->Weapon::init();
  bow->setName(name);
  const auto& data = DataSet::getConfig()["weapon"][name.c_str()];

  const auto& bow_data = data["bow"];
  bow->_bowAngleOffset = bow_data["angle-offset"].GetFloat();
  bow->setSpriteFrame(DataSet::load_frame(
      bow_data["frame"].GetString(), kWeaponResolution));
  bow->_bowNumber = bow_data["number"].GetInt();
  bow->_angleConstant = bow_data["angleconstant"].GetInt();
  bow->_hurt = bow_data["hurt"].GetFloat();

 // bow->_hurt = bow_data["hurt"].GetInt();

  const auto& arrow_data = data["arrow"];
  bow->_hurt = arrow_data["hurt"].GetInt();
  bow->_arrow = Sprite::create();
  bow->_arrow->setSpriteFrame(
      DataSet::load_frame(arrow_data["frame"].GetString()));
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

void Bow::fire(Vec2 offset) {
  Vec2 v[3];
  v[2] = Vec2((offset.y * 5 + offset.x * 12) / 15,
                 (offset.y * 12 - offset.x * 5) / 15);
  v[1] = Vec2(offset.x, offset.y);
  v[0] = Vec2((offset.x * 12 - offset.y * 5) / 15,
                 (offset.y * 12 + offset.x * 5) / 15);
  Sprite* arrows[3];
  int hurt = _hurt;
  for (int i =0; i < 3; i++) {
    arrows[i] = Sprite::create();
    arrows[i]->setSpriteFrame(_arrow->getSpriteFrame());
    arrows[i]->setAnchorPoint(_arrow->getAnchorPoint());
    arrows[i]->setPosition((_owner->getPosition()));
    Vec2 speed;
    auto space = GameScene::getRunningScene()->getPhysicsSpace();
    arrows[i]->setRotation(-offset.getAngle() * 180 / M_PI +
                        _arrow->getRotation() + _angleConstant * (i-1));
    speed = _arrowSpeed * (v[i]) / offset.getLength();
    Vec2 delta = speed / _arrowSpeed;
    arrows[i]->setVisible(true);
    getScene()->addChild(arrows[i]);
    arrows[i]->runAction(RepeatForever::create(MoveBy::create(1, speed)));
    auto lambdaArrow = arrows[i];
    auto filter = _owner->getBody().getFilter();
    auto collision_detect = [space, lambdaArrow, delta,filter,hurt=_hurt](float) {
      if (auto target = space->querySegmentFirst(
              lambdaArrow->getPosition(), lambdaArrow->getPosition() + delta,
              filter)) {
        lambdaArrow->setVisible(false);
        lambdaArrow->stopAllActions();
        lambdaArrow->unscheduleAllCallbacks();
        getInteraction(target)->attack(lambdaArrow, hurt);
      }

    };
    lambdaArrow->schedule(collision_detect, 0, "collistion_detect");
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

  spear->_hurt = spear_data["hurt"].GetFloat();

  const auto& anchor_data = spear_data["anchor"].GetArray();
  spear->_spearSpeed = spear_data["speed"].GetFloat();
  return spear;
}

void Spear::pointTo(Vec2 offset) {
  float angle_offset = isFlippedX() ? 180 - _spearAngleOffset : _spearAngleOffset;
  float angle = -offset.getAngle() * 180 / M_PI + angle_offset;
  this->setRotation(angle);
  log("%f,%f %f", offset.x, offset.y, angle);
}

void Spear::fire(Vec2 offset) {
  Vec2 speed = _spearSpeed * offset / offset.getLength();//速度同时兼任矛的长度和矛刺出的距离
  Vec2 delta = speed / _spearSpeed;
  auto space = GameScene::getRunningScene()->getPhysicsSpace();
  auto flipxAction = FlipX::create(true);
  auto moveBy = MoveBy::create(0.3f, speed);
  auto action = Sequence::create(moveBy, flipxAction, moveBy->reverse(), NULL);
  runAction(action);
  auto filter = _owner->getBody().getFilter();
    if (auto target = space->querySegmentFirst(
            _owner->getPosition(), _owner->getPosition() + speed,
            filter)) {
      getInteraction(target)->attack(this, _hurt);
    }
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
  magic->_hurt = magic_data["hurt"].GetFloat();

  magic->_hurt = magic_data["hurt"].GetInt();

  const auto& magic_data2 = data["magic"];
  magic->_magicSquare = Sprite::create();
  magic->_magicSquare->setSpriteFrame(
      DataSet::load_frame(magic_data2["frame"].GetString(),150));
  const auto& anchor_data2 = magic_data2["anchor"].GetArray();
  magic->_magicSquare->setAnchorPoint(
      Vec2(anchor_data2[0].GetFloat(), anchor_data2[1].GetFloat()));
  return magic;
}

void Magic::pointTo(Vec2 offset) {
}

void Magic::fire(Vec2 offset) {
  Sprite* magicSquare;
 int hurt = _hurt;
  magicSquare = Sprite::create();
  auto space = GameScene::getRunningScene()->getPhysicsSpace();
  magicSquare->setSpriteFrame(_magicSquare->getSpriteFrame());
  magicSquare->setAnchorPoint(_magicSquare->getAnchorPoint());
  magicSquare->setPosition((_owner->getPosition()));
  magicSquare->setVisible(true);
  getScene()->addChild(magicSquare);
 magicSquare->runAction(RotateBy::create(5.0f, 360));
 DelayTime* delayTime = DelayTime::create(5.0f);
  FadeOut*fadeout = FadeOut::create(1.0f);
 Sequence*action = Sequence::create(delayTime, fadeout, NULL);
  magicSquare->runAction(action);
 auto filter = _owner->getBody().getFilter();
 //一个阵在那里转转转转转，要是一个敌人进去只能被伤害一次太假了，所以就改成转动期间一秒钟都检测一次
 auto collision_detect = [space, magicSquare, filter, hurt=_hurt, this](float) {
   static int j = 0;
   auto target = space->queryPointAll(_owner->getPosition(), 70, filter);
   auto num = target.size();
   for (int i = 0; i < num; i++) {
     if (target[i].sprite) {
       getInteraction(target[i].sprite)->attack(magicSquare, hurt);
     }
   }
   j++;
 };
 magicSquare->schedule(collision_detect, 0.2f, 24, 0, "collistion_detect");

}

/***武器——飞镖***/
Darts* Darts::create(const std::string& name) {
  Darts* dart = new Darts;
  dart->Weapon::init();
  dart->setName(name);
  const auto& data = DataSet::getConfig()["weapon"][name.c_str()];
  const auto& dart_data = data["darts"];
  dart->setSpriteFrame(
      DataSet::load_frame(dart_data["frame"].GetString(), kWeaponResolution));
  dart->_hurt = dart_data["hurt"].GetFloat();

  const auto& dart_out_data = data["darts"];
  dart->_dart = Sprite::create();
  dart->_dart->setSpriteFrame(
      DataSet::load_frame(dart_out_data["frame"].GetString()));
  dart->_dartSpeed = dart_out_data["speed"].GetFloat();

  const auto& anchor_data = dart_out_data["anchor"].GetArray();
  dart->setAnchorPoint(
      Vec2(anchor_data[0].GetFloat(), anchor_data[1].GetFloat()));
  return dart;
}

void Darts::pointTo(Vec2 offset) {

}

void Darts::fire(Vec2 offset) {
  Sprite* darts;
  int hurt = _hurt;
   darts = Sprite::create();
  darts->setSpriteFrame(_dart->getSpriteFrame());
   darts->setAnchorPoint(_dart->getAnchorPoint());
  darts->setPosition((_owner->getPosition()));
  Vec2 speed;
  auto space = GameScene::getRunningScene()->getPhysicsSpace();
  speed = _dartSpeed * (offset) / offset.getLength();
  Vec2 delta = speed / _dartSpeed;
  darts->setVisible(true);
  getScene()->addChild(darts);
  MoveBy* moveby = MoveBy::create(1, 2*speed);
  RotateBy* rotateby = RotateBy::create(1.5f,1080);
  Action* action = Spawn::create(moveby, rotateby, NULL);
  darts->runAction(action);
  auto filter = _owner->getBody().getFilter();
  auto collision_detect = [space, darts, delta, filter,hurt=_hurt](float) {
    if (auto target = space->querySegmentFirst(
            darts->getPosition(), darts->getPosition() + delta,
            filter)) {
      darts->stopAllActions();
      darts->unscheduleAllCallbacks();
      getInteraction(target)->attack(darts, hurt);
    }
  };
  darts->schedule(collision_detect, 0, "collistion_detect");
}
