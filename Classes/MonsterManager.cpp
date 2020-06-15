#include "MonsterManager.h"
#include "Hero.h"
#include "Monster.h"
#include "DataSet.h"
#include "constants.h"
#include"GameScene.h"
#include"Map.h"
bool MonsterManager::init() {
  srand((UINT)GetCurrentTime());
  // createMonsters();
  this->_aliveNum = MAX_MONSTER_NUM;
  this->scheduleUpdate();
  this->schedule(SEL_SCHEDULE(&MonsterManager::updateMonsters), 1.0f);
  _monsterManager = this;
  return true;
}

void MonsterManager::createMonsters(const Rect rect) {
  const auto& config = DataSet::getConfig();
  const auto& debug_set = config["debug-set"].GetObject();
  for (int i = 0; i < MAX_MONSTER_NUM; i++) {
    auto monster = DataSet::load_monster(debug_set["monster"].GetString());
    this->addChild(monster, kMapPrioritySprite);
    monster->reset(rect);
    monster->show();
   // monster->pickWeapon(DataSet::load_weapon(debug_set["monster-weapon"].GetString()));
    _monsterArr.pushBack(monster);
    //碰撞
    auto space = GameScene::getRunningScene()->getPhysicsSpace();
    auto filter = monster->getBody().getFilter();
    auto collision_detect = [rect,space, filter, monster](float) {
      if (space->querySegmentFirst(monster->getPosition(),
                                   monster->getPosition() + monster->_speed,
                                   filter)) {
        Size visibleSize = Director::getInstance()->getVisibleSize();
        static std::default_random_engine e(time(0));
        static std::uniform_int_distribution<unsigned> v(0, 10);
        int Z = v(e);
        if (Z == 0) {
          monster->_speed.x = 0;
          monster->_speed.y = 0;
        } else {
          auto position = monster->getPosition();
          if (!rect.containsPoint(Vec2(position.x+15,position.y))) {
            monster->_speed.x = -monster->_speed.x;
          } else if (!rect.containsPoint(Vec2(position.x - 15, position.y))) {
            monster->_speed.x = -monster->_speed.x;
          }
          if (!rect.containsPoint(Vec2(position.x, position.y+15))) {
            monster->_speed.y = -monster->_speed.y;
          } else if (!rect.containsPoint(Vec2(position.x, position.y - 15))) {
            monster->_speed.y = -monster->_speed.y;
          }
         /* monster->_speed.x = -monster->_speed.x;
          monster->_speed.y = -monster->_speed.y;*/
        }
      }
    };
    monster->schedule(collision_detect, 0, "monster-collision_detect");
  }
}

void MonsterManager::update(float dt) {
  if (isAllDead()) _room->leaveRoom();
  followRun();
}

void MonsterManager::followRun() {
  for (auto monster : _monsterArr) {
    if (!monster->isAlive()) {
      if (!monster->_isCounted) {
        monster->_isCounted = true;
        _aliveNum -= 1;
      }
      continue;
    }
    if (monster->_weapon!=NULL) {
      monster->_weapon->pointTo(
          Vec2(_hero->getPosition().x, _hero->getPosition().y));
      monster->_weapon->setPositionNormalized(
          Vec2(1 - monster->_handPos.x, monster->_handPos.y));
    }
    float x = _hero->getPosition().x - monster->getPosition().x;
    float y = _hero->getPosition().y - monster->getPosition().y;
    if (x < 0) {
      monster->setFlippedX(true);
      if (monster->_weapon!=NULL )
      monster->_weapon->setFlippedX(false);
    } else {
      monster->setFlippedX(false);
      if (monster->_weapon != NULL)
      monster->_weapon->setFlippedX(true);
    }
  }
  for (auto monster : _monsterArr) {
    if (!monster->isAlive()) continue;
    float x = _hero->getPosition().x - monster->getPosition().x;
    float y = _hero->getPosition().y - monster->getPosition().y;
    monster->_dis = sqrt(x * x + y * y);
    if (monster->_dis > monster->_visibleRange) continue;
    if (monster->_dis <= monster->_attackRange) {
      monster->judgeAttack(_hero);
      continue;
    }
    if (x < -monster->_attackRange) {
      if (monster->_isAttack) continue;
      monster->setPosition(monster->getPosition().x - 1,
                           monster->getPosition().y);

    } else if (x > monster->_attackRange) {
      if (monster->_isAttack) continue;
      monster->setPosition(monster->getPosition().x + 1,
                           monster->getPosition().y);

    } else if (x <= monster->_attackRange) {
      if (_hero->getPosition().y > monster->getPosition().y) {
        if (monster->_isAttack) continue;
        monster->setPosition(monster->getPosition().x,
                             monster->getPosition().y + 1);
      } else if (_hero->getPosition().y < monster->getPosition().y) {
        if (monster->_isAttack) continue;
        monster->setPosition(monster->getPosition().x,
                             monster->getPosition().y - 1);
      }
    }
  }
}

void MonsterManager::updateMonsters(float delta) {
  for (auto monster : _monsterArr) {
    float x = _hero->getPosition().x - monster->getPosition().x;
    float y = _hero->getPosition().y - monster->getPosition().y;
    monster->_dis = sqrt(x * x + y * y);
    if (monster->_dis >= monster->_visibleRange) {
      monster->monsterRun();
    }
  }
}

void MonsterManager ::bindHero(Hero* hero) {this-> _hero = hero; }

void MonsterManager::bindRoom(Room* room) { this->_room = room; }

bool MonsterManager::isAllDead() {
  if (!this->_aliveNum)
    return true;
  else
    return false;
}
