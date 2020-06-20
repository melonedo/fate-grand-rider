#include "Mob.h"
#include "constants.h"
#include "Physics.h"
#include "GameScene.h"
#include "DataSet.h"
#include "Physics.h"


bool Mob::init(unsigned int category) {
  if (!Sprite::init()) return false;
  // 为保证位置正确，anchorPoint应该设为(0.5, 0.25)，即圆心
  this->setAnchorPoint(Vec2(0.5f, 0.25f));
  chipmunk::initPhysicsForMob(this);
  auto filter = cpShapeGetFilter(_body.getShape());
  filter.categories = category;
  cpShapeSetFilter(_body.getShape(), filter);
  return true;
}


void Mob::setPosition(float x, float y) {
  Sprite::setPosition(x, y);
  //cpBodySetPosition(_body.getBody(), cpv(x, y));
  _body.setPosition(x, y);
}

