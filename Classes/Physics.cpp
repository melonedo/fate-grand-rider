#include "Physics.h"
#include "cocos2d.h"
#include "DataSet.h"
#include "GameScene.h"
#include "Mob.h"

namespace chipmunk {
void Space::addBodyAndOwn(Body&& body) {
  addBody(&body);
  _bodies.emplace_back(std::move(body));
}

void Space::addBody(Body* body) {
  cpSpaceAddBody(_space, body->getBody());
  cpSpaceAddShape(_space, body->getShape());
}

void Space::removeBody(Body* body) {
  cpSpaceRemoveShape(_space, body->getShape());
  cpSpaceRemoveBody(_space, body->getBody());
}

Space::~Space() {
  cpSpaceFree(_space);
}

Body Body::createBox(cpFloat width, cpFloat height,
                                      cpFloat radius) {
  Body body;
  body._body = cpBodyNew(0, 0);
  body._shape = cpBoxShapeNew(body._body, width, height, radius);
  return body;
}

Body Body::createCircle(cpFloat radius, cpVect offset) {
  Body body;
  body._body = cpBodyNew(0, 0);
  body._shape = cpCircleShapeNew(body._body, radius, offset);
  return body;
}

static void initPhysicsForTile(cocos2d::Sprite*);
void initPhysicsForMap(cocos2d::TMXTiledMap* map) {
  using namespace cocos2d;
  // 给所有type写了wall的方块加上碰撞箱
  auto meta_layer = map->getLayer("meta");
  auto layer_size = meta_layer->getLayerSize();
  auto map_size = map->getMapSize();
  CCASSERT(map_size.width == layer_size.width &&
               map_size.height == layer_size.height,
           "Size of map and meta layer must be the same.");
  for (int x = 0; x < map_size.width; x++) {
    for (int y = 0; y < map_size.height; y++) {
      Vec2 pos(x, y);
      auto prop = map->getPropertiesForGID(meta_layer->getTileGIDAt(pos));
      if (!prop.isNull() && prop.asValueMap().at("type").asString() == "wall") {
        initPhysicsForTile(meta_layer->getTileAt(pos));
      }
    }
  }
}

static void initPhysicsForTile(Sprite* tile) {
  Body body = Body::createBox(kTileResolution, kTileResolution);
  cpBodySetType(body.getBody(), cpBodyType::CP_BODY_TYPE_STATIC);
  auto bb = tile->getBoundingBox();
  cpBodySetPosition(body.getBody(), cpv(bb.getMidX(), bb.getMidY()));
  // 存储指针到shape
  cpShapeSetUserData(body.getShape(), tile);
  GameScene::getRunningScene()->getPhysicsSpace()->addBodyAndOwn(std::move(body));

  if (DataSet::getShowPhysicsDebugBoxes()) {
    // 这里计算大小需要变换到真实大小，使用game-config的global-zoom-scale来缩放
    float scale = DataSet::getInstance()->getGlobaZoomScale();
    auto box = PhysicsBody::createBox(tile->getContentSize() * scale);
    box->setDynamic(false);
    tile->addComponent(box);
  }
}

void initPhysicsForMob(Mob* mob) {
  float size = kSpriteResolution;
  mob->_body = Body::createCircle(kSpriteResolution / 4, cpv(0, 0));
  cpBodySetType(mob->_body.getBody(), cpBodyType::CP_BODY_TYPE_KINEMATIC);
  // 存储指针在shape里
  cpShapeSetUserData(mob->_body.getShape(), mob);
  GameScene::getRunningScene()->getPhysicsSpace()->addBody(&mob->_body);

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

//Sprite* getSprteFromShape(const Body* body) {
//  return getSprteFromShape(body->getShape());
//}
cocos2d::Sprite* getSpriteFromShape(const cpShape* shape) {
  return static_cast<Sprite*>(cpShapeGetUserData(shape));
}


};