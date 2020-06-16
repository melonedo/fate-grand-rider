#include "FlowWord.h"

bool FlowWord::init() {
  m_textLab = Label::createWithSystemFont("", "Microsoft YaHei", 10,
                                          Size::ZERO, TextHAlignment::CENTER);
  m_textLab->setColor(Color3B(255, 0, 0));
  m_textLab->setAnchorPoint(Point(1, 0));
  m_textLab->setVisible(false);

  this->addChild(m_textLab);
  return true;
}

void FlowWord::showWord(int hit, Point pos) {
  char text[5];
  _itoa(hit, text, 10);
  m_textLab->setString(text);
  m_textLab->setPosition(pos);
  m_textLab->setVisible(true);

  /* 组合两个动作，放大后缩小 */
  auto scaleLarge = ScaleTo::create(0.3f, 2.5f, 2.5f);
  auto scaleSmall = ScaleTo::create(0.5f, 0.5f, 0.5f);

  auto callFunc = CallFunc::create([&]() {
    /* 动作结束，从父节点中删除自身 */
    m_textLab->setVisible(false);
    m_textLab->removeFromParentAndCleanup(true);
  });

  auto actions = Sequence::create(scaleLarge, scaleSmall, callFunc, NULL);

  m_textLab->runAction(actions);
}
