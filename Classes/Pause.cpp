#pragma once

#include "Pause.h"
#include "DataSet.h"

// Pause场景实现
Scene* Pause::createScene() { return Pause::create(); }

int Pause::_audioID = AudioEngine::INVALID_AUDIO_ID;
bool Pause::_loopEnabled =
    DataSet::getConfig()["music"]["loop-enabled"].GetBool();
float Pause::_volume = DataSet::getConfig()["music"]["init-volume"].GetFloat();

bool Pause::init() {
  if (!Scene::init()) {
    return false;
  }

  auto visibleSize = Director::getInstance()->getVisibleSize();
  Vec2 origin = Director::getInstance()->getVisibleOrigin();
  auto& layerSize = this->getContentSize();

  auto label = Label::createWithTTF("volume", "fonts/Marker Felt.ttf", 24);
  label->setPosition(Vec2(origin.x + visibleSize.width / 8,
                          origin.y + visibleSize.height / 2));
  this->addChild(label);

  auto volumeSlider = SliderEx::create();
  volumeSlider->setPercent(_volume * 100);
  volumeSlider->addEventListener([&](Ref* sender, Slider::EventType event) {
    SliderEx* slider = dynamic_cast<SliderEx*>(sender);
    _volume = slider->getRatio();
    if (_audioID != AudioEngine::INVALID_AUDIO_ID) {
      AudioEngine::setVolume(_audioID, _volume);
    }
  });
  volumeSlider->setPosition(Vec2(origin.x + visibleSize.width / 2,
                                 origin.y + visibleSize.height / 2));

  addChild(volumeSlider);

  return true;
}

/*
用pushScene实现场景暂停
*/

//暂停背景音量控制滑块实现
SliderEx* SliderEx::create() {
  auto ret = new (std::nothrow) SliderEx();
  const auto& data = DataSet::getConfig()["pause"]["volume-control"];

  if (ret && ret->init()) {
    ret->loadBarTexture(data["slider-track"].GetString());
    ret->loadSlidBallTextures(data["silder-thumb"].GetString(),
                              data["silder-thumb"].GetString(), "");
    ret->loadProgressBarTexture(data["slider-progress"].GetString());
    ret->setTouchEnabled(true);

    ret->autorelease();

    return ret;
  }
  CC_SAFE_DELETE(ret);
  return ret;
}

void SliderEx::setRatio(float ratio) 
{
   ratio = cocos2d::clampf(ratio, 0.0f, 1.0f);

   _ratio = ratio;
   setPercent(100 * _ratio);
}

float SliderEx::getRatio() {
  _ratio = 1.0f * _percent / _maxPercent;
  return _ratio;
}