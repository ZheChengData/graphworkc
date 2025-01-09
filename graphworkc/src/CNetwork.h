#include <algorithm>
#include <atomic>
#include <chrono>
#include <cmath>
#include <deque>
#include <fstream>
#include <future>
#include <limits>
#include <limits>
#include <mutex>
#include <sstream>
#include <string>
#include <unordered_map>
#include <queue>
#include <thread>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <map>

#include "CNode.h"
#include "CLink.h"
#include "COrigin.h"
#pragma once
using namespace std;
using namespace chrono;
namespace py = pybind11;
struct NodeDistancePair {
	int nodeID; // �ڵ���
	double distance; // ����㵽�ýڵ�ľ���

	bool operator>(const NodeDistancePair& other) const {
		return this->distance > other.distance;
	}
};
struct pair_hash {
	template <class T1, class T2>
	std::size_t operator() (const std::pair<T1, T2>& pair) const {
		return std::hash<T1>()(pair.first) ^ (std::hash<T2>()(pair.second) << 1);
	}
};
class CNetwork
{
public:
	vector<CNode> m_Node;// ����ڵ㼯��
	vector<CLink> m_Link; // ����·�μ���
	vector<COrigin> m_Origin; // �������㼯��
	int m_nNode; // �ڵ���
	int m_nLink; // ·������
	int m_nOrigin; // �������

	vector<double> LinkFlow; // ·������
	vector<double> LinkTravelTime; // ·������ʱ��
	vector<double> ShortestPathCost; // ��ʱ���������нڵ㵽�������·
	vector<int> ShortestPathParent; // ���·�ϣ����нڵ㵽����ǰ��·��
	unordered_map<std::pair<int, int>, int, pair_hash> LinkIndex; // �����յ㵽��ID��ӳ��
	// ���캯��
	CNetwork()
		: m_nNode(0), m_nLink(0), m_nOrigin(0), m_Node(0) {
		// ������Խ����κζ���ĳ�ʼ��
		// �����ʼ��������Ա��������ִ���ض����߼�
	}


	// �������ָ��ַ���
	std::vector<std::string> SplitString(const std::string& str, char delimiter);
	// ��ȡ�ڵ��ļ�
	void ReadNodeCSV(const std::string& filename);
	// ��ȡ·���ļ�
	void ReadLinkCSV(const std::string& DataPath);
	// ��ȡOD���ļ�
	void ReadODpairsCSV(const std::string& DataPath);
	// ��ʾ��ǰ����Ϣ
	void DisplayInfo() const;
	std::vector<int> ReconstructPath(int Start, int End, const std::vector<int>& ShortestPathParent);

	// ��Ԫ���·���㷨
	std::pair<std::vector<double>, std::vector<int>> ComputeShortestPathsFromSource(int Start);

	// ���·��
	void AddEdgeFrom(const py::tuple& t);
	void AddEdgeFromC(int first, int second, double travelTime);
	tuple<double, vector<int>> single_source_dijkstra(int Start, int End);
	tuple<unordered_map<int, double>, unordered_map<int, vector<int>>> single_source_dijkstra(int Start);
	void InitializeLinkIndex() {
		for (const CLink& link : m_Link) {
			LinkIndex[{link.pInNode->ID, link.pOutNode->ID}] = link.ID;
		}
	}
};

