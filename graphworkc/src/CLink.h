#include "CNode.h"
#pragma once
class CLink {
public:
	int ID; // 路段的编号，从零开始编号
	shared_ptr<CNode> pInNode; // 使用指针
	shared_ptr<CNode> pOutNode; // 使用指针

	double FreeFlowTravelTime; // AB自由流走行时间
	double TravelTime; // 走行时间
	double Capacity; // 路段通行能力
	double DIR; // 路段的方向信息，表示是否是单项或双向
	double Alpha = 0.15; // BPR函数参数，一般取0.15
	double Power = 4.0; // BPR函数参数，一般取4.0

	// 默认构造函数
	CLink()
		:
		ID(0),
		pInNode(nullptr),
		pOutNode(nullptr),
		FreeFlowTravelTime(0.0),
		Capacity(0.0),
		DIR(0.0) {}
};
