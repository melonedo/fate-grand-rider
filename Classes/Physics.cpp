#include "Physics.h"

#include "DataSet.h"
#include "GameScene.h"
#include "Mob.h"
#include "cocos2d.h"

namespace chipmunk {
Body& Space::addBodyAndOwn(Body&& body) {
  addBody(&body);
  _bodies.emplace_back(std::move(body));
  return _bodies.back();
}

void Space::addBody(Body* body) {
  cpSpaceAddBody(_space, body->getBody());
  cpSpaceAddShape(_space, body->getShape());
  body->setSpace(shared_from_this());
}

void Space::removeBody(Body* body) {
  cpSpaceRemoveShape(_space, body->getShape());
  cpSpaceRemoveBody(_space, body->getBody());
}

Space::~Space() { cpSpaceFree(_space); }

void Body::initAsBox(cpFloat width, cpFloat height, cpFloat radius) {
  this->clear();
  _body = cpBodyNew(0, 0);
  _shape = cpBoxShapeNew(_body, width, height, radius);
}

void Body::initAsCircle(cpFloat radius, cpVect offset) {
  this->clear();
  _body = cpBodyNew(0, 0);
  _shape = cpCircleShapeNew(_body, radius, offset);
}
Body Space::addBoxForTile(Sprite* tile, cpShapeFilter filter) {
  Body body;
  body.initAsBox(kTileResolution, kTileResolution);
  cpBodySetType(body.getBody(), cpBodyType::CP_BODY_TYPE_STATIC);
  auto bb = tile->getBoundingBox();
  // 设置位置
  cpBodySetPosition(body.getBody(), cpv(bb.getMidX(), bb.getMidY()));
  // 存储指针到shape
  cpShapeSetUserData(body.getShape(), tile);
  // 设置筛选器
  if (filter.group == 0) filter.group = ++_groupCount;
  cpShapeSetFilter(body.getShape(), filter);
  addBody(&body);

  if (DataSet::getShowPhysicsDebugBoxes()) {
    // 这里计算大小需要变换到真实大小，使用game-config的global-zoom-scale来缩放
    float scale = DataSet::getInstance()->getGlobaZoomScale();
    auto box = PhysicsBody::createBox(tile->getContentSize() * scale);
    box->setDynamic(false);
    tile->addComponent(box);
  }
  return body;
}

void initPhysicsForTile(Sprite* tile) {
  GameScene::getRunningScene()->getPhysicsSpace()->addBoxForTile(tile);
}

void Space::addCircleForMob(Mob* mob, cpShapeFilter filter) {
  float size = kSpriteResolution;
  mob->_body.initAsCircle(kSpriteResolution / 4, cpv(0, 0));
  cpBodySetType(mob->_body.getBody(), cpBodyType::CP_BODY_TYPE_KINEMATIC);
  // 存储指针在shape里
  cpShapeSetUserData(mob->_body.getShape(), mob);
  // 设置筛选器
  if (filter.group == 0) filter.group = ++_groupCount;
  cpShapeSetFilter(mob->_body.getShape(), filter);
  addBody(&mob->_body);

  if (DataSet::getShowPhysicsDebugBoxes()) {
    // 按照cocos2d-x的方法放上碰撞箱
    float scale = DataSet::getGlobaZoomScale();
    auto body = PhysicsBody::createCircle(scale * size / 4,
                                          PHYSICSBODY_MATERIAL_DEFAULT,
                                          scale * Vec2(size / 2, size / 4));
    body->setDynamic(false);
    mob->addComponent(body);
  }
}

void initPhysicsForMob(Mob* mob) {
  GameScene::getRunningScene()->getPhysicsSpace()->addCircleForMob(mob);
}

// Sprite* getSprteFromShape(const Body* body) {
//  return getSprteFromShape(body->getShape());
//}
cocos2d::Sprite* getSpriteFromShape(const cpShape* shape) {
  if (shape != nullptr) {
    return static_cast<Sprite*>(cpShapeGetUserData(shape));
  } else {
    return nullptr;
  }
}

Body::Body(Body&& other) noexcept
    : _body(other._body), _shape(other._shape), _space(other._space) {
  other._body = nullptr;
  other._shape = nullptr;
  other._space.reset();
}

Body& Body::operator=(Body&& other) noexcept {
  std::swap(_body, other._body);
  std::swap(_shape, other._shape);
  std::swap(_space, other._space);
  return *this;
}

Body::~Body() { clear(); }

void Body::clear() {
  if (_shape != nullptr) {
    if (_space) {
      cpSpaceRemoveShape(_space->getSpace(), _shape);
    }
    cpShapeFree(_shape);
    _shape = nullptr;
  }
  if (_body != nullptr) {
    if (_space) {
      cpSpaceRemoveBody(_space->getSpace(), _body);
    }
    cpBodyFree(_body);
    _body = nullptr;
  }
}

void Body::setPosition(float x, float y) {
  cpBodySetPosition(_body, cpv(x, y));
  cpSpaceReindexShape(cpShapeGetSpace(_shape), _shape);
}

Sprite* Space::queryPointNearest(const Vec2& point, float radius,
                                 cpShapeFilter filter,
                                 PointQueryInfo* out) const {
  cpPointQueryInfo cp_out;
  cpSpacePointQueryNearest(_space, cpvFromVec2(point), radius, filter, &cp_out);

  if (out != nullptr) {
    out->sprite = getSpriteFromShape(cp_out.shape);
    out->shape = cp_out.shape;
    out->point = vec2FromCpv(cp_out.point);
    out->distance = cp_out.distance;
    out->grad = vec2FromCpv(cp_out.gradient);
  }
  return getSpriteFromShape(cp_out.shape);
}

Sprite* Space::querySegmentFirst(const Vec2& start, const Vec2& end,
                                 cpShapeFilter filter, SegmentQueryInfo* out,
                                 float radius) const {
  cpSegmentQueryInfo cp_out;
  cpSpaceSegmentQueryFirst(_space, cpvFromVec2(start), cpvFromVec2(end), radius,
                           filter, &cp_out);
  if (out != nullptr) {
    out->sprite = getSpriteFromShape(cp_out.shape);
    out->shape = cp_out.shape;
    out->point = vec2FromCpv(cp_out.point);
    out->normal = vec2FromCpv(cp_out.normal);
    out->alpha = cp_out.alpha;
  }
  return getSpriteFromShape(cp_out.shape);
}

auto Space::querySegmentAll(const Vec2& start, const Vec2& end,
                            cpShapeFilter filter, float radius) const
    -> std::vector<SegmentQueryInfo> {
  typedef std::vector<SegmentQueryInfo> result_type;
  result_type result;
  cpSpaceSegmentQueryFunc query_func = [](cpShape* shape, cpVect point,
                                          cpVect normal, cpFloat alpha,
                                          void* data) {
    result_type* result = static_cast<result_type*>(data);
    result->push_back({getSpriteFromShape(shape), shape, Vec2(point.x, point.y),
                       Vec2(normal.x, normal.y), alpha});
  };
  cpSpaceSegmentQuery(_space, cpvFromVec2(start), cpvFromVec2(end), radius,
                      filter, query_func, &result);
  return result;
}

auto Space::queryPointAll(const Vec2& point, float radius,
                          cpShapeFilter filter) const
    -> std::vector<PointQueryInfo> {
  typedef std::vector<PointQueryInfo> result_type;
  result_type result;
  cpSpacePointQueryFunc query_func = [](cpShape* shape, cpVect point,
                                        cpFloat dist, cpVect grad, void* data) {
    result_type* result = static_cast<result_type*>(data);
    result->push_back({getSpriteFromShape(shape), shape, Vec2(point.x, point.y),
                       dist, Vec2(grad.x, grad.y)});
  };
  cpSpacePointQuery(_space, cpvFromVec2(point), radius, filter, query_func,
                    &result);
  return result;
}

};  // namespace chipmunk