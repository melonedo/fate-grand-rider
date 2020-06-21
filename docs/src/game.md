## 游戏说明
*	打开exe文件
*	游戏设置默认是静音状态，若要调出声音，则在一开始点击“设置”，拖动滑块即可把声音调到你心中合适的状态
* 	进入游戏：若不调音量直接进入，则直接点击“开始”，若是调完音量之后的界面，则点击左下角的“go on”
* 	进入游戏后，走到宝箱附近，宝箱盖子会自动打开，点击空格，就会弹出武器或者道具，当道具上没有显示名称时，再次点击宝箱，会随机出现第二个武器或道具；有名称，点击空格，则会1）使用出现的道具2）与手中武器更换
* 	进入房间后，在完全消灭敌人之前，无法退出房间，当你准备攻击的时候，选择你打算发射箭/刺出矛/掷出飞镖的方向，单击左键即可完成一次射击，若这个武器没有方向性，则点击游戏界面中任意位置即可。
* 	若你想穿过传送门，进入另一个板块，则站在走到传送门处，当传送门图片将主角完全覆盖时，就会自动完成传送
* 	在游戏过程中，可以点击“Esc”键，就可以暂停游戏，在暂停过程中，可以调节音量
* 	当你通过所有关卡之后，你可以点击“重新开始”再次挑战，或者直接退出。

## 编译说明
* 下载本项目repo：
```shell
git clone https://github.com/melonedo/fate-grand-rider.git
cd fate-grand-rider
```
* 下载[cocos2d-x-4.0](http://cocos2d-x.org/filedown/cocos2d-x-v4.0)，把cocos2d-x-4.0的文件解压并放到`cocos2d`文件夹中（coco2d中应有`cmake`, `cocos`, `external`等文件夹）；
* 进入`fate-grand-rider`后使用下列指令编译（需要vs2019和cmake）：
```shell
mkdir win32-build
cd win32-build
cmake .. -G "Visual Studio 16 2019" -A win32
cmake --build . --config Release
cd ..
```
* 运行，程序位于`win32-build/bin/fate-grand-rider/Release`，可进入文件夹后手动打开`fate-grand-rider.exe`，也可继续在命令行中使用指令：
```shell
 win32-build/bin/fate-grand-rider/Release/fate-grand-rider.exe
```
