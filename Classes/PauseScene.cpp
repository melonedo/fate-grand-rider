#include"PauseScene.h"
#include"DataSet.h"
Scene* PauseScene::scene(Sprite* sqr)
{
	Scene* myscene = Scene::create();
	PauseScene* mylayer = PauseScene::create();
	myscene->addChild(mylayer);

	Sprite* _spr = sqr;
	Size visibleSize = Director::getInstance()->getVisibleSize();
	_spr->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));//����λ��
	//_spr->setFlippedY(true);//��ת����ΪUI�����OpenGL���겻ͬ
	//_spr->setColor(Color3B::GRAY);//��ɫ��Ϊ��ɫ
	myscene->addChild(_spr);

	//������Ϸ
	auto label = Label::createWithTTF("go on", "fonts/Marker Felt.ttf", 60);
	assert(label);
	label->setAnchorPoint(Vec2(0.5f, 0.5f));
	label->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	myscene->addChild(label);

	auto listener = EventListenerMouse::create();
	listener->onMouseMove = [](EventMouse* event) {
		// ����ƶ�����ǩ��ʱ�Ŵ��ǩ
		auto target = event->getCurrentTarget();
		auto bbox = target->getBoundingBox();
		if (bbox.containsPoint(Vec2(event->getCursorX(), event->getCursorY()))) {
			target->setScale(1.1f);
		}
		else { // �Ƴ�ʱ���ԭ����
			target->setScale(1);
		}
	};
	listener->onMouseDown = [&](EventMouse* event)
	{
		auto target = event->getCurrentTarget();
		auto bbox = target->getBoundingBox();
		if (bbox.containsPoint(Vec2(event->getCursorX(), event->getCursorY()))) {
			Director::getInstance()->resume();
			Director::getInstance()->popScene();//������һ����������������Ϸ
		}
	};
	label->getEventDispatcher()->addEventListenerWithSceneGraphPriority(
		listener, label);

	return myscene;
}


