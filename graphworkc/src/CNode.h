#include <iostream>
#include <vector>

#pragma once

using namespace std;

class CNode {
public:
	int ID; // 节点的编号，从零开始编号
	int zone_ID;// 区域ID
	double PositionX; // 节点的X坐标
	double PositionY; // 节点的Y坐标
	int Origin_ID = 0; // 节点对应的起点编号，-1表示不是起点
	vector<int> IncomingLink; // 进入节点的路段编号集合
	vector<int> OutgoingLink; // 离开节点的路段编号集合
	// 重载 == 操作符来比较两个对象是否相等
	bool operator==(const CNode& other) const {
		return this->ID == other.ID;
	}
	// 构造函数
	CNode(int id = 0, double x = 0.0, double y = 0.0, int originId = -1, int zone_ID = -1)
		: ID(id), PositionX(x), PositionY(y), Origin_ID(originId) {}
};

