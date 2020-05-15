#include "Mob.h"
#include "constants.h"
#include "Physics.h"
#include "GameScene.h"
#include "DataSet.h"
#include "Physics.h"


bool Mob::init() {

  if (!Sprite::init()) return false;
  // Ϊ��֤λ����ȷ��anchorPointӦ����Ϊ(0.5, 0.25)����Բ��
  this->setAnchorPoint(Vec2(0.5f, 0.25f));
  //setAnchorPoint(Vec2(0, 0));
  chipmunk::initPhysicsForMob(this);
  return true;
}


void Mob::setPosition(float x, float y) {
  Sprite::setPosition(x, y);
  cpBodySetPosition(_body.getBody(), cpv(x, y));
}

