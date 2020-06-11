#pragma once

#include "UI.h"
#include "DataSet.h"
#include "GameScene.h"
#include "Hero.h"

using namespace cocos2d;

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

  auto healthbar = UIBar::create();
  healthbar->setPosition(
      Vec2(position.x - visibleSize.width / 2 + bgSize.width / 2 + 25,
           position.y + visibleSize.height / 2 - 10));
  healthbar->setBackgroundTexture(data["bar"].GetString());
  healthbar->setForegroundTexture(data["health-progress"].GetString());
  healthbar->setTotalProgress(120.0f);
  healthbar->setCurrentProgress(22.0f);
  this->addChild(healthbar, kBars);

  auto shieldbar = UIBar::create();
  shieldbar->setPosition(
      Vec2(position.x - visibleSize.width / 2 + bgSize.width / 2 + 25,
           position.y + visibleSize.height / 2 - 20));
  shieldbar->setBackgroundTexture(data["bar"].GetString());
  shieldbar->setForegroundTexture(data["shield-progress"].GetString());
  shieldbar->setTotalProgress(120.0f);
  shieldbar->setCurrentProgress(22.0f);
  this->addChild(shieldbar, kBars);

  auto magicbar = UIBar::create();
  magicbar->setPosition(
      Vec2(position.x - visibleSize.width / 2 + bgSize.width / 2 + 25,
           position.y + visibleSize.height / 2 - 30));
  magicbar->setBackgroundTexture(data["bar"].GetString());
  magicbar->setForegroundTexture(data["magic-progress"].GetString());
  magicbar->setTotalProgress(120.0f);
  magicbar->setCurrentProgress(22.0f);
  this->addChild(magicbar, kBars);

  scheduleUpdate();
  return true;
}

void UISprite::setOffset(Vec2 offset) { _offSet = offset; }

void UISprite::update(float delta) {}

bool UIBar::init() {
  _bar = NULL;
  _progress = NULL;
  _totalProgress = 0.0f;
  _currentProgress = 0.0f;
  _scale = 1.0f;
  return true;
}

UIBar* UIBar::create() {
  auto ret = new (std::nothrow) UIBar;
  if (ret && ret->init()) {
    ret->autorelease();
    return ret;
  }
  CC_SAFE_DELETE(ret);
  return ret;
}

void UIBar::setBackgroundTexture(const char* type) {
  _bar = Sprite::create(type);
  this->addChild(_bar, kBars);
}

void UIBar::setForegroundTexture(const char* type) {
  _progress = Sprite::create(type);
  _progress->setAnchorPoint(Vec2(0.0f, 0.5f));
  _progress->setPosition(Vec2(-_progress->getContentSize().width * 0.5f, 0));
  this->addChild(_progress,kProgress);
}

void UIBar::setTotalProgress(float total) {
  if (_progress == NULL) return;
  _scale = _progress->getContentSize().width / total;
  _totalProgress = total;
}

void UIBar::setCurrentProgress(float progress) {
  if (_progress == NULL) return;
  if (progress < 0.0f) progress = 0.0f;
  if (progress > _totalProgress) progress = _totalProgress;
  _currentProgress = progress;
  float rectWidth = progress * _scale;
  const Vec2 from = _progress->getTextureRect().origin;
  _progress->setTextureRect(
      Rect(from.x, from.y, rectWidth, _progress->getContentSize().height));
}

float UIBar::getCurrentProgress() const { return _currentProgress; }

float UIBar::getTotalProgress() const { return _totalProgress; }

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