#pragma once
#include"cocos2d.h"
#include"Monster.h"
USING_NS_CC;
using namespace cocos2d;
//怪物数量
#define MAX_MONSTER_NUM 5

class MonsterManager : public Node {
 public:
  CREATE_FUNC(MonsterManager);
  bool isAllDead();
  //绑定hero
  void bingHero(Hero* hero);
  void createMonsters(const Rect rect);
 private:
  virtual bool init();
  virtual void update(float dt);
  void FollowRun();
  void updateMonsters(float delta);
 private:
  //存Monster们
  Vector<Monster*> _monsterArr;
  int _aliveNum;
  Hero* _hero;
};