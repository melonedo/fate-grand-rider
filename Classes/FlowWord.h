#pragma once
#include "cocos2d.h"
USING_NS_CC;

class FlowWord : public Node {
 public:
  CREATE_FUNC(FlowWord);
  virtual bool init();

 public:
  void showWord(int hit, Point pos);

 private:
  Label* m_textLab;
};
