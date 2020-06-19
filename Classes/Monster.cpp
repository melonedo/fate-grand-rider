#include"cocos2d.h"
#include"Monster.h"
#include "GameScene.h"
#include"random"
#include"FlowWord.h"
USING_NS_CC;
using namespace cocos2d;

bool Monster::init() {
  if (!Mob::init()) return false;
  srand((UINT)GetCurrentTime());
  _isCounted = false;
  _isAlive = false;
  _speed.x = _speed.y = 0;
  loadAnimation();
  const auto& data = DataSet::getConfig()["monsters"][getMonsterName()];
  _hp = data["hp"].GetInt();
  const auto& hand_data = data["hand-pos"];
  _handPos = Vec2(hand_data[0].GetFloat(), hand_data[1].GetFloat());

  _visibleRange = data["visible-size"].GetInt();
  _attackRange = data["attack-size"].GetInt();
  _hp = data["hp"].GetInt();
  //武器
  this->pickWeapon(DataSet::loadWeapon(data["weapon"].GetString ()));
  this->scheduleUpdate();
  this->schedule(SEL_SCHEDULE(&Monster::updateMonster), 1.0f);
  addComponent(MonsterInteraction::create());
  return true;
}

void Monster::hide() {
  setVisible(false);
  _isAlive = false;
}

void Monster::show() {
  setVisible(true);
  _isAlive = true;
}

bool Monster::isAlive() { return _isAlive; }

void Monster::reset(const Rect rect) {
  Size visibleSize = Director::getInstance()->getVisibleSize();
  static std::uniform_int_distribution<unsigned> u(0, visibleSize.width);
  static std::default_random_engine e(time(0));
  static std::uniform_int_distribution<unsigned> v(0, visibleSize.height);
  while (true) {
    double X = u(e);
    double Y = v(e);
    if (rect.containsPoint(Point(X, Y)) &&
        rect.containsPoint(Point(X - 20, Y - 0)) &&
        rect.containsPoint(Point(X + 20, Y + 20))) {
      this->setPosition(X, Y);
      break;
    }
  }
}

void Monster::loadAnimation() {
	const auto& config = DataSet::getConfig();
	// 加载站立和行走动画
	const auto& data = DataSet::getConfig()["monsters"][getMonsterName()];
	_walkAnimation = DataSet::loadAnimation(data["walk"]);
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

void Monster::judgeAttack(Hero* hero) {
  if (this->_isAttack) return;
  int x = rand() % 100;
  if (x < 30) {
    this->_weapon->fire(Vec2(hero->getPosition().x, hero->getPosition().y));
    this->_isAttack = true;
  }
}

void Monster::monsterRun() {
  if (_dis < _visibleRange) return;
  int x = rand() % 10;
  int y = rand() % 10;
  _speed.x = (x - 5) / 2;
  _speed.y = (y - 5) / 2;
  int z = rand() % 3;
  if (z == 2) _speed.x = _speed.y = 0;
}
void Monster::pickWeapon(Weapon* weapon) {
  _weapon = weapon;
  weapon->setVisible(true);
  weapon->setPositionNormalized(_handPos);

  this->addChild(weapon, 1);
  weapon->setOwner(this);
}

void Monster::updateMonster(float delta) { this->_isAttack = false; }

void Monster::MonsterInteraction::attack(Sprite* source, float hit) {
//  if (!dynamic_cast<Hero *>(source->getParent ())) return;
  auto target = dynamic_cast<Monster*>(getOwner());
  if (!target->isAlive()) return;
  FlowWord* flowWord = FlowWord::create();
  GameScene::getRunningScene()->addChild(flowWord);
  flowWord->showWord(hit, getOwner()->getPosition());

  target->_hp -= hit;
  if (target->_hp <= 0) {
    target->die();
  }
}

void Monster::dieUpdate(float dt) { this->setVisible(false); }

void Monster::die() {
  this->_isAlive = false;
  setColor(Color3B::GRAY);
  auto backeRodate = RotateBy::create(0.1f, -10, 0);
  _body.clear();
  this->runAction(backeRodate);
  this->stopAllActions();
  this->unscheduleAllCallbacks();
  this->unscheduleUpdate();
  if (this->_weapon != NULL) {
    this->_weapon->stopAllActions();
    this->_weapon->unscheduleAllCallbacks();
    this->_weapon->unscheduleUpdate();
  }
  this->scheduleOnce(SEL_SCHEDULE(&Monster::dieUpdate), 0.5f);
}

int Monster::getHp() { return this->_hp; }

bool SampleMonster::init() {
  if (!Monster::init()) return false;
  return true;
}

bool KnifeMonster::init() {
  if (!Monster::init()) return false;
  return true;
}