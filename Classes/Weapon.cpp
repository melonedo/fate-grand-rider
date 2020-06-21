#include "Weapon.h"
#include "cocos2d.h"
#include "DataSet.h"
#include "GameScene.h"
#include "CCVector.h"
#include "json/document.h"
using namespace cocos2d;
using namespace std;

void Weapon::load(const std::string& name, Weapon* weaponLoading,
                  const rapidjson::Value& data,
                  const rapidjson::Value& weapon_data) {
  weaponLoading->setName(name);
  weaponLoading->_weaponAngleOffset = weapon_data["angle-offset"].GetFloat();
  weaponLoading->setSpriteFrame(
      DataSet::loadFrame(weapon_data["frame"].GetString(), kWeaponResolution));
  weaponLoading->_hurt = weapon_data["hurt"].GetFloat();
}

void Weapon::loadfire(Weapon* weaponLoading,
                      const rapidjson::Value& weapon_data) {
  weaponLoading->_fireweapon->setSpriteFrame(
      DataSet::loadFrame(weapon_data["frame"].GetString()));
  weaponLoading->_fireweapon->setRotation(
      weapon_data["angle-offset"].GetFloat());
}

void Weapon::preparefire(Sprite* weapon, Vec2 offset) {
  weapon->setSpriteFrame(_fireweapon->getSpriteFrame());
  weapon->setAnchorPoint(_fireweapon->getAnchorPoint());

  weapon->setVisible(true);
  getScene()->addChild(weapon);
}


/***武器——弓1***/
BlinkBow* BlinkBow::createweapon(const std::string& name) {
  auto blinkbow = BlinkBow::create();
  blinkbow->Weapon::init();

  const auto& data = DataSet::getConfig()["weapon"][name.c_str()];

  const auto& blinkbow_data = data["bow"];
  blinkbow->load(name, blinkbow, data, blinkbow_data);
  /*blinkbow->setName(name);
  blinkbow->_bowAngleOffset = blinkbow_data["angle-offset"].GetFloat();
  blinkbow->setSpriteFrame(DataSet::loadFrame(
      blinkbow_data["frame"].GetString(), kWeaponResolution));
  blinkbow->_hurt = blinkbow_data["hurt"].GetFloat();*/
  blinkbow->_bowNumber = blinkbow_data["number"].GetInt();
  blinkbow->_angleConstant = blinkbow_data["angleconstant"].GetInt();


  const auto& arrow_data = data["arrow"];
  blinkbow->_fireweapon = Sprite::create();
  blinkbow->loadfire(blinkbow, arrow_data);
  /*blinkbow->_fireweapon->setSpriteFrame(
      DataSet::loadFrame(arrow_data["frame"].GetString()));
  blinkbow->_fireweapon->setRotation(arrow_data["angle-offset"].GetFloat());*/
  blinkbow->_arrowSpeed = arrow_data["speed"].GetFloat();

    const auto& arrow_data2 = data["arrow2"];
  blinkbow->_arrow2 = Sprite::create();
    blinkbow->_arrow2->setSpriteFrame(
        DataSet::loadFrame(arrow_data2["frame"].GetString()));
  blinkbow->_arrow2->setRotation(arrow_data2["angle-offset"].GetFloat());

  const auto& anchor_data = blinkbow_data["anchor"].GetArray();
  blinkbow->setAnchorPoint(
      Vec2(anchor_data[0].GetFloat(), anchor_data[1].GetFloat()));

  const auto& anchor_data2 = arrow_data["anchor"];
  blinkbow->_fireweapon->setAnchorPoint(
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
      _fireweapon->getAnchorPoint());  // 用另一个的，防止同步出错问题
  arrows2->setRotation(-offset.getAngle() * 180 / M_PI +
                       _arrow2->getRotation());
  arrows2->setVisible(false);
  getScene()->addChild(arrows2);

  arrows = Sprite::create();

  arrows->setPosition((_owner->getPosition()));
  Vec2 speed;
  auto space = GameScene::getRunningScene()->getPhysicsSpace();

  speed = _arrowSpeed * (offset) / offset.getLength();
  Vec2 delta = speed / _arrowSpeed;
 preparefire(arrows, offset);
 /* arrows->setSpriteFrame(_fireweapon->getSpriteFrame());
  arrows->setAnchorPoint(_fireweapon->getAnchorPoint());
  arrows->setVisible(true);
  getScene()->addChild(arrows);*/
 arrows->setRotation(-offset.getAngle() * 180 / M_PI +
                     _fireweapon->getRotation());
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
  float angle_offset =
      isFlippedX() ? 180 - _weaponAngleOffset : _weaponAngleOffset;
  float angle = -offset.getAngle() * 180 / M_PI + angle_offset;
  this->setRotation(angle);
}

