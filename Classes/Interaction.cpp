#include "Interaction.h"
#include "cocos2d.h"
#include "Hero.h"
#include "Physics.h"
#include "constants.h"
#include "Map.h"
#include "Weapon.h"
#include "Item.h"
#include "GameScene.h"
#include "constants.h"
#include "DialogBox.h"
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

void Interaction::endInteracting(Hero* hero) {
  if (hero->_interacting == this) {
    hero->_interacting = nullptr;
  }
}

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

ItemChest* ItemChest::load(const Vec2& position, const ValueMap& property,
                   chipmunk::Body&& body) {
  auto item_chest = create();
  item_chest->_body = std::move(body);
  item_chest->_position = position;
  item_chest->_tileOffset = property.at("offset").asInt();
  return item_chest;
}

void ItemChest::touch(Hero* source) {
  auto layer = getLayerForTileComponent(this);
  layer->setTileGID(layer->getTileGIDAt(_position) + _tileOffset, _position);
}

void ItemChest::endTouch(Hero* source) {
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

// void Gate::touch(Hero*) { log("A gate"); }

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

void Gate::leaveRoom(Room*) {
  // 删掉原有的刚体
  _body.clear();
  // 设置此图块不可见
  getOwner()->setVisible(false);
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

void DroppedWeapon::touch(Hero*) {
  createDialog(getOwner(), getOwner()->getName());
}

void DroppedWeapon::endTouch(Hero*) { destroyDialog(getOwner()); }

void DroppedWeapon::dialog(Hero* hero) {
  Weapon* weapon = dynamic_cast<Weapon*>(getOwner());
  hero->pickWeapon(weapon);
  weapon->removeComponent(this);
  endInteracting(hero);
  destroyDialog(weapon);
}

void Chest::dialog(Hero*) {
  // 首先随便找一个武器
  const auto& names = DataSet::getConfig()["weapon"].GetObject();
  std::string name;
  name = names.MemberBegin()[rand() % names.MemberCount()].name.GetString();
  // 加载
  Weapon* weapon = DataSet::loadWeapon(name);
  // 放到地上
  weapon->setPosition(getOwner()->getPosition() + Vec2(0, -kTileResolution));
  weapon->addComponent(DroppedWeapon::create(weapon));
  getOwner()->getParent()->getParent()->addChild(weapon, kMapPriorityBackground);
}

DroppedItem* DroppedItem::create(Item* item) {
  DroppedItem* dropped = create();
  dropped->_body =
      GameScene::getRunningScene()->getPhysicsSpace()->addBoxForTile(item);
  setBodyTypeToItem(dropped->_body);
  return dropped;
}

void DroppedItem::touch(Hero*) {
  createDialog(getOwner(), getOwner()->getName());
}

void DroppedItem::endTouch(Hero*) { destroyDialog(getOwner()); }

void DroppedItem::dialog(Hero* hero) {
  Item* item = dynamic_cast<Item*>(getOwner());
  item->Impact(hero);
  item->setVisible(false);
  item->removeComponent(this);
  endInteracting(hero);
  destroyDialog(item);
}

void ItemChest::dialog(Hero* hero) {
  // 首先随便找一个道具
  const auto& names = DataSet::getConfig()["item"].GetObject();
  std::string name;
  name = names.MemberBegin()[rand() % names.MemberCount()].name.GetString();
  // 加载
  Item* item = DataSet::loadItem(name);
  // 放到地上
  item->setPosition(getOwner()->getPosition() + Vec2(0, -kTileResolution));
  item->addComponent(DroppedItem::create(item));
  getOwner()->getParent()->getParent()->addChild(item, kMapPriorityBackground);
}

Teleport* Teleport::load(const Vec2& position, const ValueMap& property,
                         chipmunk::Body&& body) {
  
  Teleport* teleport = create();
  teleport->_body = std::move(body);
  return teleport;
}

void Teleport::onAdd() {
  // 加载图片
  auto teleport = Sprite::create("teleport.png");
  teleport->setPosition(getOwner()->getPosition() +
                        getOwner()->getBoundingBox().size / 2);
  teleport->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
  getOwner()->getParent()->getParent()->addChild(teleport);
}

void Teleport::touch(Hero* hero) {
  GameScene::getRunningScene()->scheduleOnce(
      [this, hero] (float) {
        this->endInteracting(hero);
        GameScene::getRunningScene()->nextLevel();
      },
      0, "next-level");
}
