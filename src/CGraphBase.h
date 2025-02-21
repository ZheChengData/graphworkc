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
	Graph G;
	int number_link = 0;

	// 初始化
	CGraph() {
		Graph G;
	}

	// 添加一条边
	virtual void add_edge(
		const py::object& u_,
		const py::object& v_,
		const py::object& attribute_dict_);

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