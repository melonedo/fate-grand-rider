#pragma once
#include "cocos2d.h"
#include "Hero.h"
#include "AutoRef.h"
#include "Monster.h"
// 武器
class Weapon : public cocos2d::Sprite {
 public:
  // 在鼠标移动时调用，用于改变武器方向。offset为指向的点相对英雄的位置。
  virtual void pointTo(cocos2d::Vec2 offset) = 0;
  // 向指定地点开火。offset为指向的点相对英雄的位置。
  virtual void fire(cocos2d::Vec2 offset) = 0;
  // 设置所有者，用于同步位置
  void setOwner(Mob* owner) { _owner = owner; }
  // 设置为掉落物
  void drop();
 protected:
  using Sprite::init;
  Mob* _owner;
};

// 弓
class Bow : public Weapon {
 public:
  static Bow* create(const std::string& name);
  void pointTo(cocos2d::Vec2) override;
  void fire(cocos2d::Vec2) override;
 protected:
  Bow() = default;
  // 弓图片的角度
  float _hurt;
  float _bowAngleOffset;
  int _angleConstant;
  int _bowNumber;
  // 箭
  AutoRef<Sprite> _arrow;
  AutoRef<Sprite> _arrow2;
  // 箭的速度
  float _arrowSpeed;

};

class BlinkBow : public Bow {
 public:
  static BlinkBow* create(const std::string& name);
  void fire(cocos2d::Vec2) override;

 protected:
  BlinkBow() = default;
  // 弓图片的角度
  float _hurt;
  float _bowAngleOffset;
  int _angleConstant;
  int _bowNumber;
  // 箭
  AutoRef<Sprite> _arrow;
  // 箭的速度
  float _arrowSpeed;
};


//矛
class Spear : public Weapon {
 public:
  static Spear* create(const std::string& name);
  void pointTo(cocos2d::Vec2) override;
  void fire(cocos2d::Vec2) override;

 protected:
  Spear() = default;
  float _hurt;
  // 矛图片的角度
  float _spearAngleOffset;
  // 矛刺出的速度
  float _spearSpeed;
};

//法阵
class Magic : public Weapon {
 public:
  static Magic* create(const std::string& name);
  void pointTo(cocos2d::Vec2) override;
  void fire(cocos2d::Vec2) override;

 protected:
  Magic() = default;
  float _hurt;
  AutoRef<Sprite> _magicSquare;
  friend Hero;
};

//飞镖
class Darts : public Weapon {
 public:
  static Darts* create(const std::string& name);
  void pointTo(cocos2d::Vec2) override;
  void fire(cocos2d::Vec2) override;
  AutoRef<Sprite> _dart;

 protected:
  Darts() = default;
  float _hurt;
  // 飞镖速度
  float _dartSpeed;
};

class RedBall : public Weapon {
 public:
  static RedBall* create(const std::string& name);
  void pointTo(cocos2d::Vec2) override;
  void fire(cocos2d::Vec2) override;
 protected:
  RedBall() = default;
  float _hurt;
  float _redBallSpeed;
};

class Knife : public Weapon {
 public:
  static Knife* create(const std::string& name);
  void pointTo(cocos2d::Vec2) override;
  void fire(cocos2d::Vec2) override;
 protected:
  Knife() = default;
  float _hurt;
  float _knifeRadius;
};