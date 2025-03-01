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
	Graph G; // 图：起点节点：{终点节点1：属性字典1，终点节点2：属性字典2}			非行星点图
	Graph m_planet_start_map; // 哈希表 起点：{终点1：属性字典1，终点2：属性字典2}  行星点图
	Graph m_planet_end_map; // 哈希表 终点：{起点1：属性字典1，起点2：属性字典2}	行星点图
	NodeMap m_node_map; // 节点哈希表
	int number_link = 0; // 边数
	unordered_map<int, vector<int>> node_in_list; // 节点前导点 
	unordered_map<int, vector<int>> node_out_list; // 节点邻接点

	// 初始化
	CGraph() {
		Graph G;
	}

	// 基础加点方法
	virtual void basic_add_node(
		const int node,
		const unordered_map<string, double> attribute_dict,
		const bool is_planet = false
	);

	// 基础删点方法
	virtual void basic_remove_node(
		const int node
	);

	// 基础加边方法
	virtual void basic_add_edge(
		const int o,
		const int d,
		const unordered_map<string, double> attribute_dict_,
		const int planet_);

	// 基础删边方法
	virtual void basic_remove_edge(
		const int o,
		const int d);

	// 添加一个节点
	virtual void add_node(
		const py::object& node_,
		const py::dict& attribute_dict_,
		const py::object& planet_);

	// 添加多个节点
	virtual void add_nodes(
		const py::list& nodes);

	// 删除一个节点
	virtual void remove_node(
		const py::object& node);

	// 删除多个节点
	virtual void remove_nodes(
		const py::list& nodes);

	// 添加一条边
	virtual void add_edge(
		const py::object& u_,
		const py::object& v_,
		const py::dict& attribute_dict_,
		const py::object& planet_);

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

	// 修改节点为行星点
	virtual void set_node_as_planet(const py::object& node_);

	// 获取图的基本信息
	virtual void get_graph_info();

	// 获取点的基本信息
	virtual void get_node_info(
		const py::object& id);

	// 获取边的基本信息
	virtual void get_link_info(
		const py::object& start,
		const py::object& end);
};