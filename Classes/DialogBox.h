#pragma once
#include "cocos2d.h"
#include <string>

// 一个简单的对话框，创建时会在对应的节点下加入一个名为"dialog"的标签。
// 使用静态函数的形式，使用时不需要加入新的成员函数。
typedef cocos2d::Label Dialog;

// 获取该节点对应的对话框。不存在对话框时返回nullptr。
Dialog* getDialog(cocos2d::Node* node);

// 在该节点下创建对话框。已存在对话框时不做任何操作，直接返回空指针。
Dialog* createDialog(cocos2d::Node* node, const std::string& msg = "");

// 删除该节点对应的对话框。
void destroyDialog(cocos2d::Node* node);
