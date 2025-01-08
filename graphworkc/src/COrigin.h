#include "CNode.h"
#pragma once
class COrigin {
public:
	int ID; // 起点的编号，从零开始编号
	CNode* pOriginNode; // 起点对应的节点（指针）
	vector<int> DestinationNode; // OD对，只记录有需求的OD
	vector<double> ODDemand; // OD需求，只记录有需求的OD

	// 构造函数
	COrigin(int id = 0, CNode* originNode = nullptr)
		: ID(id), pOriginNode(originNode) {}

	// 可选择添加析构函数以释放资源
	~COrigin() {
		// 如果需要，可以在这里处理资源释放
		// 注意：如果 pOriginNode 是动态分配的，需要手动释放
		// delete pOriginNode; // 不要随意删除，确保只在原来分配时释放
	}
};

