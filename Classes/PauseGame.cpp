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
		/*Size visibleSize = Director::getInstance()->getVisibleSize();
		RenderTexture* renderTexture = RenderTexture::create(visibleSize.width, visibleSize.height);
		Scene* scene = Director::getInstance()->getRunningScene();
		renderTexture->begin();
		scene->visit();
		renderTexture->end();
		auto director = Director::getInstance();
		director->pause();
		//Director::getInstance()->pushScene(PauseScene::scene(renderTexture));
		*/
		Director::getInstance()->getTextureCache()->removeAllTextures();
		PauseGame::capture(this);
	}
}

//屏幕截图
void PauseGame::capture(Ref* sender)
{
	CCLOG("ScreenShot");
	utils::captureScreen(CC_CALLBACK_2(PauseGame::afterCapture, this), "D:/fate-grand-rider/Resources/ScreenShot.png");
}

//截图后执行afterCapture
void PauseGame::afterCapture(bool succeed, const std::string& outputFile)
{
	if (succeed)
	{
		CCLOG("%s", outputFile.c_str());
		//显示截图
		Sprite* sp = Sprite::create(outputFile);
		Director::getInstance()->pushScene(PauseScene::scene(sp));
	}
	else
	{
		CCLOG("Capture screen failed.");
	}
}