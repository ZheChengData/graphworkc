#include "CGraphBase.h"

#pragma once
// 图计算继承类（主要是算法）
class GraphAlgorithms : public CGraph {
public:
	// 核心算法
	dis_and_path multi_source_dijkstra(
		const vector<int>& sources,
		int target = -1,
		double cutoff = numeric_limits<double>::infinity(),
		string weight_name = "no_input"
	);

	double shortest_path_dijkstra(
		int source,
		int target,
		vector<int>& path,
		unordered_set<int>& ignore_nodes,
		const string& weight_name
	);

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

	dis_and_path multi_source_all(
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

	dis_and_path single_source_all(
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

	vector<dis_and_path> multi_single_source_all(
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

	vector<dis_and_path> multi_multi_source_all(
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
};

