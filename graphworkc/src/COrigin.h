#include "CNode.h"
#pragma once
class COrigin {
public:
	int ID; // ���ı�ţ����㿪ʼ���
	CNode* pOriginNode; // ����Ӧ�Ľڵ㣨ָ�룩
	vector<int> DestinationNode; // OD�ԣ�ֻ��¼�������OD
	vector<double> ODDemand; // OD����ֻ��¼�������OD

	// ���캯��
	COrigin(int id = 0, CNode* originNode = nullptr)
		: ID(id), pOriginNode(originNode) {}

	// ��ѡ����������������ͷ���Դ
	~COrigin() {
		// �����Ҫ�����������ﴦ����Դ�ͷ�
		// ע�⣺��� pOriginNode �Ƕ�̬����ģ���Ҫ�ֶ��ͷ�
		// delete pOriginNode; // ��Ҫ����ɾ����ȷ��ֻ��ԭ������ʱ�ͷ�
	}
};

