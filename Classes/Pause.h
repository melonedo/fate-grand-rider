#pragma once

#include "DataSet.h"
#include "Music.h"
#include "audio/include/AudioEngine.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"

using namespace cocos2d::ui;

class SliderEx : public Slider {
 public:
   //创建一个音量控制滑块实例
  static SliderEx* create();

  //设置音量比率
  void setRatio(float ratio);

  //得到音量比率
  float getRatio();

 private:
  float _ratio;
};

class Pause : public cocos2d::Scene {
 public:
   //����һ����ͣ�ĳ���
  static cocos2d::Scene* createScene(Sprite* sp);

  //暂停场景的初始化，包括了音量的调节
  bool init()override;

  //创建Pause实例
 CREATE_FUNC(Pause);

 //使得Music类可以与Pause共用private的成员（背景音乐和调节音量共用同一个音量控制键）
  friend class Music;

 private:
  static int _audioID;
  static bool _loopEnabled;
  static float _volume;
};