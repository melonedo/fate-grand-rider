#pragma once
#include "Physics.h"
#include "cocos2d.h"
class Hero;
class Room;

// 所有物品间的互动都通过Interaction来实现。交互时，先用getInteraction找到名为"interaction"的组件，
// 然后调用对应的接口函数。
class Interaction : public cocos2d::Component {
 public:
  // 几个事件不需要全部实现，如果不响应对应的事件就直接留空即可。
  // 每个事件都应该传递对应的触发者，即第一个source参数。

  // 人物靠近时触发
  virtual void touch(Hero* source) {}
  // 人物离开时触发
  virtual void endTouch(Hero* source) {}
  // 靠近并空格时触发
  virtual void dialog(Hero* source) {}
  // 攻击时触发，适用于会被破坏的建筑和生物。source应该是Weapon*，这里暂时不强制要求。
  virtual void attack(cocos2d::Sprite* source, float damage) {}

  // 从地图中加载的互动对应的工厂函数都应为以下形式
  //（其中position为格子的位置，property为该格子对应的属性列表，body是对应的刚体，有需要可以调用）
  // static Interaction* load(const cocos2d::Vec2& position,
  //                          const cocos2d::ValueMap& property
  //                          chipmunk::Body&& body);
  // 从地图中加载出来后，与房间想关联需要以下方法：
  // 连接到对应的房间,通常只有门需要实现
  virtual void linkRoom(Room**) {}
  // 进入房间时调用
  virtual void enterRoom(Room*) {}
  // 离开房间（结束关卡）时调用
  virtual void leaveRoom(Room*) {}

  // 强制结束互动，防止出现dangling
  // pointer。注意删除互动的时候一定要调用这个函数，否则立刻报错。
  void endInteracting(Hero*);

 protected:
  bool init() override;
};
// 关于析构，需要注意：
// 1.英雄可能会有残存的指针，需要析构Interaction的方法必须手动调用 endInteracting清除。
// 2.touch中有局部变量指向当前的互动，不能立即析构当前互动，可以把对应的函数用scheduleOnce包装。

// 获取对应的互动组件。
// cocos2d-x引擎中，根本没有组件的对象连组件容器都是空指针，直接就空指针报错了，无法检查。
inline Interaction* getInteraction(cocos2d::Node* node) {
  return dynamic_cast<Interaction*>(node->getComponent("interaction"));
}

// 没有任何互动，对应墙
class NoInteraction : public Interaction {
 public:
  static NoInteraction* load(const cocos2d::Vec2&, const cocos2d::ValueMap&,
                             chipmunk::Body&& body);

 private:
  CREATE_FUNC(NoInteraction);
  chipmunk::Body _body;
};

// 示例互动：躲藏。互动可以进去，再互动会出来。
class HideSpot : public Interaction {
 public:
  static HideSpot* load(const cocos2d::Vec2&, const cocos2d::ValueMap&,
                        chipmunk::Body&& body);

  void touch(Hero*) override { cocos2d::log("You can hide here."); }
  void endTouch(Hero*) override { cocos2d::log("Bye"); }
  void dialog(Hero*) override;

 private:
  CREATE_FUNC(HideSpot);
  chipmunk::Body _body;
};

// 宝箱，靠近时打开，空格可获得
class Chest : public Interaction {
 public:
  static Chest* load(const cocos2d::Vec2& position,
                     const cocos2d::ValueMap& property, chipmunk::Body&&);

  void touch(Hero*) override;
  void endTouch(Hero*) override;
  // 放一个武器在地上
  void dialog(Hero*) override;

 private:
  CREATE_FUNC(Chest);
  chipmunk::Body _body;
  // 开闭之间的GID差
  int _tileOffset;
  // 地图上的坐标
  cocos2d::Vec2 _position;
};

//道具宝箱
class ItemChest : public Interaction {
 public:
  static ItemChest* load(const cocos2d::Vec2& position,
                     const cocos2d::ValueMap& property, chipmunk::Body&&);

  void touch(Hero*) override;
  void endTouch(Hero*) override;
  // 放一个道具在地上
  void dialog(Hero*) override;

 private:
  CREATE_FUNC(ItemChest);
  chipmunk::Body _body;
  // 开闭之间的GID差
  int _tileOffset;
  // 地图上的坐标
  cocos2d::Vec2 _position;
};

class Gate : public Interaction {
 public:
  static Gate* load(const cocos2d::Vec2& position,
                    const cocos2d::ValueMap& property, chipmunk::Body&&);
  // void touch(Hero*) override;
  // 触发进入房间的事件
  void endTouch(Hero*) override;
  // 记录所在的房间
  void linkRoom(Room**) override;
  // 关门
  void enterRoom(Room*) override;
  // 开门
  void leaveRoom(Room*) override;

 private:
  CREATE_FUNC(Gate);
  // 物理刚体
  chipmunk::Body _body;
  // 开闭之间的GID差
  int _tileOffset;
  // 地图上的坐标
  cocos2d::Vec2 _position;
  // 门已关上
  bool _isClosed;
  // 对应房间
  Room** _room;
};

// 靶子
class Target : public Interaction {
 public:
  static Target* load(const cocos2d::Vec2& position,
                      const cocos2d::ValueMap& property, chipmunk::Body&&);
  void attack(cocos2d::Sprite*, float) override;

 private:
  CREATE_FUNC(Target);
  chipmunk::Body _body;
};

class Weapon;
// 地上的武器，可穿越，碰到时显示名字，对话时给予。
class DroppedWeapon : public Interaction {
 public:
  // 展示武器名
  void touch(Hero*) override;
  // 关掉对话框
  void endTouch(Hero*) override;
  // 捡起武器
  void dialog(Hero*) override;
  // 生成对应于地上的武器所需的交互。
  static DroppedWeapon* create(Weapon* weapon);

 private:
  CREATE_FUNC(DroppedWeapon);

  chipmunk::Body _body;
};

class Item;

class DroppedItem:public Interaction {
 public:
  // 展示道具名
  void touch(Hero*) override;
  // 关掉对话框
  void endTouch(Hero*) override;
  // 捡起道具
  void dialog(Hero*) override;
  // 生成对应于地上的武器所需的交互。
  static DroppedItem* create(Item* item);

 private:
  CREATE_FUNC(DroppedItem);

  chipmunk::Body _body;
};

// 传送门
class Teleport : public Interaction {
 public:
  static Teleport* load(const cocos2d::Vec2& position,
                        const cocos2d::ValueMap& property, chipmunk::Body&&);
  void onAdd() override;
  void touch(Hero*) override;
  CREATE_FUNC(Teleport);

 private:
  chipmunk::Body _body;
};