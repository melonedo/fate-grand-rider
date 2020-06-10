#include "MonsterManager.h"
#include "Hero.h"
#include "Monster.h"
#include "DataSet.h"
#include "constants.h"
#include"GameScene.h"
bool MonsterManager::init()
{
	srand((UINT)GetCurrentTime());
	//createMonsters();
	this->scheduleUpdate();
	this->schedule(SEL_SCHEDULE(&MonsterManager::updateMonsters), 3.0f);
	return true;
}
void MonsterManager::createMonsters(const Rect rect) {
  //Ъ§Он
  const auto& config = DataSet::getConfig();
  const auto& debug_set = config["debug-set"].GetObject();
  for (int i = 0; i < MAX_MONSTER_NUM; i++) {
    auto monster = DataSet::load_monster(debug_set["monster"].GetString());
    this->addChild(monster, kMapPrioritySprite);
    monster->reset(rect);
    monster->show();
    monster->pickWeapon(DataSet::load_weapon(debug_set["weapon"].GetString()));
    m_monsterArr.pushBack(monster);
    //碰撞
    auto space = GameScene::getRunningScene()->getPhysicsSpace();
    auto filter = monster->getBody().getFilter();

    auto collision_detect = [space, filter, monster](float) {
      if (space->querySegmentFirst(monster->getPosition(),
                                   monster->getPosition() + monster->_speed,
                                   filter)) {
        Size visibleSize = Director::getInstance()->getVisibleSize();
        static std::uniform_int_distribution<unsigned> u(0, 10);
        static std::default_random_engine e(time(0));
        static std::uniform_int_distribution<unsigned> v(0, 4);
        int Z = v(e);
        if (Z == 2) {
          monster->_speed.x = 0;
          monster->_speed.y = 0;
          return;
        }
        monster->_speed.x = -monster->_speed.x;
        monster->_speed.y = -monster->_speed.y;
      }
    };
    monster->schedule(collision_detect, 0, "monster-collision_detect");
  }
}

void MonsterManager::update(float dt)
{
	FollowRun();
}

void MonsterManager::FollowRun()
{
	for (auto monster : m_monsterArr)
	{
		if (monster->_weapon)
			monster->_weapon->pointTo(Vec2(m_hero->getPosition().x, m_hero->getPosition().y)
			);
		monster->_weapon->setPositionNormalized(Vec2(1 - monster->_handPos.x, monster->_handPos.y));

		float x = m_hero->getPosition().x - monster->getPosition().x;
		float y = m_hero->getPosition().y - monster->getPosition().y;
		if (x < 0)
		{
			monster->setFlippedX(true);
			monster->_weapon->setFlippedX(false);
		}
		else
		{
			monster->setFlippedX(false);
			monster->_weapon->setFlippedX(true);
		}
	}
	for (auto monster : m_monsterArr)
	{
		float x = m_hero->getPosition().x - monster->getPosition().x;
		float y = m_hero->getPosition().y - monster->getPosition().y;
		monster->dis = sqrt(x * x + y * y);
		if (monster->dis >monster->visibleRange)
			continue;
		if (monster->dis <= monster->attackRange)
		{
			monster->judgeAttack(m_hero);
			continue;
		}
		if (x < -monster->attackRange)
		{
			if (monster->isAttack)
				continue;
			monster->setPosition(monster->getPosition().x - 1, monster->getPosition().y);
			
		}
		else if (x > monster->attackRange)
		{
			if (monster->isAttack)
				continue;
			monster->setPosition(monster->getPosition().x + 1, monster->getPosition().y);
			
		}
		else if (x <= monster->attackRange)
		{
			if (m_hero->getPosition().y > monster->getPosition().y)
			{
				if (monster->isAttack)
					continue;
				monster->setPosition(monster->getPosition().x, monster->getPosition().y+1);
			}
			else if (m_hero->getPosition().y < monster->getPosition().y)
			{
				if (monster->isAttack)
				  continue;
				monster->setPosition(monster->getPosition().x, monster->getPosition().y - 1);
			}
		}
	}
}
void MonsterManager::updateMonsters(float delta)
{
	for (auto monster : m_monsterArr)
	{
		float x = m_hero->getPosition().x - monster->getPosition().x;
		float y = m_hero->getPosition().y - monster->getPosition().y;
		monster->dis = sqrt(x * x + y * y);
		if (monster->dis >= monster->visibleRange)
		{
			monster->monsterRun();
		}
	}
}

void MonsterManager :: bingHero(Hero* hero)
{
	m_hero = hero;
}