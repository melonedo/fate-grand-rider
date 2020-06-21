var documenterSearchIndex = {"docs":
[{"location":"project.html#项目介绍-1","page":"项目介绍","title":"项目介绍","text":"","category":"section"},{"location":"project.html#时间规划-1","page":"项目介绍","title":"时间规划","text":"","category":"section"},{"location":"project.html#","page":"项目介绍","title":"项目介绍","text":"(Image: timeline)","category":"page"},{"location":"project.html#","page":"项目介绍","title":"项目介绍","text":"时间 进度\n第一周 基础框架（人物、地图、物理引擎）\n第二周 部分武器、部分暂停\n第三周 互动和房间、添加音乐\n第四周 \n第五周 武器、怪物基本完工\n第六周 UI","category":"page"},{"location":"project.html#分工-1","page":"项目介绍","title":"分工","text":"","category":"section"},{"location":"project.html#","page":"项目介绍","title":"项目介绍","text":"名字 分工\n曾富楠 人物、地图、封装物理引擎\n安江涛 怪物（及对应武器）、暂停\n李沁心 武器\n香宁雨 音乐、UI、道具","category":"page"},{"location":"project.html#开发难点-1","page":"项目介绍","title":"开发难点","text":"","category":"section"},{"location":"project.html#","page":"项目介绍","title":"项目介绍","text":"问题：cocos2d-x引擎加载的瓦片地图难以注入代码；解决：利用每个节点都有的Component，把对应的互动作为Component的衍生来存储于地图的对应图块\n问题：难以寻找并处理地图中的房间；解决：地图中预先标记类别为\"room\"的图块，读取时将其作为一个房间的内侧。\n问题：地图预处理时产生的房间Room在返回的过程中多次利用移动语义转手，指针不定；解决：每个房间存储一个指向自己的unique_ptr<Room>，需要引用房间时使用对应的双重指针Room**。\n问题：cocos2d-x的物理引擎包装过于严密，难以使用；解决：重新包装chipmunk引擎。\n问题：包装时产生的物理空间Space和物理刚体Body互相引用，难以析构；解决：每个刚体存储一个shared_ptr<Space>，保证物理空间在物理刚体之后析构。","category":"page"},{"location":"index.html#Fate-grand-rider-1","page":"Fate-grand-rider","title":"Fate-grand-rider","text":"","category":"section"},{"location":"index.html#","page":"Fate-grand-rider","title":"Fate-grand-rider","text":"2020软工大项目fate-grand-rider说明文档","category":"page"},{"location":"index.html#类的框架-1","page":"Fate-grand-rider","title":"类的框架","text":"","category":"section"},{"location":"index.html#","page":"Fate-grand-rider","title":"Fate-grand-rider","text":"(Image: Inheritance graph)","category":"page"},{"location":"index.html#","page":"Fate-grand-rider","title":"Fate-grand-rider","text":"本项目中主要的（基）类的结构如图，主要包括继承自cocos2d中的动画精灵Sprite的生物Mob和武器Weapon，以及继承自cocos2d中的组件Component的互动组件Interaction。","category":"page"},{"location":"index.html#","page":"Fate-grand-rider","title":"Fate-grand-rider","text":"主要类的说明如下：","category":"page"},{"location":"index.html#","page":"Fate-grand-rider","title":"Fate-grand-rider","text":"Mob：生物，提供关于生物移动时对应物理引擎的接口，并作为Hero和Monster类的基类。\nWeapon：武器，所有的生物的攻击都通过武器来完成，武器提供的接口为攻击和瞄准。\nHero：英雄，即主角，由玩家键盘控制移动，携带的武器可以用鼠标控制进行攻击。\nMonster：怪物，行为与英雄类似，但由电脑AI控制。","category":"page"},{"location":"index.html#","page":"Fate-grand-rider","title":"Fate-grand-rider","text":"互动组件Interaction比较特殊，所有元素（英雄、怪物、地图）中的互动都以Interaction为中介进行，即首先用getInteraction获取节点对应的互动，然后再调用互动对应的函数，一个典型的例子如下：","category":"page"},{"location":"index.html#","page":"Fate-grand-rider","title":"Fate-grand-rider","text":"auto space = GameScene::getRunningScene()->getSpace();\nif (auto target = space->queryPointNearest(this->getPosition()))\n  getInteraction(target)->attack(this, _damage);","category":"page"},{"location":"index.html#","page":"Fate-grand-rider","title":"Fate-grand-rider","text":"例中展现武器类进行攻击的基本模式，首先获取了物理空间以进行搜索最近的目标，若找到目标（指针非零），则获取对应的互动组件，调用该组建的attack方法，完成一次攻击。 Interaction类所进行的互动包括，角色与地图的互动：靠近touch、离开endTouch、空格键进行对话dialog；地图中建筑与房间的互动：进入房间enterRoom、离开房间enterRoom，连接房间linkRoom；从地图中加载互动的静态方法load；受攻击时的互动attack。这些具体的互动的方法不会全部定义，如果不定义时默认忽略对应类型的互动。","category":"page"},{"location":"index.html#游戏操作-1","page":"Fate-grand-rider","title":"游戏操作","text":"","category":"section"},{"location":"index.html#","page":"Fate-grand-rider","title":"Fate-grand-rider","text":"游戏使用WASD键控制方向进行移动，单击鼠标左键进行攻击，按空格键可以与场景中的建筑进行交互。","category":"page"}]
}
