#include <iostream>
#include <vector>

#pragma once

using namespace std;

class CNode {
public:
	int ID; // �ڵ�ı�ţ����㿪ʼ���
	int zone_ID;// ����ID
	double PositionX; // �ڵ��X����
	double PositionY; // �ڵ��Y����
	int Origin_ID = 0; // �ڵ��Ӧ������ţ�-1��ʾ�������
	vector<int> IncomingLink; // ����ڵ��·�α�ż���
	vector<int> OutgoingLink; // �뿪�ڵ��·�α�ż���
	// ���� == ���������Ƚ����������Ƿ����
	bool operator==(const CNode& other) const {
		return this->ID == other.ID;
	}
	// ���캯��
	CNode(int id = 0, double x = 0.0, double y = 0.0, int originId = -1, int zone_ID = -1)
		: ID(id), PositionX(x), PositionY(y), Origin_ID(originId) {}
};

