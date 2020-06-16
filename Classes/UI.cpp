#pragma once

#include "UI.h"
#include "DataSet.h"
#include "GameScene.h"
#include "Hero.h"

using namespace cocos2d;

bool UISprite::init() {
  if (!Sprite::init()) return false;
  

  

  /*auto shieldbar = UIBar::create();
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
  this->addChild(magicbar, kBars);*/

  return true;
}

/*void UISprite::addUI(StaticNode& node) {
  const auto& data = DataSet::getConfig()["UI"]["bars"];

  auto bgBars = cocos2d::Sprite::create(data["bg-bars"].GetString());
  bgBars->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
  bgBars->setPosition(0, node->getVisibleSize().height);
  bgBars->setGlobalZOrder(kUserInterfaceBackground);
  node->addChild(bgBars);

  auto health = cocos2d::Sprite::create(data["health"].GetString());
  health->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
  health->setPosition(3, node->getVisibleSize().height - 1);
  health->setGlobalZOrder(kBars);
  node->addChild(health);

  auto shield = cocos2d::Sprite::create(data["shield"].GetString());
  shield->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
  shield->setPosition(3, node->getVisibleSize().height - 11);
  shield->setGlobalZOrder(kBars);
  node->addChild(shield);

  auto magic = cocos2d::Sprite::create(data["magic"].GetString());
  magic->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
  magic->setPosition(3, node->getVisibleSize().height - 21);
  magic->setGlobalZOrder(kBars);
  node->addChild(magic);

  auto healthbar = UIBar::create();
  healthbar->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
  healthbar->setPosition(40, node.getVisibleSize().height - 1);
  healthbar->setBackgroundTexture(data["bar"].GetString());
  healthbar->setForegroundTexture(data["health-progress"].GetString());
  healthbar->setTotalProgress(120.0f);
  healthbar->setCurrentProgress(22.0f);
  node.addChild(healthbar, kBars);
}*/


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
  this->addChild(_progress, kProgress);
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
