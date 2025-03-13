#include "CGraphBase.h"

#pragma once
// 图计算继承类（主要是算法）
class GraphAlgorithms : public CGraph {
public:
	Graph GTemp;

	// 核心算法 ---------------------------------------------------------------------------------------
	// 多源花费
	unordered_map<int, double> multi_source_dijkstra_cost(
		const unordered_map<int, vector<pair<int, double>>>& g,
		const vector<int>& sources,
		int& target,
		double& cut_off,
		string& weight_name);


	// 多源路径
	unordered_map<int, vector<int>> multi_source_dijkstra_path(
		const unordered_map<int, vector<pair<int, double>>>& g,
		const vector<int>& sources,
		int target,
		double cut_off,
		string weight_name);


	// 多源路径花费
	dis_and_path multi_source_dijkstra(
		const unordered_map<int, vector<pair<int, double>>>& g,
		const vector<int>& sources,
		int target,
		double cut_off,
		string weight_name);


	// 多源路径花费形心点
	unordered_map<int, double> multi_source_dijkstra_cost_centroid(
		const vector<int>& sources,
		int target,
		double cut_off,
		string weight_name);


	// 非全勤权重邻接字典获取
	unordered_map<int, vector<pair<int, double>>> weight_func(
		string weight_name);


	// 非全勤权重前导字典获取
	unordered_map<int, vector<pair<int, double>>> weight_reverse_func(
		string weight_name);


	// 获取正向权重
	const unordered_map<int, vector<pair<int, double>>>&
		get_weight_map(const string& weight_name);


	// 获取反向权重
	const unordered_map<int, vector<pair<int, double>>>&
		get_weight_reverse_map(const string& weight_name);


	// 双向Dijkstra算法
	dis_and_path bidirectional_dijkstra(
		const unordered_map<int, vector<pair<int, double>>>& reverse_g,
		const unordered_map<int, vector<pair<int, double>>>& g,
		const vector<int>& sources,
		int target,
		double cut_off);


	// 双向Dijkstra算法 存在ignore
	dis_and_path bidirectional_dijkstra_ignore(
		const unordered_map<int, vector<pair<int, double>>>& reverse_g,
		const unordered_map<int, vector<pair<int, double>>>& g,
		const vector<int>& sources,
		int target,
		double cut_off,
		const set<int>& ignore_nodes,
		const set<pair<int, int>>& ignore_edges);


	// 计算长度
	double calculate_path_length(
		const unordered_map<int, vector<pair<int, double>>>& g,
		const vector<int>& path,
		const string& weight);


	// 获取K条最短路径 K大于一定值时
	vector<vector<int>> shortest_simple_paths_much(
		int source,
		int target,
		int K,
		const string& weight_name);


	// 获取K条最短路径 K小于一定值时
	vector<vector<int>> GraphAlgorithms::shortest_simple_paths_few(
		int source,
		int target,
		int K,
		const string& weight_name);
	// 调用方法 ---------------------------------------------------------------------------------------


	// 多源最短路径
	unordered_map<int, double> multi_source_cost(
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


	// 单源最短路径
	unordered_map<int, double> single_source_cost(
		const py::object& o_,
		const py::object& method_,
		const py::object& target_,
		const py::object& cut_off_,
		const py::object& weight_name_);


	unordered_map<int, std::vector<int>> single_source_path(
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
	vector<unordered_map<int, double>> multi_multi_source_cost(
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
	py::array_t<double> cost_matrix_to_numpy(
		const py::object& starts,
		const py::object& ends,
		const py::object& method_,
		const py::object& cut_off_,
		const py::object& weight_name_,
		const py::object& num_thread_);


	// 路径字典
	py::dict path_list_to_numpy(
		const py::object& starts_,
		const py::object& ends_,
		const py::object& method_,
		const py::object& cut_off_,
		const py::object& weight_name_,
		const py::object& num_thread_);


	// 多个多源最短花费(带形心)
	vector<unordered_map<int, double>> multi_multi_source_cost_centroid(
		const py::object& list_o_,
		const py::object& method_,
		const py::object& target_,
		const py::object& cut_off_,
		const py::object& weight_name_,
		const py::object& num_thread_);


	// K条最短路
	vector<vector<int>> shortest_paths(
		const py::object& source,
		const py::object& target,
		const py::object& num_k,
		const py::object& weight_name);
	// test-------------------------------------------------------------------
	
	vector<unordered_map<int, double>> test(
		const py::object& list_o_,
		const py::object& method_,
		const py::object& target_,
		const py::object& cut_off_,
		const py::object& weight_name_,
		const py::object& num_thread_);

	unordered_map<int, double> test1(
		const vector<int>& sources,
		int target,
		double cut_off,
		string weight_name);
	
};


