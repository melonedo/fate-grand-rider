#pragma once
#include "cocos2d.h"

using namespace cocos2d;

// 打开游戏第一个看见的界面
class HomeScene : public Scene {
 public:
  typedef std::function<void()> callback_t;
  static HomeScene* create(callback_t on_start_pressed,
                           callback_t on_settings_pressed);
  // 省得还得找对对应的参数去create了
  static HomeScene* createScene();
 private:
  CREATE_FUNC(HomeScene);
};