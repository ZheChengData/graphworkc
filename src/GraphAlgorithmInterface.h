#include "GraphAlgorithms.h"
#pragma once
class GraphAlgorithmInterface: public GraphAlgorithms{
public:
	// 调用方法 ---------------------------------------------------------------------------------------

	// 单源最短路径
	py::dict single_source_cost(
		const py::object& o_,
		const py::object& method_,
		const py::object& target_,
		const py::object& cut_off_,
		const py::object& weight_name_);


	unordered_map<int, vector<int>> single_source_path(
		const py::object& o_,
		const py::object& method_,
		const py::object& target_,
		const py::object& cut_off_,
		const py::object& weight_name_);


	dis_and_path single_source_all(
		const py::object& o_,
		const py::object& method_,
		const py::object& target_,
		const py::object& cut_off_,
		const py::object& weight_name_);


	// 多源最短路径
	py::dict multi_source_cost(
		const py::object& list_o,
		const py::object& method,
		const py::object& target,
		const py::object& cut_off,
		const py::object& weight_name);


	unordered_map<int, vector<int>> multi_source_path(
		const py::object& list_o,
		const py::object& method,
		const py::object& target,
		const py::object& cut_off,
		const py::object& weight_name);


	dis_and_path multi_source_all(
		const py::object& list_o,
		const py::object& method,
		const py::object& target,
		const py::object& cut_off,
		const py::object& weight_name);


	// 多个单源最短路径计算
	vector<unordered_map<int, double>> multi_single_source_cost(
		const py::object& list_o_,
		const py::object& method_,
		const py::object& target_,
		const py::object& cut_off_,
		const py::object& weight_name_,
		const py::object& num_thread_);


	vector<unordered_map<int, vector<int>>> multi_single_source_path(
		const py::object& list_o_,
		const py::object& method_,
		const py::object& target_,
		const py::object& cut_off_,
		const py::object& weight_name_,
		const py::object& num_thread_);


	vector<dis_and_path> multi_single_source_all(
		const py::object& list_o_,
		const py::object& method_,
		const py::object& target_,
		const py::object& cut_off_,
		const py::object& weight_name_,
		const py::object& num_thread_);


	// 多个多源最短路径计算
	vector<unordered_map <int, double>> multi_multi_source_cost(
		const py::object& list_o_,
		const py::object& method_,
		const py::object& target_,
		const py::object& cut_off_,
		const py::object& weight_name_,
		const py::object& num_thread_);


	vector<unordered_map<int, vector<int>>> multi_multi_source_path(
		const py::object& list_o_,
		const py::object& method_,
		const py::object& target_,
		const py::object& cut_off_,
		const py::object& weight_name_,
		const py::object& num_thread_);


	vector<dis_and_path> multi_multi_source_all(
		const py::object& list_o_,
		const py::object& method_,
		const py::object& target_,
		const py::object& cut_off_,
		const py::object& weight_name_,
		const py::object& num_thread_);


	// 花费矩阵
	py::array_t<double> cost_matrix(
		const py::object& starts,
		const py::object& ends,
		const py::object& method_,
		const py::object& cut_off_,
		const py::object& weight_name_,
		const py::object& num_thread_);


	// 路径字典：所有起点到所有终点
	py::dict path_dict(
		const py::object& starts_,
		const py::object& ends_,
		const py::object& method_,
		const py::object& cut_off_,
		const py::object& weight_name_,
		const py::object& num_thread_);


	// 路径字典：OD一一对应
	py::dict path_dict_pairwise(
		const py::object& starts_,
		const py::object& ends_,
		const py::object& method_,
		const py::object& weight_name_,
		const py::object& num_thread_);


	// 多个多源最短花费(带形心)
	vector<unordered_map<int, double>> multi_multi_source_cost_centroid(
		const vector< vector<pair<int, double>>>& g,
		const py::object& list_o_,
		const py::object& method_,
		const py::object& target_,
		const py::object& cut_off_,
		const py::object& weight_name_,
		const py::object& num_thread_);


	// K条最短路
	vector<vector<int>> k_shortest_paths(
		const py::object& source,
		const py::object& target,
		const py::object& num_k,
		const py::object& weight_name);


	// 单源节点到目标点的花费
	double shortest_path_cost(
		const py::object& source_,
		const py::object& target_,
		const py::object& weight_name_);


	// 单源节点到目标点的路径
	vector<int> shortest_path_path(
		const py::object& source_,
		const py::object& target_,
		const py::object& weight_name_);


	// 单源节点到目标点的路径
	pair<double, vector<int>> shortest_path_all(
		const py::object& source_,
		const py::object& target_,
		const py::object& weight_name_);
};

