#include "Interaction.h"
#include "cocos2d.h"
#include "Hero.h"
#include "Physics.h"
#include "constants.h"
#include "Map.h"
#include "Weapon.h"
#include "GameScene.h"
#include "constants.h"
using namespace cocos2d;

// 获取对应的图层用来动态调整GID
static TMXLayer* getLayerForTileComponent(Component*);

static void setBodyTypeToItem(chipmunk::Body&);

void setBodyTypeToItem(chipmunk::Body& body) {
  auto filter = body.getFilter();
  filter.categories = kShapeMaskItem;
  body.setFilter(filter);
}

bool Interaction::init() {
  if (Component::init()) {
    setName("interaction");
    return true;
  } else {
    return false;
  }
}

void Interaction::endInteracting(Hero* hero) { hero->_interacting = nullptr; }

HideSpot* HideSpot::load(const cocos2d::Vec2&, const cocos2d::ValueMap&,
                         chipmunk::Body&& body) {
  setBodyTypeToItem(body);
  auto res = create();
  res->_body = std::move(body);
  return res;
}

void HideSpot::dialog(Hero* source) {
  log("Yaho");
  auto bbox = this->getOwner()->getBoundingBox();
  if (bbox.containsPoint(source->getPosition())) {
    source->setPosition(bbox.getMidX(), bbox.getMidY() - bbox.size.height);
  } else {
    source->setPosition(bbox.getMidX(), bbox.getMidY());
  }
}

static TMXLayer* getLayerForTileComponent(Component* comp) {
  auto owner = dynamic_cast<Sprite*>(comp->getOwner());
  auto layer = dynamic_cast<TMXLayer*>(owner->getBatchNode());
  return layer;
}

Chest* Chest::load(const Vec2& position, const ValueMap& property,
                   chipmunk::Body&& body) {
  auto chest = create();
  chest->_body = std::move(body);
  chest->_position = position;
  chest->_tileOffset = property.at("offset").asInt();
  return chest;
}

void Chest::touch(Hero* source) {
  auto layer = getLayerForTileComponent(this);
  layer->setTileGID(layer->getTileGIDAt(_position) + _tileOffset, _position);
  // 如需给英雄武器，用source->pickWeapon()
}

void Chest::endTouch(Hero* source) {
  auto layer = getLayerForTileComponent(this);
  layer->setTileGID(layer->getTileGIDAt(_position) - _tileOffset, _position);
}

Gate* Gate::load(const Vec2& position, const ValueMap& property,
                 chipmunk::Body&& body) {
  setBodyTypeToItem(body);
  auto res = create();
  res->_body = std::move(body);
  res->_position = position;
  res->_tileOffset = property.at("offset").asInt();
  res->_isClosed = false;
  return res;
}

void Gate::touch(Hero*) { log("A gate"); }

void Gate::endTouch(Hero* hero) {
  if (!_isClosed && (*_room)->getBoundingBox().containsPoint(hero->getPosition()))
    (*_room)->enterRoom();
}

void Gate::linkRoom(Room** room) {
  assert(room && "One gate shoud not belong to more than one room.");
  _room = room;
}

void Gate::enterRoom(Room*) {
  auto filter = _body.getFilter();
  filter.categories = kShapeMaskTile;
  _body.setFilter(filter);
  auto layer = getLayerForTileComponent(this);
  layer->setTileGID(layer->getTileGIDAt(_position) + _tileOffset, _position);
  _isClosed = true;
}

NoInteraction* NoInteraction::load(const cocos2d::Vec2&,
                                          const cocos2d::ValueMap&,
                                          chipmunk::Body&& body) {
  auto res = create();
  res->_body = std::move(body);
  return res;
}

void Target::attack(cocos2d::Sprite*, float) {
  getOwner()->setScale(1.5);
  getOwner()->runAction(ScaleTo::create(0.1, 1));
}

Target* Target::load(const cocos2d::Vec2& position,
                     const cocos2d::ValueMap& property, chipmunk::Body&& body) {
  auto res = create();
  res->_body = std::move(body);
  return res;
}

DroppedWeapon* DroppedWeapon::create(Weapon* weapon) {
  DroppedWeapon* dropped = create();
  dropped->_body =
      GameScene::getRunningScene()->getPhysicsSpace()->addBoxForTile(weapon);
  setBodyTypeToItem(dropped->_body);
  return dropped;
}

void DroppedWeapon::touch(Hero*) { log("%s", getOwner()->getName().c_str()); }

void DroppedWeapon::dialog(Hero* hero) {
  Weapon* weapon = dynamic_cast<Weapon*>(getOwner());
  hero->pickWeapon(weapon);
  weapon->removeComponent(this);
  endInteracting(hero);
}

void Chest::dialog(Hero*) {
  // 首先随便找一个武器
  const auto& names = DataSet::getConfig()["weapon"].GetObject();
  std::string name;
  name = names.MemberBegin()[rand() % names.MemberCount()].name.GetString();
  // 加载
  Weapon* weapon = DataSet::load_weapon(name);
  // 放到地上
  weapon->setPosition(getOwner()->getPosition() + Vec2(0, -kTileResolution));
  weapon->addComponent(DroppedWeapon::create(weapon));
  GameScene::getRunningScene()->addChild(weapon, kMapPriorityBackground);
}