#pragma once
#include "Mob.h"
#include "Hero.h"
using namespace cocos2d;
class Weapon;//hh学zfn
class Monster :public Mob
{
public:
	//可视范围
	const int visibleRange = 100;
	//攻击范围
	const int attackRange = 100;

	void setMoveSpeed(float vx, float vy);
	// 初始化
	bool init() override;
	virtual void update(float dt);

	//拾取武器
	void pickWeapon(Weapon*);
	//判断是否攻击
	void judgeAttack(Hero* hero);
	//随机移动
	void monsterRun();
	//加载动画
	void loadAnimation();

	//是否活着
	bool isAlive();
	void hide();
        void reset(const Rect rect);
	void show();
	virtual const char* getMonsterName() const = 0;
	//每隔一秒重置isAttack
	void updateMonster(float delta);
	//和hero之间的距离
	float dis;
	//是否在攻击状态
	bool isAttack;

	Weapon* _weapon;

	Vec2 _handPos;

        // 移动速度
        Vec2 _speed;

       private:

	//活着没
	bool m_isAlive;
	// 行走动画
	AutoRef<Animation> _walkAnimation;
};

class SampleMonster : public Monster {
public:
	CREATE_FUNC(SampleMonster);

	const char* getMonsterName() const override { return "sample-monster"; }
};