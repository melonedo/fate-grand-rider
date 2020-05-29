#pragma once

#include "Pause.h"
using namespace cocos2d;

Scene* Pause::createScene() { return Pause::create(); }

int Pause::_audioID = AudioEngine::INVALID_AUDIO_ID;
bool Pause::_loopEnabled = true;
float Pause::_volume = 1.0f;

bool Pause::init() {
  if (!Scene::init()) {
    return false;
  }

  auto visibleSize = Director::getInstance()->getVisibleSize();
  Vec2 origin = Director::getInstance()->getVisibleOrigin();
  auto& layerSize = this->getContentSize();

  /*auto sprite = Sprite::create("HelloWorld.png");
  sprite->setPosition(Vec2(origin.x+visibleSize.width/2,
  origin.y+visibleSize.height/2)); this->addChild(sprite);*/
  auto label = Label::createWithTTF("volume", "fonts/Marker Felt.ttf", 24);
  label->setPosition(Vec2(origin.x + visibleSize.width / 8,
                          origin.y + visibleSize.height / 2));
  this->addChild(label);

  auto volumeSlider = SliderEx::create();
  volumeSlider->setPercent(100);
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
