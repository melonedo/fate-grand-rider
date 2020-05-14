#pragma once
#include "cocos2d.h"

// 生物，这里没啥共同的可用。
// 判断是否是生物应该先用tag判断，之后就可以直接dynamic_cast。
class Mob : public cocos2d::Sprite {
 public:
  virtual void damage(int blood) {}

 protected:
  bool init() override {
    if (!Sprite::init()) return false;
    return true;
  }
};
