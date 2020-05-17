#pragma once
#include "cocos2d.h"
#include "Physics.h"

// 生物，具有一个碰撞箱。
// 判断是否是生物应该先用tag判断，之后就可以直接dynamic_cast。
class Mob : public cocos2d::Sprite {
 public:
  virtual void damage(int blood) {}

  // 改变节点和碰撞箱的位置
  void setPosition(const cocos2d::Vec2& v) override {
    setPosition(v.x, v.y);
  }

  // 改变节点和碰撞箱的位置
  void setPosition(float x, float y) override;

  const chipmunk::Body& getBody() const { return _body; }

 protected:
  Mob() = default;
  // 设置锚点，并加入碰撞箱（圆形）
  bool init() override;

  // 碰撞箱
  chipmunk::Body _body;
  friend void chipmunk::initPhysicsForMob(Mob*);
  friend chipmunk::Space;
};
