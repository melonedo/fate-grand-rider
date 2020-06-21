#include "Hero.h"
#include <unordered_map>
#include <unordered_set>
#include "DataSet.h"
#include "Endgame.h"
#include "GameScene.h"
#include "Interaction.h"
#include "Item.h"
#include "cocos2d.h"
#include "ui.h"
using namespace cocos2d;

bool Hero::init() {
  if (!Mob::init()) return false;
  // 衍生类如果不需要这些直接重写即可
  scheduleUpdate();
  loadAnimation();
  // 手的位置
  const auto& hand_data =
      DataSet::getConfig()["heroes"][getHeroName()]["hand-pos"];
  _handPos = Vec2(hand_data[0].GetFloat(), hand_data[1].GetFloat());
  // 加入互动
  addComponent(HeroInteraction::create());

  const auto& data = DataSet::getConfig()["heroes"][getHeroName()];
  _hp = data["total-hp"].GetFloat();
  _se = data["total-se"].GetFloat();
  _mp = data["total-mp"].GetFloat();
  _totalHp = data["total-hp"].GetFloat();
  _totalSe = data["total-se"].GetFloat();
  _totalMp = data["total-mp"].GetFloat();
  _ifShield = true;

  schedule(static_cast<SEL_SCHEDULE>(&Hero::timeUpdate));
  schedule(static_cast<SEL_SCHEDULE>(&Hero::shieldUpdate), 1.0f);

  return true;
}

void Hero::timeUpdate(float dt) {
  struct timeval now;
  gettimeofday(&now, NULL);
  if (now.tv_sec - _timeOfAttack == 3) {
    _ifShield = true;
  }
}

void Hero::shieldUpdate(float dt) {
  if (_se < this->getTotalSe() && _ifShield) {
    _se += 1;
  }
}

void Hero::loadAnimation() {
  const auto& config = DataSet::getConfig();
  assert(config["heroes"].IsObject());
  assert(config["heroes"].HasMember(getHeroName()));
  // 加载站立和行走动画
  const auto& data = DataSet::getConfig()["heroes"][getHeroName()];

  _walkAnimation = DataSet::loadAnimation(data["walk"]);
  _walkAnimation->setLoops(-1);
  _standAnimation = DataSet::loadAnimation(data["stand"]);
  _standAnimation->setLoops(-1);

  // 不播放的话英雄看不见
  auto animate = Animate::create(_standAnimation);
  animate->setTag(kTagStandAnimation);
  this->runAction(animate);
}

// void Hero::setMoveSpeed(float vx, float vy) { _speed = Vec2(vx, vy); }

void Hero::registerUserInput() {
  _speedScale = DataSet::getConfig()["hero-speed-scale"].GetFloat();

  auto keyboard_listener = EventListenerKeyboard::create();
  using namespace std::placeholders;
  keyboard_listener->onKeyPressed =
      std::bind(&Hero::onKeyPressed, this, _1, _2);
  keyboard_listener->onKeyReleased =
      std::bind(&Hero::onKeyReleased, this, _1, _2);
  this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(
      keyboard_listener, this);

  auto mouse_listener = EventListenerMouse::create();
  mouse_listener->onMouseMove = [this](EventMouse* event) {
    this->_weapon->pointTo(Vec2(event->getCursorX(), event->getCursorY()) -
                           designResolutionSize / 2);
  };
  mouse_listener->onMouseDown = [this](EventMouse* event) {
    this->_weapon->fire(Vec2(event->getCursorX(), event->getCursorY()) -
                        designResolutionSize / 2);
  };
  this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(
      mouse_listener, this);
}

// wasd对应的方向
const static std::unordered_map<EventKeyboard::KeyCode, Vec2> kWasdDirections{
    {EventKeyboard::KeyCode::KEY_W, {0, 1}},
    {EventKeyboard::KeyCode::KEY_A, {-1, 0}},
    {EventKeyboard::KeyCode::KEY_S, {0, -1}},
    {EventKeyboard::KeyCode::KEY_D, {1, 0}}};

void Hero::onKeyPressed(EventKeyboard::KeyCode code, Event*) {
  if (kWasdDirections.count(code) != 0) {
    _wasdPressed.insert(code);
    if (_wasdPressed.size() == 1) {
      this->stopActionByTag(kTagStandAnimation);
      auto animate = Animate::create(_walkAnimation);
      animate->setTag(kTagWalkingAnimation);
      this->runAction(animate);
    }
    if (code == EventKeyboard::KeyCode::KEY_A) {
      this->setFlippedX(true);
      _weapon->setFlippedX(true);
      _weapon->setPositionNormalized(Vec2(1 - _handPos.x, _handPos.y));
    } else if (code == EventKeyboard::KeyCode::KEY_D) {
      this->setFlippedX(false);
      _weapon->setFlippedX(false);
      _weapon->setPositionNormalized(_handPos);
    }
    updateSpeed();
  }
}

void Hero::onKeyReleased(EventKeyboard::KeyCode code, Event*) {
  if (kWasdDirections.count(code) != 0) {
    _wasdPressed.erase(code);
    updateSpeed();
    if (_wasdPressed.size() == 0) {
      this->stopActionByTag(kTagWalkingAnimation);
      auto animate = Animate::create(_standAnimation);
      animate->setTag(kTagStandAnimation);
      this->runAction(animate);
    }
  } else if (code == EventKeyboard::KeyCode::KEY_SPACE &&
             _interacting != nullptr) {
    _interacting->dialog(this);
  }
}

