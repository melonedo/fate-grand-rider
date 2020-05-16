#pragma once
// 定义全局的常量
#include "cocos2d.h"

// 建议都设置为const static以避免产生重复定义

const static cocos2d::Size designResolutionSize = cocos2d::Size(1024, 768);
const static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
const static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
const static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

enum MapPriority{ kMapPriorityBackground = -100, kMapPrioritySprite = 0, kMapPriorityForeground = 100 };

const static int kSpriteResolution = 32, kTileResolution = 16, kWeaponResolution = 34;

enum { 
  kTagStandAnimation = 1000, 
  kTagWalkingAnimation };