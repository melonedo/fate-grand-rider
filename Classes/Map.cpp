#include "Map.h"
#include "cocos2d.h"
#include "constants.h"
#include "Physics.h"
#include "DataSet.h"
#include "sstream"
#include "GameScene.h"
using namespace cocos2d;
using namespace chipmunk;

typedef std::map<Vec2, Interaction*> tile_set_t;

//struct HashVec2 {
//  size_t operator()(const Vec2& key) const {
//    return std::hash<int>()(key.y * 10000 + key.x);
//  }
//};

// 处理房间信息。内部函数，请勿调用。
Room processRoom(const tile_set_t& tiles,
                 const Vec2& room_pos, std::array<int, 4>& bound);

std::vector<std::string> splitString(const std::string& str) {
  typedef std::istream_iterator<std::string> iter_t;
  std::stringstream str_stream(str);
  std::vector<std::string> res{iter_t(str_stream), iter_t()};
  return res;
}

std::vector<Room> processMap(cocos2d::TMXTiledMap* map) {
  // 断言
  CCASSERT(map, "Failed to load tiled map");
  map->getLayer("invisible")->setVisible(false);

  // 设置前景和背景的优先级
  std::vector<std::string> layer_names;
  try {
    auto fg_names = splitString(map->getProperty("fg-layers").asString());
    layer_names.insert(layer_names.cend(), fg_names.cbegin(), fg_names.cend());
    for (auto&& name : fg_names) {
      auto layer = map->getLayer(name);
      if (layer == nullptr) throw name;
      layer->setGlobalZOrder(kMapPriorityForeground);
    }

    auto bg_names = splitString(map->getProperty("bg-layers").asString());
    layer_names.insert(layer_names.cend(), bg_names.cbegin(), bg_names.cend());
    for (auto&& name : bg_names) {
      auto layer = map->getLayer(name);
      if (layer == nullptr) throw name;
      layer->setGlobalZOrder(kMapPriorityBackground);
    }
  } catch (const std::string& name) {
    log("Layer \"%s\" is listed in map property but does not exist.",
        name.c_str());
    assert(0 &&
           "One layer is listed in map property but does not exist, check "
           "output.");
  }

  // 遍历地图中所有的图块，添加对应的互动
  auto space = GameScene::getRunningScene()->getPhysicsSpace();
  // 记录房间中的关键信息，虽然Vec2是float，但是值很小，可以精确对应整数。
  tile_set_t tile_set;
  std::vector<Vec2> room_set;
  for (auto&& name : layer_names) {
    auto layer = map->getLayer(name);
    CCASSERT(layer, "Layer listed in the map property is not found!");

    auto layer_size = layer->getLayerSize();
    auto map_size = map->getMapSize();
    CCASSERT(map_size.width == layer_size.width &&
                 map_size.height == layer_size.height,
             "Size of map and eachlayer must be the same.");
    for (int x = 0; x < map_size.width; x++) {
      for (int y = 0; y < map_size.height; y++) {
        Vec2 pos(x, y);
        auto prop = map->getPropertiesForGID(layer->getTileGIDAt(pos));
        if (prop.isNull()) continue;

        auto value_map = prop.asValueMap();
        auto tile = layer->getTileAt(pos);
        auto&& body = space->addBoxForTile(tile);
        auto type = value_map.at("type").asString();
        if (type != "room") {
          auto interaction = DataSet::load_interaction(
              type, Vec2(x, y), value_map, std::move(body));
          tile->addComponent(interaction);
          tile_set.emplace(pos, interaction);
        } else {
          room_set.emplace_back(pos);
        }
      }
    }
  }

  // 生成房间
  std::vector<Room> rooms;
  for (auto&& room_pos : room_set) {
    std::array<int, 4> bound;
    rooms.emplace_back(std::move(processRoom(tile_set, room_pos, bound)));
    auto&& room = rooms.back();
    // 转换坐标
    Rect bbox;
    int height = map->getMapSize().height;
    bbox.origin.x = bound[0];
    bbox.origin.y = height - bound[3];
    bbox.size.width = bound[1] - bound[0] + 1;
    bbox.size.height = bound[3] - bound[2] + 1;
    room._boundingBox.size = bbox.size * kTileResolution;
    room._boundingBox.origin = bbox.origin * kTileResolution;
  }
  return rooms;
}

Room processRoom(const tile_set_t& tiles, const Vec2& room_pos,
                 std::array<int, 4>& bound) {
  // 宽搜寻找房间边界，同时如果边界及内部是门则记录下来。
  // 复杂度O(n2*log n)
  Room room;
  // 已访问的瓦片
  std::set<Vec2> visited;
  // 未访问的空地，建筑不会加入
  std::vector<Vec2> to_visit{room_pos};
  // 四个方向
  int left, right, up, down;
  left = right = room_pos.x;
  up = down = room_pos.y;

  while (to_visit.size() != 0) {
    Vec2 cur = to_visit.back();
    to_visit.pop_back();

    auto res = visited.insert(cur);
    if (!res.second) continue;

    auto tile = tiles.find(cur);
    if (tile != tiles.end()) {
      // 建筑
      room.addBuilding(tile->second);
      tile->second->linkRoom(room.getRef());
    } else {
      // 空地
      // 记录边界范围
      if (left > cur.x) left = cur.x;
      if (right < cur.x) right = cur.x;
      if (down > cur.y) down = cur.y;
      if (up < cur.y) up = cur.y;

      // 探索四周
      // 四个方向，或许为了优化可以设成static?
      const std::vector<Vec2> directions{{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
      for (const Vec2& diff : directions) {
        Vec2 next = cur + diff;
        to_visit.emplace_back(next);
      }
    }
  }
  // 记录边界
  bound[0] = left;
  bound[1] = right;
  bound[2] = down;
  bound[3] = up;

  return room;
}

Room::Room(Room&& other) noexcept
    : _buildings(std::move(other._buildings)),
      _boundingBox(std::move(other._boundingBox)),
      _ref(std::move(other._ref)) {
  *_ref = this;
}

void Room::enterRoom() {
  for (auto i : _buildings) i->enterRoom(this);
}

void Room::leaveRoom() {
  for (auto i : _buildings) i->leaveRoom(this);
}
