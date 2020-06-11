#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class UISprite : public cocos2d::Sprite{
 public:
  bool init();

  void update(float) override;

  void setOffset(cocos2d::Vec2 offset);

  CREATE_FUNC(UISprite);

  private:
  cocos2d::Vec2 _offSet;

};

/*class UIBars :public cocos2d::ui::Slider {
  public:
  bool init();

  UIBars* create();

  void setOffset(cocos2d::Vec2 offset);

  void setPoints(int type);

  CREATE_FUNC(UIBars);

  private:
  cocos2d::Vec2 _offSet;
   float _health;
   float _shield;
   float _magic;

};*/

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