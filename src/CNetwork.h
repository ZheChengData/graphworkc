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
#include <pybind11/numpy.h>
#include <map>
#include <iomanip>

#include "CNode.h"
#include "CLink.h"
#pragma once

using namespace std;
using namespace chrono;
namespace py = pybind11;

struct NodeDistancePair {
	int nodeID; 
	double distance; 

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

struct path_result
{
	std::unordered_map<int, double> dict_cost;
	std::unordered_map<int, std::vector<int>> dict_path;
};

class CNetwork{
public:
	vector<CNode> m_Node; // 网络节点集合
	vector<CLink> m_Link;  // 网络路段集合
	int m_nNode; // 节点数
	int m_nLink; // 路段数量

	vector<double> LinkFlow; // 路段流量
	vector<double> ShortestPathCost; // 临时变量，所有节点到起点的最短路花费
	vector<int> ShortestPathParent; // 最短路上，所有节点到起点的前继路段

	unordered_map<pair<int, int>, int, pair_hash> LinkIndex; // 起点和终点到边ID的映射
	unordered_map<int, size_t> ID2Index_map; // 节点ID到m_Node索引的映射
	unordered_map<int, size_t> ID2Index_linkmap; // 节点ID到m_Node索引的映射

	unordered_map<int, double> dic_cost;
	unordered_map<int, vector<int>> dic_path;
	unordered_map < int, path_result> m_path_result;


	CNetwork()
		: m_nNode(0), m_nLink(0), m_Node(0) {

	}

	// 清空路网
	void ClearAll();

	// 添加一条边
	void AddEdgeFromTuple(const py::tuple& t);

	// 添加多条边
	void AddEdgesFromList(const std::vector<py::tuple>& tupleList);

	// 移除一条边
	void RemoveEdge(int first, int second);

	// 移除多条边
	void RemoveEdges(const vector<pair<int, int>>& edges);

	// 最短路径 迪杰斯特拉算法 
	void SingleSourceDijkstra(int Start, double cut_off = 9999999);

	// 单源最短路径 
	// 仅返回路径
	unordered_map<int, std::vector<int>> SingleSourcePath(int Start,  string method = "Dijkstra", double cut_off = 9999999);
	// 仅返回花费
	unordered_map<int, double> SingleSourceCost(int Start, string method = "Dijkstra", double cut_off = 9999999);
	// 返回所有结果
	pair<unordered_map<int, double>, unordered_map<int, std::vector<int>>> SingleSourceALL(int Start, string method = "Dijkstra", double cut_off = 9999999);

	// 多源最短路径
	// 仅返回路径
	unordered_map <int, unordered_map<int, vector<int>>> MultiSourcePath(vector<int> StartNodes, string method = "Dijkstra", double cut_off = 9999999);
	// 仅返回花费
	unordered_map<int, unordered_map<int, double>> MultiSourceCost(vector<int> StartNodes, string method = "Dijkstra", double cut_off = 9999999);
	// 返回所有结果
	pair<unordered_map <int, unordered_map<int, vector<int>>>, unordered_map<int, unordered_map<int, double>>> MultiSourceAll( vector<int> StartNodes, string method = "Dijkstra", double cut_off = 9999999);

	// 最短路径花费矩阵输出
	void CostMartixToCsv(vector<int> vec_start, vector<int> vec_end, const std::string file_path);

	// 最短路径数据输出
	void PathToCsv(vector<int> vec_start, vector<int> vec_end, const std::string file_path);

	// 建立节点ID与路段ID联系
	void InitializeLinkIndex();

	// 将数据转换为 Python dict
	py::dict DictToPython() {
		py::dict py_dict;

		for (auto& entry : m_path_result) {
			// 将 path_result 的 dict_cost 转换为 Python 字典
			py::dict py_cost;
			for (auto& cost_entry : entry.second.dict_cost) {
				// 使用 py::cast 显式转换 C++ int 到 Python int
				py_cost[py::cast(cost_entry.first)] = cost_entry.second;
			}

			// 将 path_result 的 dict_path 转换为 Python 字典
			py::dict py_path;
			for (auto& path_entry : entry.second.dict_path) {
				py_path[py::cast(path_entry.first)] = py::cast(path_entry.second); // std::vector<int> -> Python list
			}

			// 将每个 entry 中的数据 (dict_cost 和 dict_path) 作为一个子字典加入到主字典中
			py_dict[py::cast(entry.first)] = py::make_tuple(py_cost, py_path);
		}

		return py_dict;
	}

};

