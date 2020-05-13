#pragma once
#include "cocos2d.h"
#include "DataSet.h"

class GameScene : public cocos2d::Scene {
 public:
  CREATE_FUNC(GameScene);

 private:
  bool init() override;

};