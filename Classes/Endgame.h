#pragma once
#include "cocos2d.h"

// 非常普通的结束界面，显示“你死了”和“恭喜通关”
class EndScene : public cocos2d::Scene {
 public:
  CREATE_FUNC(EndScene);

  // 创建结束界面，success为true时表示通关，为false表示死亡。
  static EndScene* create(bool success);
};