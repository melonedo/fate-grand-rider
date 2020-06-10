# fate-grand-rider
[![docs](https://img.shields.io/badge/docs-dev-blue.svg)](https://melonedo.github.io/fate-grand-rider/dev)

2020 oop assignment

## 编译
* 下载本项目repo：
```shell
git clone https://github.com/melonedo/fate-grand-rider.git
cd fate-grand-rider
```
* 下载cocos2d-x-4.0，把cocos2d-x-4.0的文件解压并放到`cocos2d`文件夹中（coco2d中应有`cmake`, `cocos`, `external`等文件夹）；
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
