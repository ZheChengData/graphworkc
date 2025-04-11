#include "GraphAlgorithmInterface.h"

#pragma once
class GraphTrackit : public GraphAlgorithmInterface {
private:
	py::object pandas;

public:
	GraphTrackit() {
		py::gil_scoped_acquire acquire;
		pandas = py::module::import("pandas");
	}

	// 1.计算全局变量
	bool calc_global_cache(
		const py::object& o_list_,
		const py::object& cut_off_,
		const py::object& thread_num_,
		const py::object& weight_name_);

	// 2.删除临时变量
	bool del_temp_cache();


	// 补充 计算临时变量
	void calc_temp_cache(
		vector<RowData> net,
		const py::object& num_thread_,
		const py::object& cut_off_,
		const py::object& weight_name_);


	// 3.是否存在路径
	tuple<bool, vector<int>, double> has_path(
		const py::object& o_,
		const py::object& d_,
		const py::object& use_cache_,
		const py::object& cut_off_,
		const py::object& weight_name_);


	// 4.计算gotrackit_calc
	py::object gotrackit_calc(
		const py::object& seq_k_candidate_info,
		const py::object& gps_adj_dis_map,
		const py::object& use_global_cache,
		const py::object& num_threading,
		const py::object& not_conn_cost,
		const py::object& cut_off,
		const py::object& weight_name_);

	vector<RowData> convert_dataframe(py::object df);
	map<int, vector<RowData>> group_by_seq(const std::vector<RowData>& new_net);
};