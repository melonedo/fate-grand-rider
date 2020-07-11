#pragma once
#include "Item.h"
#include "DataSet.h"
#include "GameScene.h"

HealthBottle* HealthBottle::create(const std::string& name) {
  auto ret = new (std::nothrow) HealthBottle();
  const auto& data = DataSet::getConfig()["item"][name.c_str()];

  if (ret && ret->init()) {
    ret->setSpriteFrame(
        DataSet::loadFrame(data["frame"].GetString(), kItemResolution));
    ret->autorelease();

    return ret;
  }
  CC_SAFE_DELETE(ret);
  return ret;
}

void HealthBottle::Impact(Hero* hero) {
  const auto& data =
      DataSet::getConfig()["item"]["health-bottle"]["health-point"];
  float healthpoint = data.GetFloat() + hero->getHp();
  if (healthpoint > hero->getTotalHp()) {
    hero->setHp(hero->getTotalHp());
  } else {
    hero->setHp(healthpoint);
  }
}

MagicBottle* MagicBottle::create(const std::string& name) {
  auto ret = new (std::nothrow) MagicBottle();
  const auto& data = DataSet::getConfig()["item"][name.c_str()];

  if (ret && ret->init()) {
    ret->setSpriteFrame(
        DataSet::loadFrame(data["frame"].GetString(), kItemResolution));
    ret->autorelease();

    return ret;
  }
  CC_SAFE_DELETE(ret);
  return ret;
}

void MagicBottle::Impact(Hero* hero) {
  const auto& data =
      DataSet::getConfig()["item"]["magic-bottle"]["magic-point"];
  float magicpoint = data.GetFloat() + hero->getMp();
  if (magicpoint > hero->getTotalMp()) {
    hero->setMp(hero->getTotalMp());
  } else {
    hero->setMp(magicpoint);
  }
}