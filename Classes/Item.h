#pragma once
#include "cocos2d.h"
#include "Hero.h"

class Item:public cocos2d::Sprite {
 public:
  //道具功能
  virtual void Impact(Hero*)=0;
 protected:
  using Sprite::init;
};

class HealthBottle : public Item {
 public:
  //补血
  static HealthBottle* create(const std::string& name);
  void Impact(Hero*) override;
};

class MagicBottle : public Item {
 public:
   //补魔
  static MagicBottle* create(const std::string& name);
  void Impact(Hero*) override;
};