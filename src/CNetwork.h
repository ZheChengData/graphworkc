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
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include <pybind11/stl_bind.h>
#include <mutex>
#pragma once

using namespace std;
using namespace chrono;
namespace py = pybind11;

using Graph = unordered_map<int, unordered_map<int, unordered_map<string, double>>>;

struct Dis_and_Path {
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
private:
	Graph G;
	int number_link = 0;
public:

	// 初始化
	CGraph() {
		Graph G;
	}

	// 核心算法
	Dis_and_Path multi_source_dijkstra(
		const vector<int>& sources,
		int target = -1,
		double cutoff = numeric_limits<double>::infinity(),
		string weight_name = "no_input"
	);

	double CGraph::shortest_path_dijkstra(
		int source,
		int target,
		vector<int>& path,
		unordered_set<int>& ignore_nodes,
		const string& weight_name
	);

	// 添加一条边
	void add_edge(
		const py::object& u_,
		const py::object& v_,
		const py::object& attribute_dict_
	);

	// 添加多条边
	void add_edges(
		const py::list& edges_
	);

	// 删除一条边
	void remove_edge(
		const py::object& u_,
		const py::object& v_
	);

	// 删除多条边
	void remove_edges(
		const py::list& edges_
	);

	// 获取图的基本信息
	void get_graph_info();

	// 获取点的基本信息
	void get_node_info(
		const py::object& id
	);

	// 获取边的基本信息
	void get_link_info(
		const py::object& start,
		const py::object& end
	);

	// 结果计算 ---------------------------------------------------------------------------------------
	// 多源最短路径计算
	unordered_map<int, double> multi_source_cost(
		const py::object& list_o,
		const py::object& method, 
		const py::object& target,
		const py::object& cutoff, 
		const py::object& weight_name 
	);

	unordered_map<int, vector<int>> multi_source_path(
		const py::object& list_o,
		const py::object& method,
		const py::object& target,
		const py::object& cutoff,
		const py::object& weight_name
	);

	Dis_and_Path multi_source_all(
		const py::object& list_o,
		const py::object& method,
		const py::object& target,
		const py::object& cutoff,
		const py::object& weight_name
	);

	// 单源最短路径计算
	unordered_map<int, double> single_source_cost(
		const py::object& o_,
		const py::object& method_,
		const py::object& target_,
		const py::object& cutoff_,
		const py::object& weight_name_
	);

	unordered_map<int, std::vector<int>> single_source_path(
		const py::object& o_,
		const py::object& method_,
		const py::object& target_,
		const py::object& cutoff_,
		const py::object& weight_name_
	);

	Dis_and_Path single_source_all(
		const py::object& o_,
		const py::object& method_,
		const py::object& target_,
		const py::object& cutoff_,
		const py::object& weight_name_
	);

	// 多个单源最短路径计算
	vector<unordered_map<int, double>> multi_single_source_cost(
		const py::object& list_o_,
		const py::object& method_,
		const py::object& target_,
		const py::object& cutoff_,
		const py::object& weight_name_,
		const py::object& num_thread_
	);

	vector<unordered_map<int, vector<int>>> multi_single_source_path(
		const py::object& list_o_,
		const py::object& method_,
		const py::object& target_,
		const py::object& cutoff_,
		const py::object& weight_name_,
		const py::object& num_thread_
	);

	vector<Dis_and_Path> multi_single_source_all(
		const py::object& list_o_,
		const py::object& method_,
		const py::object& target_,
		const py::object& cutoff_,
		const py::object& weight_name_,
		const py::object& num_thread_
	);

	// 多个多源最短路径计算
	vector<unordered_map<int, double>> multi_multi_source_cost(
		const py::object& list_o_,
		const py::object& method_,
		const py::object& target_,
		const py::object& cutoff_,
		const py::object& weight_name_,
		const py::object& num_thread_
	);

	vector<unordered_map<int, vector<int>>> multi_multi_source_path(
		const py::object& list_o_,
		const py::object& method_,
		const py::object& target_,
		const py::object& cutoff_,
		const py::object& weight_name_,
		const py::object& num_thread_
	);

	vector<Dis_and_Path> multi_multi_source_all(
		const py::object& list_o_,
		const py::object& method_,
		const py::object& target_,
		const py::object& cutoff_,
		const py::object& weight_name_,
		const py::object& num_thread_
	);

	py::array_t<double> cost_matrix_to_numpy(
		const py::object& starts,
		const py::object& ends,
		const py::object& method_,
		const py::object& cutoff_,
		const py::object& weight_name_,
		const py::object& num_thread_
	);

	py::dict path_list_to_numpy(
		const py::object& starts_,
		const py::object& ends_,
		const py::object& method_,
		const py::object& cutoff_,
		const py::object& weight_name_,
		const py::object& num_thread_
	);

	vector<vector<int>> shortest_simple_paths(
		const py::object& start,
		const py::object& end,
		const py::object& weight_name_
	);
	// 结果计算 ---------------------------------------------------------------------------------------

};