#pragma once
#include "cocos2d.h"

// 可互动建筑的互动，通过继承Component的方法往里面注入代码
class Interaction : public cocos2d::Component {
 public:
  // 靠近时触发
  virtual void touch() = 0;
  // 靠近并空格时触发
  virtual void dialog() = 0;
 protected:
  using cocos2d::Component::init;
};

// 示例互动：躲藏。互动可以进去，再互动会出来。
class HideSpot : public Interaction {
 public:
  CREATE_FUNC(HideSpot);
  virtual void touch() override { cocos2d::log("You can hide here."); }
  virtual void dialog() override {
    cocos2d::log("Yaho.");
    using cocos2d::Director;
    auto hero =
        Director::getInstance()->getRunningScene()->getChildByTag(kTagHero);
    auto bbox = this->getOwner()->getBoundingBox();
    if (bbox.containsPoint(hero->getPosition())) {
      hero->setPosition(bbox.getMidX(), bbox.getMidY() - bbox.size.height);
    } else {
      hero->setPosition(bbox.getMidX(), bbox.getMidY());
    }
  }

 protected:
   using Interaction::init;
};