#pragma once
#include "chipmunk/chipmunk.h"
#include "cocos2d.h"
#include <vector>
#include "constants.h"

// 若#include "Mob.h"则产生循环包含，故直接声明
class Mob;

// cocos2d-x自动的物理引擎有一些非常奇怪的特性，且接口不多，非常不方便，
// 这里直接把chipmunk包装上，我们要用的只是查询功能，比较简单
namespace chipmunk {
class Body;
// 整个空间，包含所有要查询的物体
class Space {
 public:
  Space() : _space(cpSpaceNew()), _bodies() {}
  Space(const Space&) = delete;
  ~Space();
  // 获取cpSpace
  cpSpace* getSpace() const { return _space; }
  // 添加一个body，并获得所有权，管理对应内存
  void addBodyAndOwn(Body&& body);
  // 添加一个body，但不管理对应内存
  void addBody(Body* body);
  // 删除一个body，必须是不由space管理的，否则是运行错误
  void removeBody(Body* body);
 private:
  cpSpace* _space;
  // 往Sprite里面注入有关的管理代码不太现实，所以在外部进行管理
  std::vector<Body> _bodies;
};

// 这里每个shape对应一个body，不单独分离出shape了
class Body {
 public:
  Body() : _body(nullptr), _shape(nullptr) {}
  Body(const Body&) = delete;
  Body(Body&& other) noexcept : _body(other._body), _shape(other._shape) {
    other._body = nullptr;
    other._shape = nullptr;
  }
  Body& operator=(Body&& other) noexcept {
    std::swap(_body, other._body);
    std::swap(_shape, other._shape);
    return *this;
  }
  static Body createCircle(cpFloat radius, cpVect offset);
  static Body createBox(cpFloat width, cpFloat height, cpFloat radius = 1);
  cpBody* getBody() { return _body; }
  cpShape* getShape() { return _shape; }

  ~Body() {
    if (_body != nullptr) cpBodyFree(_body);
    if (_shape != nullptr) cpShapeFree(_shape);
  }
 private:
  cpBody* _body;
  cpShape* _shape;
};

// 为地图生成对应碰撞箱
void initPhysicsForMap(cocos2d::TMXTiledMap*);

// 为生物生成碰撞箱
void initPhysicsForMob(Mob*);

// 从cpShape获得对应的精灵，initPhysics的时候把指针存到了shape里。
//Sprite* getSpriteFromShape(const Body*);
cocos2d::Sprite* getSpriteFromShape(const cpShape* shape);

};  // namespace chipmunk
