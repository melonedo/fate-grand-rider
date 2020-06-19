#pragma once
#include "cocos2d.h"
#include "DataSet.h"
#include "Physics.h"
#include "Map.h"
#include "PauseGame.h"

// 静止节点，会自动同步位置和缩放比例，保持相对窗口不动
// Hero::update中每次都会调整位置。
class StaticNode : public cocos2d::Node {
 public:
  CREATE_FUNC(StaticNode);
  const cocos2d::Size& getVisibleSize() const;


 protected:
  bool init() override;
  cocos2d::Size _visibleSize;
};

class GameScene : public cocos2d::Scene {
 public:
  CREATE_FUNC(GameScene);

  ~GameScene();

  // 获取当前的场景
  static GameScene* getRunningScene() { return runningGameScene; }

  StaticNode* getStaticNode() { return _node; }

  // 获取物理空间
  chipmunk::Space* getPhysicsSpace() { return _space.get(); }
 private:
  bool init() override;

  std::shared_ptr<chipmunk::Space> _space;

  std::vector<Room> _rooms;

  // 当前运行的场景，否则获取的时候要dynamic_cast一遍
  static GameScene* runningGameScene;

  StaticNode* _node;

   void update(float);
};