void Hero::updateSpeed() {
  Vec2 speed;
  for (auto key : _wasdPressed) {
    speed += kWasdDirections.at(key);
  }
  this->_speed = speed * _speedScale;
}

void Hero::update(float delta) {
  auto old_pos = this->getPosition();
  auto disp = _speed * delta;
  auto new_pos = old_pos + disp;

  // 碰撞检测
  using namespace chipmunk;
  Sprite* result = nullptr;
  auto space = GameScene::getRunningScene()->getPhysicsSpace();
  auto radius = kSpriteResolution / 4;
  if (disp != Vec2(0, 0)) {
    // 找到附近的所有物体
    auto near_obj =
        space->queryPointAll(new_pos, 2 * radius, _body.getFilter());
    // 附近没别的就直接继续
    if (near_obj.size() != 0 && disp != Vec2(0, 0)) {
      // 找到向原方向移动的最大距离
      int nearest_obj = -1;
      cpSegmentQueryInfo nearest_info;
      nearest_info.alpha = 1;
      for (int i = 0; i < near_obj.size(); i++) {
        cpSegmentQueryInfo info;
        cpShapeSegmentQuery(near_obj[i].shape, cpvFromVec2(old_pos),
                            cpvFromVec2(new_pos), radius, &info);
        if (nearest_info.alpha > info.alpha &&
            cpvdot(info.normal, cpvFromVec2(disp)) < 0) {
          nearest_obj = i;
          nearest_info = info;
        }
      }
      // 如果不能完全地移动到new_pos则进一步修正
      if (nearest_obj != -1) {
        // log("%f,%f", getPosition().x, getPosition().y);
        // 先设置好互动对象
        result = getSpriteFromShape(nearest_info.shape);
        // 除去这个障碍
        near_obj.erase(near_obj.begin() + nearest_obj);
        // 移动到贴上，然后平行切面移动
        old_pos = old_pos + disp * nearest_info.alpha;
        disp *= 1 - nearest_info.alpha;
        disp -= disp.dot(vec2FromCpv(nearest_info.normal)) *
                vec2FromCpv(nearest_info.normal);
        new_pos = old_pos + disp;
        // 再试一次
        nearest_info.alpha = 1;
        for (int i = 0; i < near_obj.size(); i++) {
          cpSegmentQueryInfo info;
          cpShapeSegmentQuery(near_obj[i].shape, cpvFromVec2(old_pos),
                              cpvFromVec2(new_pos), radius, &info);
          if (nearest_info.alpha > info.alpha &&
              !cpveql(info.normal, nearest_info.normal) &&
              cpvdot(info.normal, cpvFromVec2(disp)) < -0.01) {
            nearest_obj = i;
            nearest_info = info;
          }
        }
        new_pos = old_pos + disp * nearest_info.alpha;
      }
    }
  }

  // 保证可以碰到已经碰上的的建筑
  cpShapeFilter filter = _body.getFilter();
  filter.mask = CP_ALL_CATEGORIES;
  // 加个0.1不然碰不到
  auto stepping = space->queryPointNearest(new_pos, radius + 0.1, filter);
  if (stepping != nullptr) result = stepping;

  Interaction* interacting = nullptr;
  // 尝试互动
  if (result != nullptr) {
    interacting = getInteraction(result);
    if (interacting != _interacting) {
      interacting->touch(this);
    }
  }
  // 取消互动
  if (_interacting != nullptr && _interacting != interacting) {
    _interacting->endTouch(this);
  }
  _interacting = interacting;

  // 滚动屏幕（Size和Vec没有减法只有加法，所以倒过来）
  auto scene = this->getScene();
  float scale = DataSet::getGlobaZoomScale();
  this->setPosition(new_pos);
  scene->setPosition((-new_pos + designResolutionSize / 2) * scale);
  scene->getChildByName("static")->setPosition(new_pos - designResolutionSize /
                                                             2 / scale);
}

Weapon* Hero::pickWeapon(Weapon* weapon) {
  // 扔掉原有的武器
  if (_weapon) {
    _weapon->drop();
  }
  auto res = _weapon;
  _weapon = weapon;
  weapon->setVisible(true);
  weapon->setPositionNormalized(_handPos);
  weapon->removeFromParent();
  this->addChild(weapon, 1);
  weapon->setOwner(this);
  return res;
}

void Hero::setHp(float hp) { _hp = hp; }
void Hero::setSe(float se) { _se = se; }
void Hero::setMp(float mp) { _mp = mp; }

const float Hero::getTotalHp() { return _totalHp; }
const float Hero::getTotalSe() { return _totalSe; }
const float Hero::getTotalMp() { return _totalMp; }

const float Hero::getHp() { return _hp; }
const float Hero::getSe() { return _se; }
const float Hero::getMp() { return _mp; }

void Hero::die() {
  auto director = Director::getInstance();
  director->replaceScene(EndScene::create(false));
}

void Hero::HeroInteraction::attack(Sprite* source, float damage) {
  auto hero = dynamic_cast<Hero*>(getOwner());

  hero->_ifShield = false;

  const auto& data = DataSet::getConfig()["heroes"][hero->getHeroName()];
  if (hero->_se - damage < 0) {
    damage = damage - hero->_se;
    hero->_se = 0;
    hero->_hp -= damage;
  } else {
    hero->_se -= damage;
  }
  if (hero->_hp < 0 || hero->_hp == 0) {
    hero->die();
  }

  struct timeval now;
  gettimeofday(&now, NULL);
  hero->_timeOfAttack = now.tv_sec;
}
