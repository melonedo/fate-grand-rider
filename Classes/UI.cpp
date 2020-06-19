#pragma once

#include "UI.h"
#include "DataSet.h"
#include "GameScene.h"
#include "Hero.h"


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
  this->addChild(_bar, kUserInterfaceBars);
}

void UIBar::setForegroundTexture(const char* type) {
  _progress = Sprite::create(type);
  _progress->setAnchorPoint(Vec2(0.0f, 0.5f));
  _progress->setPosition(Vec2(-_progress->getContentSize().width * 0.5f, 0));
  this->addChild(_progress, kUserInterfaceProgress);
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
