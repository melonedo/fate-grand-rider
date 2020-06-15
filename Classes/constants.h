#pragma once
// 定义全局的常量
#include "cocos2d.h"

// 建议都设置为const static以避免产生重复定义

const static cocos2d::Size designResolutionSize = cocos2d::Size(1024, 768);
const static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
const static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
const static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

enum MapPriority {
  kMapPriorityBackground = -300,
  kMapPrioritySprite = -200,
  kMapPriorityForeground = -100,
  kMapPriorityUI = 0
};

const static int kSpriteResolution = 32, kTileResolution = 16, kWeaponResolution = 34;

enum AnimationTag{ 
  kTagStandAnimation = 1000, 
  kTagWalkingAnimation };

enum SpriteTag { kTagHero = 2000, kTagWeapon, kTagWall, kTagInteractable};

enum UI { kUserInterfaceBackground = 1000, kBars, kProgress,kHealthPoint };

enum UITag { kTagHealth, kTagShield, kTagMagic };

// 用cpBitMask会报错C3064: must be a simple type or resolve to one，只得直接写下unsigned int
enum PhysicsShapeMask : unsigned int {
  // kShapeMaskPlayer = 1 << 1,
  kShapeMaskMob = 1 << 2,   // 生物
  kShapeMaskTile = 1 << 3,  // 建筑
  // kShapeMaskBullet = 1 << 4, // 子弹
  kShapeMaskItem = 1 << 5,  // 同样是建筑，但是可以通过，通常是地上的物品或者门

  kShapeMaskForMob = ~kShapeMaskItem // 生物不会被物品阻挡
};
