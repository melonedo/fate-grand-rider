#pragma once
#include <unordered_map>
#include <unordered_set>
#include "AutoRef.h"
#include "Interaction.h"
#include "Mob.h"
#include "constants.h"
class Weapon;
class Item;

// 英雄
class Hero : public Mob {
 public:
  // 每次更新都会尝试往对应的方向行走，速度单位像素每秒
  // 目前没有需要用到的地方
  // void setMoveSpeed(float vx, float vy);

  // 拾取武器，返回旧武器
  Weapon* pickWeapon(Weapon*);
  //
  Item* pickItem(Item*);
  // 监听键盘和鼠标移动
  void registerUserInput();

  //设置hp、se、mp
  void setHp(float);
  void setSe(float);
  void setMp(float);

  //获得血条等总数值初始化ui
  const float getTotalHp();
  const float getTotalSe();
  const float getTotalMp();

  //获得现有的血量等数值
  const float getHp();
  const float getSe();
  const float getMp();

  

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
  friend Interaction;  // 有些互动需要修改_interacting以防止endTouch再次访问

  // 角色的互动
  class HeroInteraction : public Interaction {
   public:
    CREATE_FUNC(HeroInteraction);
    void attack(cocos2d::Sprite* source, float) override;
  };

  // 角色的名字
  std::string _heroName;
  const char* getHeroName() { return _heroName.c_str(); }

  //更新盾
  void shieldUpdate(float);
  //更新是否回复盾值的时间
  void timeUpdate(float);

  // hit point
  float _hp;
  // shield energy
  float _se;
  // magic point
  float _mp;

  //总能量
  float _totalHp;
  float _totalSe;
  float _totalMp;

  //实现在被攻击后一段时间回复盾值
  //被攻击的时间
  int _timeOfAttack;
  //是否进行盾的回复
  bool _ifShield;

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