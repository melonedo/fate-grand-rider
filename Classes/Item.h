#pragma once
#include "cocos2d.h"
#include "Hero.h"

class Item:public cocos2d::Sprite {
 public:
  void setOwner(Mob* owner) { _owner = owner; }
  virtual void Impact()=0;
 protected:
  using Sprite::init;
  Mob* _owner;
};

class HealthBottle : public Item {
 public:
  static HealthBottle* create(const std::string& name);
  void Impact() override;
};

class MagicBottle : public Item {
 public:
  static MagicBottle* create(const std::string& name);
  void Impact() override;
};