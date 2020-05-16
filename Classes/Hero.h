#pragma once
#include "Mob.h"
#include "constants.h"
#include "AutoRef.h"
#include <unordered_map>
#include <unordered_set>
#include "Interaction.h"
using namespace cocos2d;
class Weapon;

// 英雄
class Hero : public Mob {
 public:

  // 每次更新都会尝试往对应的方向行走，速度单位像素每秒
  void setMoveSpeed(float vx, float vy);
  
  virtual const char* getHeroName() const = 0;

  // 拾取武器
  void pickWeapon(Weapon*);

 protected:
  // 初始化，设置update，加载动画
  bool init() override;

  // 更新时处理移动
  void update(float) override;

  // 加载对应json中的动画，并开始播放站立动画
  virtual void loadAnimation();

  // 监听键盘和鼠标移动
  virtual void registerUserInput();

  // 按键事件
  virtual void onKeyPressed(EventKeyboard::KeyCode, Event*);
  virtual void onKeyReleased(EventKeyboard::KeyCode, Event*);

  // 计算按键对应的速度
  void updateSpeed();

  // 移动速度
  Vec2 _speed;

  // 站立动画（应该可以重复使用同一个action，这里存起来）
  AutoRef<Animation> _standAnimation;

  // 行走动画
  AutoRef<Animation> _walkAnimation;

  // wasd对应的方向
  const static std::unordered_map<EventKeyboard::KeyCode, Vec2> kWasdDirections;

  // 按下的方向键
  std::unordered_set<EventKeyboard::KeyCode> _wasdPressed;

  // 移动速度的大小
  float _speedScale;

  // 手相对于左下的位置
  Vec2 _handPos;

  // 手持的武器
  Weapon* _weapon;

  // 正在互动的建筑
  Interaction* _interacting;
};

class SampleHero : public Hero {
 public:
  CREATE_FUNC(SampleHero);
  
  const char* getHeroName() const override { return "sample-man"; }
};