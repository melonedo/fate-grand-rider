#include"cocos2d.h"
#include"Monster.h"
#include "GameScene.h"
#include"random"
#include"FlowWord.h"
USING_NS_CC;
using namespace cocos2d;

bool Monster::init()
{
	if (!Mob::init()) return false;

	srand((UINT)GetCurrentTime());
	m_isAlive = false;
	_speed.x = _speed.y = 0;
	loadAnimation();
	const auto& hand_data =
		DataSet::getConfig()["monsters"][getMonsterName()]["hand-pos"];
	_handPos = Vec2(hand_data[0].GetFloat(), hand_data[1].GetFloat());
	this->scheduleUpdate();
	this->schedule(SEL_SCHEDULE(&Monster::updateMonster), 1.0f);
        addComponent(MonsterInteraction::create());
	return true;
}

void Monster::hide()
{
	setVisible(false);
	m_isAlive = false;
}
void Monster::show()
{
	setVisible(true);
	m_isAlive = true;
}
bool Monster::isAlive()
{
	return m_isAlive;
}
void Monster::reset(const Rect rect) {
  Size visibleSize = Director::getInstance()->getVisibleSize();
  static std::uniform_int_distribution<unsigned> u(0, visibleSize.width);
  static std::default_random_engine e(time(0));

  static std::uniform_int_distribution<unsigned> v(0, visibleSize.height);
  while (true) {
    double X = u(e);
    double Y = v(e);
    if (rect.containsPoint(Point(X, Y))) {
      this->setPosition(X, Y);
      break;
    }
  }
}

void Monster::loadAnimation() {
	const auto& config = DataSet::getConfig();

	// 加载站立和行走动画
	const auto& data = DataSet::getConfig()["monsters"][getMonsterName()];
	_walkAnimation = DataSet::load_animation(data["walk"]);
	_walkAnimation->setLoops(-1);

	// 不播放的话怪物看不见
	auto animate = Animate::create(_walkAnimation);
	animate->setTag(kTagStandAnimation);
	this->runAction(animate);

}



void Monster::setMoveSpeed(float vx, float vy) { _speed = Vec2(vx, vy); }
void Monster::update(float dt) {

  this->setPosition(this->getPosition().x + _speed.x,
                    this->getPosition().y + _speed.y);
}

void Monster::judgeAttack(Hero* hero)
{
	if (this->isAttack)
		return;
	int x = rand() % 100;
	if (x < 30)
	{
		this->_weapon->fire(Vec2(hero->getPosition().x, hero->getPosition().y));
		this->isAttack = true;
	}
}
void Monster::monsterRun()
{
	if (dis <visibleRange)
		return;
	int x = rand() % 10;
	int y = rand() % 10;
	_speed.x = (x - 5)/2;
	_speed.y = (y - 5)/2;
	int z = rand() % 3;
	if (z == 2)
		_speed.x = _speed.y = 0;
}
void Monster::pickWeapon(Weapon* weapon)
{
	_weapon = weapon;
	weapon->setVisible(true);
	weapon->setPositionNormalized(_handPos);

	this->addChild(weapon, 1);
        weapon->setOwner(this);
}

void Monster::updateMonster(float delta)
{
	this->isAttack = false;
}

void Monster::MonsterInteraction::attack(Sprite*, float hit) {
  FlowWord* flowWord = FlowWord::create();
  GameScene::getRunningScene()->addChild(flowWord);
  flowWord->showWord(hit, getOwner()->getPosition());
}