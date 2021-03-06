#pragma once
#include "Mob.h"
#include "Hero.h"
using namespace cocos2d;
class Weapon;
class MonsterManager;
class Monster : public Mob {
  friend MonsterManager;

 public:
  void setMoveSpeed(float vx, float vy);
  //是否活着
  bool isAlive();
  //获取血量
  int getHp();
 protected:
  // 初始化
  bool init() override;
  virtual void update(float dt);
  //可视范围
  int _visibleRange = 100;
  //攻击范围
  int _attackRange = 100;
  //判断是否攻击
  void judgeAttack(Hero* hero);
  //随机移动
  void monsterRun();
  //加载动画
  void loadAnimation();
  //获取怪物名字
  virtual const char* getMonsterName() const = 0;
  //每隔x秒重置isAttack
   void updateMonster(float delta);
   int _updateAttackTime;
    //拾取武器
  void pickWeapon(Weapon*);
  //武器
  Weapon* _weapon;
  // 移动速度
  Vec2 _speed;
  //血量
  int _hp;
 private:
  //死亡之后该做的事
  void die();
  //死亡一秒后消失
  void dieUpdate(float dt);
  //怪物消失
  void hide();
  //重置怪物的位置
  void reset(const Rect rect);
  //怪物出现
  void show();
  //和hero之间的距离
  float _dis;
  //是否在攻击状态
  bool _isAttack;
  //活着没
  bool _isAlive;
  //手的位置
  Vec2 _handPos;
  // 行走动画
  AutoRef<Animation> _walkAnimation;
  //是否已经被计数，用于计算活着的怪物的数量
  bool _isCounted;

  class MonsterInteraction : public Interaction {
   public:
    CREATE_FUNC(MonsterInteraction);
    void attack(cocos2d::Sprite* source, float) override;
  };
};

class SampleMonster : public Monster {
 public:
  bool init() override;
  CREATE_FUNC(SampleMonster);
  const char* getMonsterName() const override { return "sample-monster"; }
};

class KnifeMonster : public Monster {
 public:
  bool init() override;
  CREATE_FUNC(KnifeMonster);
  const char* getMonsterName() const override { return "knife-monster"; }
};