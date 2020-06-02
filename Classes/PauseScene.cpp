#include"PauseScene.h"
#include"DataSet.h"
Scene* PauseScene::scene(Sprite* sqr)
{
	Scene* myscene = Scene::create();
	PauseScene* mylayer = PauseScene::create();
	myscene->addChild(mylayer);

	Sprite* _spr = sqr;
	Size visibleSize = Director::getInstance()->getVisibleSize();
	_spr->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));//中心位置
	//_spr->setFlippedY(true);//翻转，因为UI坐标和OpenGL坐标不同
	//_spr->setColor(Color3B::GRAY);//颜色改为灰色
	myscene->addChild(_spr);

	//继续游戏
	auto label = Label::createWithTTF("go on", "fonts/Marker Felt.ttf", 60);
	assert(label);
	label->setAnchorPoint(Vec2(0.5f, 0.5f));
	label->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	myscene->addChild(label);

	auto listener = EventListenerMouse::create();
	listener->onMouseMove = [](EventMouse* event) {
		// 鼠标移动到标签上时放大标签
		auto target = event->getCurrentTarget();
		auto bbox = target->getBoundingBox();
		if (bbox.containsPoint(Vec2(event->getCursorX(), event->getCursorY()))) {
			target->setScale(1.1f);
		}
		else { // 移出时设回原比例
			target->setScale(1);
		}
	};
	listener->onMouseDown = [&](EventMouse* event)
	{
		auto target = event->getCurrentTarget();
		auto bbox = target->getBoundingBox();
		if (bbox.containsPoint(Vec2(event->getCursorX(), event->getCursorY()))) {
			Director::getInstance()->resume();
			Director::getInstance()->popScene();//返回上一个场景，即继续游戏
		}
	};
	label->getEventDispatcher()->addEventListenerWithSceneGraphPriority(
		listener, label);

	return myscene;
}


