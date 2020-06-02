#pragma once
#include "cocos2d.h"
using namespace cocos2d;

class PauseScene :public Layer
{
public:
	static Scene* scene(Sprite* sqr);
	CREATE_FUNC(PauseScene);
};