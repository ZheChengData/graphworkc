#include "CNode.h"
#pragma once
class CLink {
public:
	int ID; // ·�εı�ţ����㿪ʼ���
	shared_ptr<CNode> pInNode; // ʹ��ָ��
	shared_ptr<CNode> pOutNode; // ʹ��ָ��

	double FreeFlowTravelTime; // AB����������ʱ��
	double TravelTime; // ����ʱ��
	double Capacity; // ·��ͨ������
	double DIR; // ·�εķ�����Ϣ����ʾ�Ƿ��ǵ����˫��
	double Alpha = 0.15; // BPR����������һ��ȡ0.15
	double Power = 4.0; // BPR����������һ��ȡ4.0

	// Ĭ�Ϲ��캯��
	CLink()
		:
		ID(0),
		pInNode(nullptr),
		pOutNode(nullptr),
		FreeFlowTravelTime(0.0),
		Capacity(0.0),
		DIR(0.0) {}
};
