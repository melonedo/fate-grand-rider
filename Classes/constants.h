#pragma once
// 定义全局的常量
#include "cocos2d.h"

// 建议都设置为const static以避免产生重复定义

const static cocos2d::Size designResolutionSize = cocos2d::Size(1024, 768);
const static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
const static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
const static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

enum MapPriority{ kMapPriorityBackground = -100, kMapPrioritySprite = 0, kMapPriorityForeground = 100 };

const static int kTileResolution = 16, kWeaponResolution = 34;
static int kSpriteResolution = 32;

enum AnimationTag{ 
  kTagStandAnimation = 1000, 
  kTagWalkingAnimation };

enum SpriteTag { kTagHero = 2000, kTagWeapon, kTagWall, kTagInteractable};

enum PhysicsShapeMask {
  //kShapeMaskPlayer = 1 << 1,
  kShapeMaskMob = 1 << 2,
  kShapeMaskTile = 1 << 3,
  kShapeMaskBullet = 1 << 4
};