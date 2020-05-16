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
  // 箭
  AutoRef<Sprite> _arrow;
  // 箭的速度
  float _arrowSpeed;
};