#pragma once
#include"cocos2d.h"
#include"Monster.h"
#include"Map.h"
USING_NS_CC;
using namespace cocos2d;
//怪物数量
#define MAX_MONSTER_NUM 5

class MonsterManager : public Node {
 public:
  CREATE_FUNC(MonsterManager);
  bool isAllDead();
  //绑定hero
  void bindHero(Hero* hero);
  void bindRoom(Room* room);
  void createMonsters(const Rect rect);

 private:
  virtual bool init();
  virtual void update(float dt);
  void followRun();
  void updateMonsters(float delta);
 private:
  //存Monster们
  Vector<Monster*> _monsterArr;
  int _aliveNum;
  Hero* _hero;
  Room* _room;
  std::map<int, std::string> _monsterMap;
  MonsterManager* _monsterManager;
};