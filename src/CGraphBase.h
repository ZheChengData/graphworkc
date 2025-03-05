#include <algorithm>
#include <atomic>
#include <chrono>
#include <cmath>
#include <deque>
#include <fstream>
#include <future>
#include <limits>
#include <mutex>
#include <sstream>
#include <string>
#include <unordered_map>
#include <queue>
#include <thread>
#include <map>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include <pybind11/stl_bind.h>
#pragma once

using namespace std;
using namespace chrono;
namespace py = pybind11;

using Graph = unordered_map<int, unordered_map<int, unordered_map<string, double>>>;
using NodeMap = unordered_map<int, unordered_map<string, double>>;

struct dis_and_path {
	unordered_map<int, double> distances;
	unordered_map<int, vector<int>> paths;
};

struct shortest_Path {
	double length;
	vector<int> nodes;

	// 用于优先队列排序（路径长度最短的优先）
	bool operator>(const shortest_Path& other) const {
		return length > other.length;
	}
};

class CGraph {
public:
	Graph G; // 图：起点节点：{终点节点1：属性字典1，终点节点2：属性字典2}			非形心点图
	unordered_map<int, vector<pair<int, double>>> G_temp;
	Graph m_centroid_start_map; // 哈希表 起点：{终点1：属性字典1，终点2：属性字典2}  形心点图
	Graph m_centroid_end_map; // 哈希表 终点：{起点1：属性字典1，起点2：属性字典2}	形心点图
	NodeMap m_node_map; // 节点哈希表
	int number_link = 0; // 边数
	unordered_map<int, unordered_set<int>> node_in_list; // 节点前导点 
	unordered_map<int, unordered_set<int>> node_out_list; // 节点邻接点

	// 初始化
	CGraph() {
		Graph G;
	}

	// 基本算法 ---------------------------------------------------------------------------------------
	// 加边
	virtual void basic_add_edge(
		const int o,
		const int d,
		const unordered_map<string, double> attribute_dict_);

	// 删边
	virtual void basic_remove_edge(
		const int o,
		const int d);

	// 设为形心点
	virtual void basic_set_centroid(
		int o);

	// 基本算法 ---------------------------------------------------------------------------------------

	// 基础操作 ---------------------------------------------------------------------------------------
	
	// 获取图的基本信息
	virtual py::dict get_graph_info();

	// 获取点的基本信息
	virtual py::dict get_node_info(
		const py::object& id);

	// 获取边的基本信息
	virtual py::dict get_link_info(
		const py::object& start,
		const py::object& end);

	// 添加一条边
	virtual void add_edge(
		const py::object& u_,
		const py::object& v_,
		const py::dict& attribute_dict_);

	// 添加多条边
	virtual void add_edges(
		const py::list& edges_);

	// 删除一条边
	virtual void remove_edge(
		const py::object& u_,
		const py::object& v_);

	// 删除多条边
	virtual void remove_edges(
		const py::list& edges_);

	// 修改节点为形心点
	virtual void set_centroid(int node);
	virtual void set_centroid(const vector<int>& nodes);
};