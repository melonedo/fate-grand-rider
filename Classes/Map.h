#pragma once
#include "cocos2d.h"
#include "Interaction.h"

// 房间
class Room {
 public:
  Room() : _buildings(), _boundingBox(), _ref(std::make_unique<Room*>(this)) {}
  Room(const Room&) = delete;
  Room(Room&& other) noexcept;
  // 添加建筑
  void addBuilding(Interaction* building) { _buildings.emplace_back(building); }
  // 获得建筑列表
  const std::vector<Interaction*>& getBuildings() { return _buildings; }
  // 进入房间，调用所有建筑的enterRoom方法
  void enterRoom();
  // 离开房间，调用所有建筑的leaveRoom方法
  void leaveRoom();
  // 获取房间范围
  const cocos2d::Rect& getBoundingBox() const { return _boundingBox; }
  // 获取房间的指针的指针
  Room** getRef() const { return _ref.get(); }

 private:
  // 房间内所有的建筑
  std::vector<Interaction*> _buildings;
  // 房间的边界
  cocos2d::Rect _boundingBox;
  friend std::vector<Room> processMap(cocos2d::TMXTiledMap* map);
  // 指向房间的指针，由于房间在构造过程中多次转手，这里使用双层指针解决。
  std::unique_ptr<Room*> _ref;
  // 右值引用害人不浅呐，本来我直接一路unique_ptr<Room>下来就完事了。
  // 现在一路std::move转手，搞得指针变来变去。
};

// 预处理地图，包括添加碰撞箱，设定互动信息，判断房间边界
std::vector<Room> processMap(cocos2d::TMXTiledMap* map);