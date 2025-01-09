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
	int nodeID; // 节点编号
	double distance; // 从起点到该节点的距离

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
	vector<CNode> m_Node;// 网络节点集合
	vector<CLink> m_Link; // 网络路段集合
	vector<COrigin> m_Origin; // 网络的起点集合
	int m_nNode; // 节点数
	int m_nLink; // 路段数量
	int m_nOrigin; // 起点数量

	vector<double> LinkFlow; // 路段流量
	vector<double> LinkTravelTime; // 路段走行时间
	vector<double> ShortestPathCost; // 临时变量，所有节点到起点的最短路
	vector<int> ShortestPathParent; // 最短路上，所有节点到起点的前继路段
	unordered_map<std::pair<int, int>, int, pair_hash> LinkIndex; // 起点和终点到边ID的映射
	// 构造函数
	CNetwork()
		: m_nNode(0), m_nLink(0), m_nOrigin(0), m_Node(0) {
		// 这里可以进行任何额外的初始化
		// 比如初始化其它成员变量，或执行特定的逻辑
	}


	// 函数：分割字符串
	std::vector<std::string> SplitString(const std::string& str, char delimiter);
	// 读取节点文件
	void ReadNodeCSV(const std::string& filename);
	// 读取路段文件
	void ReadLinkCSV(const std::string& DataPath);
	// 读取OD对文件
	void ReadODpairsCSV(const std::string& DataPath);
	// 显示当前类信息
	void DisplayInfo() const;
	std::vector<int> ReconstructPath(int Start, int End, const std::vector<int>& ShortestPathParent);

	// 单元最短路径算法
	std::pair<std::vector<double>, std::vector<int>> ComputeShortestPathsFromSource(int Start);

	// 添加路段
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

