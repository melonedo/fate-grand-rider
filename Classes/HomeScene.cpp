#include "cocos2d.h"
#include "HomeScene.h"
#include "assert.h"
#include "constants.h"

using namespace cocos2d;

HomeScene* HomeScene::create(callback_t on_start_pressed,
  callback_t on_settings_pressed) {
  // 首先创建
  auto scene = HomeScene::create();
  assert(scene);

  // 首先加载背景（未实现）

  // 游戏标题
  TTFConfig ttfconfig("fonts/arial.ttf", 60);
  auto title_label = Label::createWithTTF(ttfconfig, "Fate grand rider",
                                          TextHAlignment::CENTER);
  assert(title_label);
  title_label->setAnchorPoint(Vec2(0.5f, 0.5f));
  title_label->setPosition(designResolutionSize.width / 2,
                           designResolutionSize.height * 0.8f);
  scene->addChild(title_label, 0, "title_label");

  // 开始键
  auto start_label = Label::createWithSystemFont(
      "开始", "Microsoft YaHei", 60, Size::ZERO, TextHAlignment::CENTER);
  assert(start_label);
  start_label->setAnchorPoint(Vec2(0.5f, 0.5f));
  start_label->setPosition(designResolutionSize.width / 2,
                           designResolutionSize.height * 0.6f);
  scene->addChild(start_label, 0, "start_label"); 

  // 设置键
  auto settings_label = Label::createWithSystemFont(
      "设置", "Microsoft YaHei", 60, Size::ZERO, TextHAlignment::CENTER);
  assert(settings_label);
  settings_label->setAnchorPoint(Vec2(0.5f, 0.5f));
  settings_label->setPosition(designResolutionSize.width / 2,
                              designResolutionSize.height * 0.4f);
  scene->addChild(settings_label, 0, "settings_label");

  // 为各按键设置事件
  auto listener = EventListenerMouse::create();
  listener->onMouseMove = [](EventMouse* event) {
    // 鼠标移动到标签上时放大标签
    auto target = event->getCurrentTarget();
    auto bbox = target->getBoundingBox();
    if (bbox.containsPoint(Vec2(event->getCursorX(), event->getCursorY()))) {
      target->setScale(1.1f);
    } else { // 移出时设回原比例
      target->setScale(1);
    }
  };
  listener->onMouseDown = [on_start_pressed](EventMouse* event) {
    auto bbox = event->getCurrentTarget()->getBoundingBox();
    if (bbox.containsPoint(Vec2(event->getCursorX(), event->getCursorY()))) {
      on_start_pressed();
    }
  };
  start_label->getEventDispatcher()->addEventListenerWithSceneGraphPriority(
      listener, start_label);
  
  listener = listener->clone();
  listener->onMouseDown = [on_settings_pressed](EventMouse* event) {
    auto bbox = event->getCurrentTarget()->getBoundingBox();
    if (bbox.containsPoint(Vec2(event->getCursorX(), event->getCursorY()))) {
      on_settings_pressed();
    }
  };
  settings_label->getEventDispatcher()->addEventListenerWithSceneGraphPriority(
      listener, settings_label);

  return scene;
}