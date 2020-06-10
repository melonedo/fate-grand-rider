#pragma once

#include "UI.h"
#include "DataSet.h"
#include "GameScene.h"
#include "Hero.h"

bool UISprite::init() {
  if (!Sprite::init()) return false;

  Vec2 position =
      static_cast<Hero*>(GameScene::getRunningScene()->getChildByTag(kTagHero))
          ->getPosition();
  auto visibleSize = Director::getInstance()->getVisibleSize();
  Vec2 origin = Director::getInstance()->getVisibleOrigin();
  float scale = DataSet::getGlobaZoomScale();

  const auto& data = DataSet::getConfig()["UI"]["bars"];

  auto bgBars = cocos2d::Sprite::create(data["bg-bars"].GetString());
  auto bgSize = bgBars->getContentSize();
  bgBars->setPosition(
      Vec2(position.x - visibleSize.width / 2 + bgSize.width / 2 + 20,
           position.y + visibleSize.height / 2 - 20));
  this->addChild(bgBars, kUserInterfaceBackground);

  auto health = cocos2d::Sprite::create(data["health"].GetString());
  auto healthSize = health->getContentSize();
  health->setPosition(
      Vec2(position.x - visibleSize.width / 2 + bgSize.width / 2 + 3,
           position.y + visibleSize.height / 2 - 10));
  this->addChild(health, kBars);

  auto shield = cocos2d::Sprite::create(data["shield"].GetString());
  auto shieldSize = shield->getContentSize();
  shield->setPosition(
      Vec2(position.x - visibleSize.width / 2 + bgSize.width / 2 + 3,
           position.y + visibleSize.height / 2 - 20));
  this->addChild(shield, kBars);

  auto magic = cocos2d::Sprite::create(data["magic"].GetString());
  auto magicSize = magic->getContentSize();
  magic->setPosition(
      Vec2(position.x - visibleSize.width / 2 + bgSize.width / 2 + 3,
           position.y + visibleSize.height / 2 - 30));
  this->addChild(magic, kBars);

  scheduleUpdate();
  return true;
}

void UISprite::setOffset(Vec2 offset) { _offSet = offset; }

void UISprite::update(float delta) {}

/*void UIBar::setBackgroundTexture(const char* type) {
  _bar = Sprite::create(type);
  this->addChild(_bar, kBars);
}

void UIBar::setForegroundTexture(const char* type) {
  _progress = Sprite::create(type);
  _progress->setAnchorPoint(Vec2(0.0f, 0.5f));
  _progress->setPosition(Vec2(-_progress->getContentSize().width * 0.5f, 0));
  this->addChild(_progress);
}

void UIBar::setTotalProgress(float total) {
  if (_progress == NULL) {
    return;
  }
}*/

/*bool UIBars::init() {
  if (!Slider::init()) return false;

  Vec2 position =
      static_cast<Hero*>(GameScene::getRunningScene()->getChildByTag(kTagHero))
          ->getPosition();
  auto visibleSize = Director::getInstance()->getVisibleSize();
  Vec2 origin = Director::getInstance()->getVisibleOrigin();
  float scale = DataSet::getGlobaZoomScale();

  auto health = UIBars::create();
  auto healthSize = health->getContentSize();
  health->setPercent(_health * 100);
  health->setPosition(
      Vec2(position.x - visibleSize.width / 2 + healthSize.width / 2 + 25,
           position.y + visibleSize.height / 2 - 10));
  addChild(health);
  return true;
}*/

/*UIBars* UIBars::create() {
  auto ret = new (std::nothrow) UIBars();
  const auto& data = DataSet::getConfig()["UI"]["bars"];

  if (ret && ret->init()) {
    ret->loadBarTexture(data["bar"].GetString());
    ret->loadSlidBallTextures(data["bar-thumb"].GetString(),
                              data["bar-thumb"].GetString(), "");
    ret->loadProgressBarTexture(data["health-progress"].GetString());
    ret->setTouchEnabled(false);

    ret->autorelease();

    return ret;
  }
  CC_SAFE_DELETE(ret);
  return ret;
}*/

/*UIBars* UIBars::load_shield_bar() {
  auto ret = new (std::nothrow) UIBars();
  const auto& data = DataSet::getConfig()["UI"]["bars"];

  if (ret && ret->init()) {
    ret->loadBarTexture(data["bar"].GetString());
    ret->loadSlidBallTextures(data["bar-thumb"].GetString(),
                              data["bar-thumb"].GetString(), "");
    ret->loadProgressBarTexture(data["shield-progress"].GetString());
    ret->setTouchEnabled(false);

    ret->autorelease();

    return ret;
  }
  CC_SAFE_DELETE(ret);
  return ret;
}

UIBars* UIBars::load_magic_bar() {
  auto ret = new (std::nothrow) UIBars();
  const auto& data = DataSet::getConfig()["UI"]["bars"];

  if (ret && ret->init()) {
    ret->loadBarTexture(data["bar"].GetString());
    ret->loadSlidBallTextures(data["bar-thumb"].GetString(),
                              data["bar-thumb"].GetString(), "");
    ret->loadProgressBarTexture(data["magic-progress"].GetString());
    ret->setTouchEnabled(false);

    ret->autorelease();

    return ret;
  }
  CC_SAFE_DELETE(ret);
  return ret;
}*/