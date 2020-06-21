#pragma once
#include <vector>

#include "chipmunk/chipmunk.h"
#include "cocos2d.h"
#include "constants.h"

// 若#include "Mob.h"则产生循环包含，故直接声明
class Mob;

// cocos2d-x自动的物理引擎有一些非常奇怪的特性，且接口不多，非常不方便，
// 这里直接把chipmunk包装上，我们要用的只是查询功能，比较简单。

namespace chipmunk {
using cocos2d::Vec2;
class Body;
// 整个空间，包含所有要查询的物体。
class Space : public std::enable_shared_from_this<Space> {
 public:
  Space() : _space(cpSpaceNew()), _bodies(), _groupCount(0) {}
  Space(const Space&) = delete;
  ~Space();
  // 获取cpSpace
  cpSpace* getSpace() { return _space; }

  //// 管理空间中的刚体

  // 添加一个body，并获得所有权，管理对应内存。返回一个引用，注意在之后这个引用对应的指针可能会失效。
  Body& addBodyAndOwn(Body&& body);
  // 添加一个body，但不管理对应内存。
  void addBody(Body* body);
  // 删除一个body，不管理对应内存。
  void removeBody(Body* body);
  // 将精灵对应为空间中静态的方形，并把指针写入cpShape中，若filter的第一个参数是0,则分配一个独立的组。
  Body addBoxForTile(cocos2d::Sprite* tile,
                     cpShapeFilter filter = {0, kShapeMaskTile,
                                             CP_ALL_CATEGORIES});
  // 将精灵下部对应的圆形加入空间，并把指针写入cpShape中，若filter的第一个参数是0,则分配一个独立的组。
  // 注意为了保证位置对应关系，应将锚点设定为(0.5,0.25)。
  void addCircleForMob(Mob* mob, cpShapeFilter filter = {0, kShapeMaskMob,
                                                         kShapeMaskForMob});
  // 如果以其他的形状添加，请自行使用chipmunk的接口。

  //// 查询
  // 提供几个常用的，若需要更多可自行使用chipmunk的函数。
  // 关于cpShapeFilter：有分组和类别两个部分，同组内不会检测
  // 分组则是交叉检测，A与B碰，A的分组和B的碰撞分组做与，B的分组和B的碰撞分组做与，
  // 要求两个都是非0

  struct PointQueryInfo {
    cocos2d::Sprite* sprite;  // 查询到的对象
    const cpShape* shape;     // 该对象的刚体
    Vec2 point;               // 最近点
    float distance;           // 距离
    Vec2 grad;                // 方向向量
  };

  // 查询圆形范围内的所有精灵
  std::vector<PointQueryInfo> queryPointAll(
      const Vec2& point, float radius,
      cpShapeFilter filter = CP_SHAPE_FILTER_ALL) const;
  // 查询一点圆形范围内最近的精灵
  cocos2d::Sprite* queryPointNearest(const Vec2& point, float radius,
                                     cpShapeFilter filter = CP_SHAPE_FILTER_ALL,
                                     PointQueryInfo* out = nullptr) const;

  struct SegmentQueryInfo {
    cocos2d::Sprite* sprite;  // 查询到的对象
    const cpShape* shape;     // 该对象的刚体
    Vec2 point;               // 交点
    Vec2 normal;              // 对应平面在交点点处的法线
    float alpha;  // 归一化长度，即到起点的路程占总长度的比例，在区间[0,1]
  };

  // 查询一射线段上最近的精灵（注意结果可能会非常诡异）
  cocos2d::Sprite* querySegmentFirst(const Vec2& start, const Vec2& end,
                                     cpShapeFilter = CP_SHAPE_FILTER_ALL,
                                     SegmentQueryInfo* out = nullptr,
                                     float radius = 1) const;
  // 查询一线段上所有精灵（注意当线段较粗时不准确）
  std::vector<SegmentQueryInfo> querySegmentAll(
      const Vec2& start, const Vec2& end,
      cpShapeFilter filter = CP_SHAPE_FILTER_ALL, float radius = 1) const;

 private:
  cpSpace* _space;
  // 空间主动管理的刚体列表
  std::vector<Body> _bodies;
  // 现在这里已经没有东西了，所有的刚体都在Interaction或者Mob里面管理。

  // 组的计数，用来给每个刚体分配一个单独的组。
  mutable int _groupCount;
};

// 物理刚体，各自对应一个形状。
class Body {
 public:
  Body() : _body(nullptr), _shape(nullptr) {}
  Body(const Body&) = delete;
  Body(Body&&) noexcept;
  Body& operator=(Body&& other) noexcept;
  ~Body();
  // 初始化为圆形，offset为相对物体位置的偏移，通常为0。
  void initAsCircle(cpFloat radius, cpVect offset = cpv(0, 0));
  // 初始化为方形，radius为边框的粗细。
  void initAsBox(cpFloat width, cpFloat height, cpFloat radius = 0);

  // 清除原有的物理刚体和形状
  void clear();

  // 获取cpBody，供chipmunk引擎使用。
  cpBody* getBody() { return _body; }
  // 获取cpShape，供chipmunk引擎使用。
  cpShape* getShape() { return _shape; }

  // 设置位置，注意只有动态的物体可以改变位置（生物默认都可以）
  void setPosition(Vec2 pos) { setPosition(pos.x, pos.y); }
  // 设置位置，注意只有动态的物体可以改变位置（生物默认都可以）
  void setPosition(float x, float y);
  // 似乎静态的也可以用?

  // 获取筛选器
  cpShapeFilter getFilter() const { return cpShapeGetFilter(_shape); }
  // 设置筛选器
  void setFilter(cpShapeFilter filter) const {
    cpShapeSetFilter(_shape, filter);
  }

  // 设置物理空间
  void setSpace(const std::shared_ptr<Space>& space) { _space = space; }
  // 获取物理空间
  const std::shared_ptr<Space>& getSpace() const { return _space; }

 private:
  cpBody* _body;
  cpShape* _shape;
  std::shared_ptr<Space> _space;
  // 由于chipmunk引擎中cpSpace在销毁的时候会遍历地解除所有的变量与空间的联系，必须保证空间在
  // 所有的刚体之后销毁，用shared_ptr保证。
};

// 为地图中的瓦片生成对应碰撞箱。
// 为兼容性暂时保留。
void initPhysicsForTile(cocos2d::Sprite*);

// 为生物生成碰撞箱。
// 为兼容性暂时保留。
void initPhysicsForMob(Mob*);

// 从cpShape获得对应的精灵，加入到空间的时候把指针存到了cpShape里。
cocos2d::Sprite* getSpriteFromShape(const cpShape* shape);

// 把cocos2d的Vec2转换为cpVect。
inline cpVect cpvFromVec2(const Vec2& vec) { return cpv(vec.x, vec.y); }

// 把cpVect转成cocos2d::Vec2
inline Vec2 vec2FromCpv(cpVect vec) { return Vec2(vec.x, vec.y); }

};  // namespace chipmunk
