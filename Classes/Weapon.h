#pragma once
#include "cocos2d.h"
#include "Hero.h"
#include "AutoRef.h"

// 武器
class Weapon : public cocos2d::Sprite {
 public:
  // 在鼠标移动时调用，用于改变武器方向。offset为指向的点相对英雄的位置。
  virtual void pointTo(cocos2d::Vec2 offset) = 0;
  // 向指定地点开火。offset为指向的点相对英雄的位置。
  virtual void fire(cocos2d::Vec2 offset) = 0;
 protected:
  using Sprite::init;
  Hero* _owner;
  friend Hero;
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

  float _bowAngleOffset;

  int _bowNumber;
  // 箭
  AutoRef<Sprite> _arrow;
  AutoRef<Sprite> _arrow2;
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
  AutoRef<Sprite> _magicSquare;
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
  // 飞镖速度
  float _dartSpeed;
};