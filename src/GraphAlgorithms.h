#include "CGraphBase.h"

#pragma once
// 图计算继承类（主要是算法）
class GraphAlgorithms : public CGraph {
public:
	Graph GTemp;

	// 核心算法 ---------------------------------------------------------------------------------------
	// 多源花费
	py::dict multi_source_dijkstra_cost(
		const vector< vector<pair<int, double>> >& g,
		const vector<int>& sources,
		int& target,
		double& cut_off,
		string& weight_name);


	// 多源花费 多线程
	unordered_map<int, double> multi_source_dijkstra_cost_threading(
		const vector< vector<pair<int, double>> >& g,
		const vector<int>& sources,
		int& target,
		double& cut_off,
		string& weight_name);


	// 多源路径
	unordered_map<int, vector<int>> multi_source_dijkstra_path(
		const vector< vector<pair<int, double>> >& g,
		const vector<int>& sources,
		int target,
		double cut_off,
		string weight_name);

	unordered_map<int, vector<int>> multi_source_dijkstra_path_threading(
		const vector<vector<pair<int, double>>>& g,
		const vector<int>& sources,
		int target,
		double cut_off,
		string weight_name);


	// 多源路径花费
	dis_and_path multi_source_dijkstra(
		const vector<vector<pair<int, double>>>& g,
		const vector<int>& sources,
		int target,
		double cut_off,
		string weight_name);


	dis_and_path multi_source_dijkstra_threading(
		const vector<vector<pair<int, double>>>& g,
		const vector<int>& sources,
		int target,
		double cut_off,
		string weight_name);


	// 多源路径花费形心点
	unordered_map<int, double> multi_source_dijkstra_cost_centroid(
		const vector< vector<pair<int, double>>>& g,
		const vector<int>& sources,
		const unordered_set<int>& targets,
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
	vector<vector<int>> shortest_simple_paths_few(
		int source,
		int target,
		int K,
		const string& weight_name);


	// 获取单个OD对的花费
	pair<double, vector<int>> single_source_to_target(
		int source,
		int target,
		const string& weight_name);


	// 获取非全勤权重列表
	vector<vector<pair<int, double>>>& get_not_full_weight_map();
	

	// test-------------------------------------------------------------------
	// 定义全局变量和局部变量
	unordered_map<int, dis_and_path> global_cache_result;
	unordered_map<int, dis_and_path> temp_cache_result;



};