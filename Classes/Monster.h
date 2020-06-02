#pragma once
#include "Mob.h"
#include "Hero.h"
class Weapon;//hhѧzfn
class Monster :public Mob
{
public:
	//���ӷ�Χ
	const int visibleRange = 100;
	//������Χ
	const int attackRange = 100;

	void setMoveSpeed(float vx, float vy);
	// ��ʼ��
	bool init() override;
	virtual void update(float dt);

	//ʰȡ����
	void pickWeapon(Weapon*);
	//�ж��Ƿ񹥻�
	void judgeAttack(Hero* hero);
	//����ƶ�
	void monsterRun();
	//���ض���
	void loadAnimation();
	//�Ƿ����
	bool isAlive();
	void hide();
	void reset();
	void show();
	virtual const char* getMonsterName() const = 0;
	//ÿ��һ������isAttack
	void updateMonster(float delta);
	//��hero֮��ľ���
	float dis;
	//�Ƿ��ڹ���״̬
	bool isAttack;

	Weapon* _weapon;

	Vec2 _handPos;
private:

	// �ƶ��ٶ�
	Vec2 _speed;
	//����û
	bool m_isAlive;
	// ���߶���
	AutoRef<Animation> _walkAnimation;
};

class SampleMonster : public Monster {
public:
	CREATE_FUNC(SampleMonster);

	const char* getMonsterName() const override { return "sample-monster"; }
};