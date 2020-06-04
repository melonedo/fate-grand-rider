#pragma once
#include"cocos2d.h"
#include"Monster.h"
USING_NS_CC;
using namespace cocos2d;
//怪物数量
#define MAX_MONSTER_NUM 5

class MonsterManager :public Node
{
public:
	CREATE_FUNC(MonsterManager);
	virtual bool init();
	virtual void update(float dt);
	//绑定hero
	void bingHero(Hero* hero);
	//简单的AI
	void FollowRun();
	void updateMonsters(float delta);

	void createMonsters(const Rect rect);
private:
	//存Monster们
	Vector<Monster*>m_monsterArr;
	Hero* m_hero;
};