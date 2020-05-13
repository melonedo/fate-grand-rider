#pragma once
#include "Mob.h"
#include "constants.h"
#include "AutoRef.h"

// 英雄
class Hero : public Mob {
 public:

  // 每次更新都会尝试往对应的方向行走，速度单位像素每秒
  void setMoveSpeed(float vx, float vy);
  
  virtual const char* getHeroName() const = 0;

 protected:
  // 移动速度
  float _vx, _vy;
  // 初始化，设置update，加载动画
  bool init() override;
  // 更新时处理移动
  void update(float) override;
  // 站立动画（应该可以重复使用同一个action，这里存起来）
  AutoRef<cocos2d::Animation> _standAnimation;
  // 行走动画
  AutoRef<cocos2d::Animation> _walkAnimation;
};

class SampleHero : public Hero {
 public:
  CREATE_FUNC(SampleHero);
  
  const char* getHeroName() const override { return "sample-man"; }
};