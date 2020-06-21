#include "Endgame.h"
#include "HomeScene.h"
using namespace cocos2d;

EndScene* EndScene::create(bool success) {
  auto scene = create();
  // 放入正中大标签
  auto word = success ? "恭喜通关" : "你死了";
  auto label = Label::createWithSystemFont(word, "Microsoft Ya Hei", 70,
                                           Size ::ZERO, TextHAlignment::CENTER,
                                           TextVAlignment::CENTER);
  label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
  scene->addChild(label);
  label->setPositionNormalized(Vec2(.5, .6));
  if (!success) {
    label->setColor(Color3B::RED);
  } else {
    label->setColor(Color3B::WHITE);
  }
  // 返回开始菜单
  auto button = Label::createWithSystemFont("重新开始", "Microsoft Ya Hei", 40,
                                            Size ::ZERO, TextHAlignment::CENTER,
                                            TextVAlignment::CENTER);
  button->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
  scene->addChild(button);
  button->setPositionNormalized(Vec2(.5, .4));
  // 按键互动，参见HomeScene.cpp
  auto listener = EventListenerMouse::create();
  listener->onMouseMove = [](EventMouse* event) {
    auto target = event->getCurrentTarget();
    auto bbox = target->getBoundingBox();
    if (bbox.containsPoint(Vec2(event->getCursorX(), event->getCursorY()))) {
      target->setScale(1.1f);
    } else {
      target->setScale(1);
    }
  };
  listener->onMouseDown = [](EventMouse* event) {
    auto bbox = event->getCurrentTarget()->getBoundingBox();
    if (bbox.containsPoint(Vec2(event->getCursorX(), event->getCursorY()))) {
      Director::getInstance()->replaceScene(HomeScene::createScene());
    }
  };
  button->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,
                                                                       button);
  return scene;
}