void Weapon::drop() {
  // 保持一下引用
  AutoRef<Weapon> keep_ref(this);
  if (_owner) {
    this->removeFromParent();
    this->setPosition(_owner->getPosition());
    _owner = nullptr;
  }
  GameScene::getRunningScene()->getChildByName("map")->addChild(
      this, kMapPriorityBackground);
  this->addComponent(DroppedWeapon::create(this));
}

Bow* Bow::createweapon(const std::string& name) {
  auto bow = Bow::create();
  bow->Weapon::init();

  const auto& data = DataSet::getConfig()["weapon"][name.c_str()];
  const auto& bow_data = data["bow"];
  bow->load(name, bow, data, bow_data);
  /*bow->setName(name);
  bow->_bowAngleOffset = bow_data["angle-offset"].GetFloat();
  bow->setSpriteFrame(DataSet::loadFrame(
      bow_data["frame"].GetString(), kWeaponResolution));
  bow->_hurt = bow_data["hurt"].GetFloat();*/
  bow->_bowNumber = bow_data["number"].GetInt();
  bow->_angleConstant = bow_data["angleconstant"].GetInt();


 // bow->_hurt = bow_data["hurt"].GetInt();

  const auto& arrow_data = data["arrow"];
  bow->_fireweapon = Sprite::create();
  bow->loadfire(bow, arrow_data);
  /*bow->_fireweapon->setSpriteFrame(
      DataSet::loadFrame(arrow_data["frame"].GetString()));
  bow->_arrow->setRotation(arrow_data["angle-offset"].GetFloat());*/
  bow->_arrowSpeed = arrow_data["speed"].GetFloat();

  const auto& anchor_data = bow_data["anchor"].GetArray();
  bow->setAnchorPoint(
      Vec2(anchor_data[0].GetFloat(), anchor_data[1].GetFloat()));

  const auto& anchor_data2 = arrow_data["anchor"];
  bow->_fireweapon->setAnchorPoint(
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

    arrows[i]->setPosition((_owner->getPosition()));
    Vec2 speed;
    auto space = GameScene::getRunningScene()->getPhysicsSpace();
    arrows[i]->setRotation(-offset.getAngle() * 180 / M_PI +
                           _fireweapon->getRotation() +
                           _angleConstant * (i - 1));
    speed = _arrowSpeed * (v[i]) / offset.getLength();
    Vec2 delta = speed / _arrowSpeed;
    preparefire(arrows[i], offset);
    /*arrows[i]->setSpriteFrame(_fireweapon->getSpriteFrame());
    arrows[i]->setAnchorPoint(_fireweapon->getAnchorPoint());
    arrows[i]->setVisible(true);
    getScene()->addChild(arrows[i]);*/
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
Spear* Spear::createweapon(const std::string& name) {
  auto spear = Spear::create();
  spear->Weapon::init();

  const auto& data = DataSet::getConfig()["weapon"][name.c_str()];
  const auto& spear_data = data["spear"];
  spear->load(name, spear, data, spear_data);
  /*spear->setName(name);
  spear->_spearAngleOffset = spear_data["angle-offset"].GetFloat();
  spear->setSpriteFrame(
      DataSet::loadFrame(spear_data["frame"].GetString(), kWeaponResolution));

  spear->_hurt = spear_data["hurt"].GetFloat();*/

  const auto& anchor_data = spear_data["anchor"].GetArray();
  spear->_spearSpeed = spear_data["speed"].GetFloat();
  return spear;
}

void Spear::pointTo(Vec2 offset) {
  float angle_offset =
      isFlippedX() ? 180 - _weaponAngleOffset : _weaponAngleOffset;
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
Magic* Magic::createweapon(const std::string& name) {
  auto magic = Magic::create();
  magic->Weapon::init();
  const auto& data = DataSet::getConfig()["weapon"][name.c_str()];
  const auto& magic_data = data["magicball"];
  magic->load(name, magic, data, magic_data);
  /*magic->setName(name);
  magic->setSpriteFrame(
      DataSet::loadFrame(magic_data["frame"].GetString(), kWeaponResolution));
        magic->_hurt = magic_data["hurt"].GetFloat();*/
  const auto& anchor_data = magic_data["anchor"].GetArray();
  magic->setAnchorPoint(
      Vec2(anchor_data[0].GetFloat(), anchor_data[1].GetFloat()));


  const auto& magic_data2 = data["magic"];
  magic->_fireweapon = Sprite::create();
  magic->_fireweapon->setSpriteFrame(
      DataSet::loadFrame(magic_data2["frame"].GetString(),150));
  const auto& anchor_data2 = magic_data2["anchor"].GetArray();
  magic->_fireweapon->setAnchorPoint(
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

  magicSquare->setPosition((_owner->getPosition()));
  Vec2 centre = _owner->getPosition();
  preparefire(magicSquare, offset);
 /* magicSquare->setSpriteFrame(_fireweapon->getSpriteFrame());
  magicSquare->setAnchorPoint(_fireweapon->getAnchorPoint());
  magicSquare->setVisible(true);
  getScene()->addChild(magicSquare);*/
 magicSquare->runAction(RotateBy::create(5.0f, 360));
 DelayTime* delayTime = DelayTime::create(5.0f);
  FadeOut*fadeout = FadeOut::create(1.0f);
 Sequence*action = Sequence::create(delayTime, fadeout, NULL);
  magicSquare->runAction(action);
 auto filter = _owner->getBody().getFilter();
 //一个阵在那里转转转转转，要是一个敌人进去只能被伤害一次太假了，所以就改成转动期间一秒钟都检测一次
 auto collision_detect = [space, magicSquare, filter, hurt=_hurt, this,centre](float) {
   static int j = 0;
   auto target = space->queryPointAll(centre, 70, filter);
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
Darts* Darts::createweapon(const std::string& name) {
  auto dart = Darts::create();
  dart->Weapon::init();
  //dart->setName(name);
  const auto& data = DataSet::getConfig()["weapon"][name.c_str()];
  const auto& dart_data = data["darts"];
  dart->load(name, dart, data, dart_data);
    /*dart->setName(name);
  dart->setSpriteFrame(
      DataSet::loadFrame(dart_data["frame"].GetString(), kWeaponResolution));
  dart->_hurt = dart_data["hurt"].GetFloat();*/

  const auto& dart_out_data = data["darts"];
  dart->_fireweapon = Sprite::create();
  dart->loadfire(dart, dart_out_data);
  /*dart->_fireweapon->setSpriteFrame(
      DataSet::loadFrame(dart_out_data["frame"].GetString()));*/
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

  darts->setPosition((_owner->getPosition()));
  Vec2 speed;
  auto space = GameScene::getRunningScene()->getPhysicsSpace();
  speed = _dartSpeed * (offset) / offset.getLength();
  Vec2 delta = speed / _dartSpeed;
  preparefire(darts,offset);
  /*darts->setSpriteFrame(_fireweapon->getSpriteFrame());
  darts->setAnchorPoint(_fireweapon->getAnchorPoint());
  darts->setVisible(true);
  getScene()->addChild(darts);*/
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

RedBall* RedBall::create(const std::string& name) {
  RedBall* redBall = new RedBall;
  redBall->autorelease();
  redBall->Weapon::init();
  redBall->setName(name);
  const auto& data = DataSet::getConfig()["weapon"][name.c_str()];
  const auto& redBall_data = data["redball"];
  redBall->_hurt = redBall_data["hurt"].GetFloat();
  redBall->_redBallSpeed = redBall_data["speed"].GetFloat();

  return redBall;
}

void RedBall::pointTo(Vec2 offset) {}

void RedBall::fire(Vec2 vec) { 
  Sprite* redBall;
  int hurt = _hurt;
  redBall = Sprite::create();
  redBall->setPosition((_owner->getPosition()));
  const auto& data = DataSet::getConfig()["weapon"]["redball"];
  const auto& redBall_data = data["redball"];
  redBall->setSpriteFrame(
      DataSet::loadFrame(redBall_data["frame"].GetString(), kWeaponResolution));
  redBall->setVisible(true);
  getScene()->addChild(redBall);
  
  auto speed = _redBallSpeed;
  Vec2 _speed;
  auto X = vec.x;
  auto Y = vec.y;
  _speed.x = speed * X / sqrt(X * X + Y * Y);
  _speed.y = speed * Y / sqrt(X * X + Y * Y);
  redBall->runAction(RepeatForever::create(MoveBy::create(1.0f, _speed)));

  _speed = _speed / _speed.length();

  auto space = GameScene::getRunningScene()->getPhysicsSpace();
  auto filter = _owner->getBody().getFilter();
  auto collision_detect = [redBall,this, space, filter, hurt, _speed](float) {
    if (auto target = space->querySegmentFirst(
            redBall->getPosition(), redBall->getPosition() + _speed, filter)) {
     if (dynamic_cast<Monster*>(target)&&dynamic_cast<Monster*>(_owner)) return;
      redBall->setVisible(false);
      redBall->stopAllActions();
      redBall->unscheduleAllCallbacks();
      getInteraction(target)->attack(redBall, hurt);
    }
  };
  redBall->schedule(collision_detect, 0, "collistion_detect");
}

Knife* Knife::create(const std::string& name) {
  Knife* knife = new Knife; 
  knife->autorelease();
  knife->Weapon::init();
  knife->setName(name);
  const auto& data = DataSet::getConfig()["weapon"][name.c_str()];
  const auto& knife_data = data["knife"];
  knife->setSpriteFrame(
      DataSet::loadFrame(knife_data["frame"].GetString(), kWeaponResolution));
  knife->_hurt = knife_data["hurt"].GetFloat();
 knife->_knifeRadius = knife_data["radius"].GetFloat();
 const auto& anchor_data = knife_data["anchor"].GetArray();
 knife->setAnchorPoint(
     Vec2(anchor_data[0].GetFloat(), anchor_data[1].GetFloat()));
 return knife;
}

void Knife::pointTo(Vec2 offset) {}

void Knife::fire(Vec2 offset) {
  Sprite* knife;
  int hurt = _hurt;
  knife = Sprite::create();
  const auto& data = DataSet::getConfig()["weapon"]["knife"];
  const auto& knife_data = data["knife"];
  const auto& anchor_data = knife_data["anchor"].GetArray();
  knife->setSpriteFrame(DataSet::loadFrame(knife_data["frame"].GetString()));
  knife->setAnchorPoint(
      Vec2(anchor_data[0].GetFloat(), anchor_data[1].GetFloat()));
  knife->setPosition((_owner->getPosition()));

  auto space = GameScene::getRunningScene()->getPhysicsSpace();
  auto radius = _knifeRadius;
  knife->setVisible(true);
  getScene()->addChild(knife);
  RotateBy* rotateby = RotateBy::create(0.3f, 360);
  knife->runAction(rotateby);
  auto filter = _owner->getBody().getFilter();
  auto collision_detect = [space, knife,this, radius, filter, hurt = _hurt](float) {
    knife->setPosition(this->_owner->getPosition());
    auto targets = space->queryPointAll(knife->getPosition(), radius, filter);
    for (auto& target : targets) {
      if (dynamic_cast<Monster*>(target.sprite) && dynamic_cast<Monster*>(_owner))
        continue;
      //knife->stopAllActions();
      knife->unschedule("collistion_detect");
      getInteraction(target.sprite)->attack(knife, hurt);
    }
  };
  auto disappear = [this,knife](float) {
    knife->removeFromParent();
  };
  knife->scheduleOnce(disappear, 0.3, "disappear");
  knife->schedule(collision_detect, 0, "collistion_detect");
}