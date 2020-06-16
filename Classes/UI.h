#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "GameScene.h"

class UISprite : public cocos2d::Sprite{
 public:
  bool init();
  
  static void addUI(StaticNode&);

  CREATE_FUNC(UISprite);

  private:
  cocos2d::Vec2 _offSet;

};

class UIBar:public cocos2d::Sprite {
 public:
  bool init();
  static UIBar* create();

  void setBackgroundTexture(const char* type);
  void setForegroundTexture(const char* type);
  void setTotalProgress(float total);
  void setCurrentProgress(float progress);
  float getCurrentProgress() const;
  float getTotalProgress() const;

 private:
  cocos2d::Sprite* _bar;
  cocos2d::Sprite* _progress;
  float _totalProgress;
  float _currentProgress;
  float _scale;
};