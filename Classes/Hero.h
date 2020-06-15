#pragma once
#include "Mob.h"
#include "constants.h"
#include "AutoRef.h"
#include <unordered_map>
#include <unordered_set>
#include "Interaction.h"
class Weapon;

// 英雄
class Hero : public Mob {
 public:

  // 每次更新都会尝试往对应的方向行走，速度单位像素每秒
  // 目前没有需要用到的地方
  // void setMoveSpeed(float vx, float vy);

  // 拾取武器，返回旧武器
  Weapon* pickWeapon(Weapon*);
  // 监听键盘和鼠标移动
  void registerUserInput();
  // 获取状态
  float getHP() const;
  float getShield() const;
  float getMP() const;

 protected:
  // 初始化，设置update，加载动画
  bool init() override;
  // 更新时处理移动
  void update(float) override;
  // 加载对应json中的动画，并开始播放站立动画
  void loadAnimation();
  // 按键事件
  void onKeyPressed(cocos2d::EventKeyboard::KeyCode, cocos2d::Event*);
  void onKeyReleased(cocos2d::EventKeyboard::KeyCode, cocos2d::Event*);
  // 计算按键对应的速度
  void updateSpeed();
  // 英雄死亡
  void die();

  // 移动速度
  cocos2d::Vec2 _speed;

  // 站立动画（应该可以重复使用同一个action，这里存起来）
  AutoRef<cocos2d::Animation> _standAnimation;

  // 行走动画
  AutoRef<cocos2d::Animation> _walkAnimation;

  // 按下的方向键
  std::unordered_set<cocos2d::EventKeyboard::KeyCode> _wasdPressed;

  // 移动速度的大小
  float _speedScale;

  // 手相对于左下的位置
  cocos2d::Vec2 _handPos;

  // 手持的武器
  Weapon* _weapon;

  // 正在互动的建筑
  Interaction* _interacting;
  friend Interaction; // 有些互动需要修改_interacting以防止endTouch再次访问

  // 角色的互动
  class HeroInteraction : public Interaction {
   public:
    CREATE_FUNC(HeroInteraction);
    void attack(cocos2d::Sprite* source, float) override;
  };

  // 角色的名字
  std::string _heroName;
  const char* getHeroName() { return _heroName.c_str(); }

  // 血，蓝，盾
  float _HP, _MP, _shield;
  friend HeroInteraction;
};

class SampleHero : public Hero {
 public:
  CREATE_FUNC(SampleHero);
  
 protected:
  bool init() override {
    _heroName = "sample-man";
    return Hero::init();
  }
};