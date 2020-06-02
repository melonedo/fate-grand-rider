#pragma once
#include"cocos2d.h"
#include"Monster.h"
USING_NS_CC;

//��������
#define MAX_MONSTER_NUM 5

class MonsterManager :public Node
{
public:
	CREATE_FUNC(MonsterManager);
	virtual bool init();
	virtual void update(float dt);
	//��hero
	void bingHero(Hero* hero);
	//�򵥵�AI
	void FollowRun();
	void updateMonsters(float delta);
private:
	void createMonsters();
private:
	//��Monster��
	Vector<Monster*>m_monsterArr;
	Hero* m_hero;
};