#include "PauseGame.h"
USING_NS_CC;
//暂时先这样吧，以后再改
bool PauseGame::init()
{
	registeTouchEvent();
	return true;
}
void PauseGame::registeTouchEvent()
{
	auto* dispatcher = Director::getInstance()->getEventDispatcher();
	auto* keyListener = EventListenerKeyboard::create();

	keyListener->onKeyPressed = CC_CALLBACK_2(PauseGame::onKeyPressed, this);
	dispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);
}
//重写onKeyPressed方法来实现具体按键的判断
void PauseGame::onKeyPressed(EventKeyboard::KeyCode keycode, Event* event) {
	if (keycode == EventKeyboard::KeyCode::KEY_ESCAPE) {
		Size visibleSize = Director::getInstance()->getVisibleSize();
		RenderTexture* renderTexture = RenderTexture::create(visibleSize.width, visibleSize.height);
		Scene* scene = Director::getInstance()->getRunningScene();
		renderTexture->begin();
		scene->visit();
		renderTexture->end();
		Director::getInstance()->pushScene(PauseScene::scene(renderTexture));
	}
}