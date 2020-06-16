#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "GameScene.h"

class UIBar:public cocos2d::Sprite {
 public:
   //初始化变量
  bool init();
  //创建实例
  static UIBar* create();

  //设置背景框
  void setBackgroundTexture(const char* type);
  //设置前景条
  void setForegroundTexture(const char* type);
  //设置总血量等
  void setTotalProgress(float total);
  //设置现有血量
  void setCurrentProgress(float progress);
  //得到现有血量
  float getCurrentProgress() const;
  //得到总血量
  float getTotalProgress() const;

 private:
  cocos2d::Sprite* _bar;
  cocos2d::Sprite* _progress;
  float _totalProgress;
  float _currentProgress;
  float _scale;
};