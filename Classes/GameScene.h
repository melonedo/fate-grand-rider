#pragma once
#include "cocos2d.h"
#include "DataSet.h"
#include "Physics.h"

class GameScene : public cocos2d::Scene {
 public:
  CREATE_FUNC(GameScene);

  ~GameScene();

  // 获取当前的场景
  static GameScene* getRunningScene() { return runningGameScene; }
  
  // 获取物理空间
  chipmunk::Space* getPhysicsSpace() { return &_space; }
 private:
  bool init() override;
  
  chipmunk::Space _space;

  // 当前运行的场景，否则获取的时候要dynamic_cast一遍
  static GameScene* runningGameScene;
};

