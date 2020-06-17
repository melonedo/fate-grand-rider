#pragma once
#include "cocos2d.h"
#include "PauseScene.h"
using namespace cocos2d;

class PauseGame :public Node
{
public:
	virtual bool init();
	CREATE_FUNC(PauseGame); 
	virtual void onKeyPressed(EventKeyboard::KeyCode keycode, Event* event);
	void capture(Ref* sender);
  void afterCapture(bool succeed, const std::string& outputFile);
private:
	void registeTouchEvent();

};