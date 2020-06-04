#pragma once

#include "DataSet.h"
#include "Music.h"
#include "audio/include/AudioEngine.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"

using namespace cocos2d::ui;

class SliderEx : public Slider {
 public:
   //����һ���������ƻ���ʵ��
  static SliderEx* create();

  //������������
  void setRatio(float ratio);

  //�õ���������
  float getRatio();

 private:
  float _ratio;
};

class Pause : public cocos2d::Scene {
 public:
   //����һ����ͣ�ĳ���
  static cocos2d::Scene* createScene(Sprite* sp);

  //��ͣ�����ĳ�ʼ���������������ĵ���
  virtual bool init();

  //����Pauseʵ��
 CREATE_FUNC(Pause);

 //ʹ��Music�������Pause����private�ĳ�Ա���������ֺ͵�����������ͬһ���������Ƽ���
  friend class Music;

 private:
  static int _audioID;
  static bool _loopEnabled;
  static float _volume;
};