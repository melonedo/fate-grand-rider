#pragma once

#include "DataSet.h"
#include "Music.h"
#include "audio/include/AudioEngine.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"

using namespace cocos2d::ui;

class SliderEx : public Slider {
 public:
  static SliderEx* create() {
    auto ret = new (std::nothrow) SliderEx();
    const auto& data = DataSet::getConfig()["pause"]["volume-control"];

    if (ret && ret->init()) {
      ret->loadBarTexture(data["slider-track"].GetString());
      ret->loadSlidBallTextures(
          data["silder-thumb"].GetString(),
          data["silder-thumb"].GetString(), "");
      ret->loadProgressBarTexture(data["slider-progress"].GetString());
      ret->setTouchEnabled(true);

      ret->autorelease();

      return ret;
    }
    CC_SAFE_DELETE(ret);
    return ret;
  }

  void setRatio(float ratio) {
    ratio = cocos2d::clampf(ratio, 0.0f, 1.0f);

    _ratio = ratio;
    setPercent(100 * _ratio);
  }

  float getRatio() {
    _ratio = 1.0f * _percent / _maxPercent;
    return _ratio;
  }

 private:
  float _ratio;
};

class Pause : public cocos2d::Scene {
 public:
  static cocos2d::Scene* createScene();

  virtual bool init();

 CREATE_FUNC(Pause);

  friend class Music;

 private:
  static int _audioID;
  static bool _loopEnabled;
  static float _volume;
};