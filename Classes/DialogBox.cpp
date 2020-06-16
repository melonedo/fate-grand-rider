#include "DialogBox.h"
#include "constants.h"

using namespace cocos2d;

Dialog* getDialog(Node* node) {
  return dynamic_cast<Dialog*>(node->getChildByName("dialog"));
}

Dialog* createDialog(Node* node, const std::string& msg) {
  if (node->getChildByName("dialog")) return nullptr;
  Dialog* dialog = Label::createWithSystemFont(
      msg, "Microsoft YaHei", 20, Size::ZERO, TextHAlignment::CENTER,
      TextVAlignment::CENTER);
  dialog->setGlobalZOrder(kMapPriorityUI);
  dialog->setName("dialog");
  // 直接显示文字会糊掉，先设大字体再缩放成小的
  dialog->setScale(.5);
  node->addChild(dialog);
  dialog->setPositionNormalized(Vec2::ANCHOR_MIDDLE_TOP);
}

void destroyDialog(Node* node) { node->removeChildByName("dialog"); }

