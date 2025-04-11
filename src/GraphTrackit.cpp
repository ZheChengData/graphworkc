#include "GraphTrackit.h"

// 1.计算全局变量,存储结果
bool GraphTrackit::calc_global_cache(
	const py::object& o_list_,
	const py::object& cut_off_,
	const py::object& thread_num_,
	const py::object& weight_name_)
{
	auto weight_name = weight_name_.cast<string>();
	try {
		// 1. 计算获得需求结果
		py::object method_ = py::cast("Dijkstra");
		py::object target_ = py::cast(-1);

		// 调用 multi_single_source_all 获取当前结果
		vector<dis_and_path> cur_res = multi_single_source_all(
			o_list_,
			method_,
			target_,
			cut_off_,
			weight_name_,
			thread_num_);

		// 2. 将结果加入全局变量
		auto o_list = o_list_.cast<vector<int>>();
		for (int i = 0; i < o_list.size(); i++) {
			// 添加当前节点的路径和成本到 global_cache_result
			global_cache_result[o_list[i]] = cur_res[i];
			for (auto pair : G[o_list[i]]) {
				global_cache_result[o_list[i]].cost[pair.first] = pair.second[weight_name];
				global_cache_result[o_list[i]].paths[pair.first] = { o_list[i] , pair.first };
			}

		}

		return true;  // 如果没有异常，返回 true
	}
	catch (const py::error_already_set& e) {
		// 捕获 Python 相关的异常
		std::cerr << "Python exception occurred: " << e.what() << std::endl;
	}
	catch (const std::exception& e) {
		// 捕获标准 C++ 异常
		std::cerr << "Standard exception occurred: " << e.what() << std::endl;
	}

	return false;  // 如果发生异常，返回 false
}


// 2.删除临时变量
bool GraphTrackit::del_temp_cache()
{
	try {
		// 清空 temp_cache_result
		temp_cache_result.clear();

		// 检查容器是否为空（清空是否成功）
		if (temp_cache_result.empty()) {
			return true; // 清空成功
		}
		else {
			throw std::runtime_error("Failed to clear temp_cache_result.");
		}
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return false; // 捕获异常并返回 false
	}
}


// 3.是否存在路径
tuple<bool, vector<int>, double> GraphTrackit::has_path(
	const py::object& o_,
	const py::object& d_,
	const py::object& use_cache_,
	const py::object& cut_off_,
	const py::object& weight_name_)
{
	try {
		// 1.初始化
		auto o = o_.cast<int>();
		auto d = d_.cast<int>();
		auto use_cache = use_cache_.cast<bool>();
		auto cut_off = cut_off_.cast<double>();
		auto weight_name = weight_name_.cast<string>();

		// 1.1.检查原图G 是否存在起点 o
		if (1) {
			auto graph_it = G.find(o);
			if (graph_it != G.end()) {
				auto& adj_nodes = graph_it->second;
				auto adj_it = adj_nodes.find(d);
				if (adj_it != adj_nodes.end()) {
					// 确保权重存在
					
					if (adj_it->second.count(weight_name)) {
						double cost = adj_it->second[weight_name];
						return { true, {o, d}, cost };
					}
				}
			}
		}

		// 2.计算结果
		if (use_cache) {
			// 检查全局存储变量global_cache_result 是否存在起点 o
			if (global_cache_result.find(o) != global_cache_result.end()) {
				// 获取与起点 o 关联的 dis_and_path
				dis_and_path& dp = global_cache_result[o];

				if (dp.cost.find(d) != dp.cost.end()) {
					return { true, dp.paths[d], dp.cost[d] };
				}
				else {
					return { false, {}, -1 };
				}
			}

			// 检查局部存储变量temp_cache_result 是否存在起点 o
			else if (temp_cache_result.find(o) != temp_cache_result.end()) {
				if (temp_cache_result[o].cost.find(d) != temp_cache_result[o].cost.end()) {

					return { true, temp_cache_result[o].paths[d], temp_cache_result[o].cost[d] };
				}
				else {
					return { false, {}, -1 };
				}
			}
			// 前三个都没发现起点
			else {
				// 如果没有找到起点 o 对应的缓存
				return { false, {}, -1 };
			}
		}
		else {
			// 1.初始化
			const auto& weight_map = get_weight_map(weight_name);
			const auto& reverse_map = get_weight_reverse_map(weight_name);
			const set<int> ignore_nodes;
			const set<pair<int, int>> ignore_edges;

			// 双向Dijkstra计算最短路径 
			auto cur_res = bidirectional_dijkstra_ignore(
				reverse_map,
				weight_map,
				{ o },
				d,
				cut_off,
				ignore_nodes,
				ignore_edges);

			// 检查是否包含目标 d 的相关结果
			if (cur_res.cost.find(d) != cur_res.cost.end() && !cur_res.paths[d].empty()) {
				return { true, cur_res.paths[d], cur_res.cost[d] };
			}
			else {
				return { false, {}, -1 };
			}
		}
	}
	catch (const std::exception& e) {
		// 捕获标准异常并返回
		std::cerr << "Standard exception caught: " << e.what() << std::endl;
		return { false, {}, -1 };
	}
	catch (const py::error_already_set& e) {
		// 捕获 Python 异常并返回
		std::cerr << "Python exception caught: " << e.what() << std::endl;
		return { false, {}, -1 };
	}
}


// 4.计算gotrackit_calc
py::object GraphTrackit::gotrackit_calc(
	const py::object& seq_k_candidate_info,
	const py::object& gps_adj_dis_map_,
	const py::object& use_global_cache_,
	const py::object& not_conn_cost_,
	const py::object& num_thread_,
	const py::object& cut_off_,
	const py::object& weight_name_)
{
	// 1.初始化
	auto gps_adj_dis_map = gps_adj_dis_map_.cast<unordered_map<int, double>>();
	auto use_global_cache = use_global_cache_.cast<bool>();
	auto not_conn_cost = not_conn_cost_.cast<double>();
	auto num_thread = num_thread_.cast<int>();
	auto cut_off = cut_off_.cast<double>();
	auto weight_name = weight_name_.cast<string>();

	vector<int> f_list, t_list, fl_list, tl_list;
	vector<double> gap_list, rt_list;

	vector<RowData> net = convert_dataframe(seq_k_candidate_info);
	map<int, vector<RowData>> seq_groups = group_by_seq(net);

	// 2.获取排序后的唯一seq列表
	vector<int> unique_sorted_values;
	transform(seq_groups.begin(), seq_groups.end(), back_inserter(unique_sorted_values),
		[](const auto& pair) { return pair.first; });
	sort(unique_sorted_values.begin(), unique_sorted_values.end());

	// 3.计算临时变量 temp_cache_result
	if (!use_global_cache) calc_temp_cache(net, num_thread_, cut_off_, weight_name_);
	struct Result {
		int f, t, fl, tl;
		double gap, rt;
	};
	vector<Result> results;
	// 4.处理相邻 seq 对, 计算结果
	auto process_seq_pairs = [&](auto& result_cache) {
		// 预计算每个外层循环的偏移量和总长度
		vector<size_t> offsets;
		size_t total_pairs = 0;
		for (size_t i = 0; i < unique_sorted_values.size() - 1; ++i) {
			int front = unique_sorted_values[i], back = unique_sorted_values[i + 1];
			auto it0 = seq_groups.find(front), it1 = seq_groups.find(back);
			if (it0 != seq_groups.end() && it1 != seq_groups.end()) {
				offsets.push_back(total_pairs);
				total_pairs += it0->second.size() * it1->second.size();
			}
			else {
				offsets.push_back(total_pairs);
			}
		}

		// 预分配内存并初始化默认值
		f_list.resize(total_pairs, -1);
		t_list.resize(total_pairs, -1);
		fl_list.resize(total_pairs, -1);
		tl_list.resize(total_pairs, -1);
		gap_list.resize(total_pairs, not_conn_cost);
		rt_list.resize(total_pairs, not_conn_cost);

		// 处理每个循环
		#pragma omp parallel for schedule(guided) num_threads(num_thread)
		for (int i = 0; i < unique_sorted_values.size() - 1; ++i) {
			int front = unique_sorted_values[i], back = unique_sorted_values[i + 1];
			auto it0 = seq_groups.find(front), it1 = seq_groups.find(back);
			if (it0 == seq_groups.end() || it1 == seq_groups.end()) continue;

			auto& net_0 = it0->second, net_1 = it1->second;
			const size_t base_idx = offsets[i];

			// seq双循环
			for (int idx = 0; idx < net_0.size() * net_1.size(); ++idx) {
				size_t from_idx = idx / net_1.size();
				size_t to_idx = idx % net_1.size();
				// 计算全局索引
				const size_t global_idx = base_idx + from_idx * net_1.size() + to_idx;

				auto& row_from = net_0[from_idx];
				auto& row_to = net_1[to_idx];
				auto& from_seq = row_from.seq;
				auto& from_link = row_from.single_link_id;
				auto& from_route_dis = row_from.route_dis;
				auto& from_link_f = row_from.from_node;
				auto& from_link_t = row_from.to_node;
				auto& to_seq = row_to.seq;
				auto& to_link = row_to.single_link_id;
				auto& to_route_dis = row_to.route_dis;
				auto& to_link_f = row_to.from_node;
				auto& to_link_t = row_to.to_node;

				double cur_x = not_conn_cost;
				double cur_r = not_conn_cost;
				double R = 0;
				vector<int> Q = {};

				// 如果路径直接在G中，则直接赋值
				if (!use_global_cache) {
					// 检查图中直接连接
					auto graph_it = G.find(from_link_f);
					if (graph_it != G.end()) {
						auto& adj_nodes = graph_it->second;
						auto adj_it = adj_nodes.find(to_link_f);
						if (adj_it != adj_nodes.end()) {
							// 确保权重存在
							if (!adj_it->second.count(weight_name)) {
								throw runtime_error("Missing weight '" + weight_name + "' in edge");
							}
							R = adj_it->second.at(weight_name);
							Q = { from_link_f, to_link_f };
							goto skip_query_cache;
						}
						else {
							// 起点存在但终点不存在，查询缓存
							goto query_cache;
						}
					}
					else {
						// 起点不存在，查询缓存
						goto query_cache;
					}
				query_cache:
					auto from_node_paths = result_cache.find(from_link_f);
					if (from_node_paths != result_cache.end()) {
						auto to_node_path = from_node_paths->second.paths.find(to_link_f);
						if (to_node_path != from_node_paths->second.paths.end()) {
							Q = to_node_path->second;
							R = from_node_paths->second.cost.at(to_link_f);
						}
						else {
							R = -1.0;
							Q.clear();
						}
					}
					else {
						R = -1.0;
						Q.clear();
					}

				skip_query_cache:;
				}
				else {
					auto from_node_paths = result_cache.find(from_link_f);
					if (from_node_paths != result_cache.end()) {
						auto to_node_path = from_node_paths->second.paths.find(to_link_f);
						if (to_node_path != from_node_paths->second.paths.end()) {
							Q = to_node_path->second;
							R = result_cache[from_link_f].cost[to_link_f];
						}
						else {
							R = -1.0;
							Q.clear();
						}
					}
					else {
						R = -1.0;
						Q.clear();
					}
				}

				// 条件1: 同一条link
				if (from_link == to_link) {
					cur_r = abs(0 - from_route_dis + to_route_dis);
					cur_x = abs(cur_r - gps_adj_dis_map[front]);
				}

				// 条件2: 形成环
				else if (from_link_f == to_link_t && from_link_t == to_link_f) {
					cur_r = abs(R - from_route_dis + to_route_dis);
					cur_x = abs(cur_r - gps_adj_dis_map[front]);
				}

				// 条件3: 有效路径且满足拓扑
				else if (R > 0 && Q.size() >= 2 && Q[1] == from_link_t && Q[Q.size() - 2] != to_link_t) {
					cur_r = abs(R - from_route_dis + to_route_dis);
					cur_x = abs(cur_r - gps_adj_dis_map[front]);
				}

				// 直接写入预分配位置
				f_list[global_idx] = front;
				t_list[global_idx] = back;
				fl_list[global_idx] = row_from.single_link_id;
				tl_list[global_idx] = row_to.single_link_id;
				gap_list[global_idx] = cur_x;
				rt_list[global_idx] = cur_r;
			}
		}
	};

	if (use_global_cache) {
		process_seq_pairs(global_cache_result);
	}
	else {
		process_seq_pairs(temp_cache_result);
	}

	// 5.返回结果
	py::gil_scoped_acquire acquire;

	py::dict columns;
	columns["f"] = py::array_t<int>(f_list.size(), f_list.data());
	columns["t"] = py::array_t<int>(t_list.size(), t_list.data());
	columns["fl"] = py::array_t<int>(fl_list.size(), fl_list.data());
	columns["tl"] = py::array_t<int>(tl_list.size(), tl_list.data());
	columns["gap"] = py::array_t<double>(gap_list.size(), gap_list.data());
	columns["rt"] = py::array_t<double>(rt_list.size(), rt_list.data());

	py::object df = pandas.attr("DataFrame")(columns);

	return df;
}


// 0.计算临时变量
void GraphTrackit::calc_temp_cache(
	vector<RowData> net,
	const py::object& num_thread_,
	const py::object& cut_off_,
	const py::object& weight_name_)
{
	unordered_set<int> unique_from_nodes;
	for (const auto& pair : net) {
		unique_from_nodes.insert(pair.from_node);
	}
	vector<int> unique_vec_from_node(unique_from_nodes.begin(), unique_from_nodes.end());

	// 过滤掉已经在 temp_cache_result 中的节点
	unique_vec_from_node.erase(
		remove_if(unique_vec_from_node.begin(), unique_vec_from_node.end(),
			[this](int node) {
		return temp_cache_result.find(node) != temp_cache_result.end();
	}),
		unique_vec_from_node.end());

	py::object method_ = py::cast("Dijkstra");
	py::object target_ = py::cast(-1);
	py::object unique_vec_from_node_p = py::cast(unique_vec_from_node);

	vector<dis_and_path> cur_res = multi_single_source_all(
		unique_vec_from_node_p, method_, target_, cut_off_, weight_name_, num_thread_);

	for (size_t i = 0; i < unique_vec_from_node.size(); i++) {
		temp_cache_result[unique_vec_from_node[i]] = cur_res[i];
	}
}


// 0.数据结构转换：将dataframe转换为c++结构
vector<RowData> GraphTrackit::convert_dataframe(py::object df)
{
	std::vector<RowData> rows;

	py::array seq_array = df.attr("seq").cast<py::array>();
	py::array single_link_id_array = df.attr("single_link_id").cast<py::array>();
	py::array from_node_array = df.attr("from_node").cast<py::array>();
	py::array to_node_array = df.attr("to_node").cast<py::array>();
	py::array length_array = df.attr("length").cast<py::array>();
	//py::array dir_array = df.attr("dir").cast<py::array>();
	py::array prj_dis_array = df.attr("prj_dis").cast<py::array>();
	py::array route_dis_array = df.attr("route_dis").cast<py::array>();

	auto seq = seq_array.unchecked<int>();
	auto single_link_id = single_link_id_array.unchecked<int>();
	auto from_node = from_node_array.unchecked<int>();
	auto to_node = to_node_array.unchecked<int>();
	auto length = length_array.unchecked<double>();
	//auto dir = dir_array.unchecked<int>();
	auto prj_dis = prj_dis_array.unchecked<double>();
	auto route_dis = route_dis_array.unchecked<double>();

	for (py::ssize_t i = 0; i < seq.shape(0); ++i) {
		RowData row;
		row.seq = seq(i);
		row.single_link_id = single_link_id(i);
		row.from_node = from_node(i);
		row.to_node = to_node(i);
		row.length = length(i);
		//row.dir = dir(i);
		row.prj_dis = prj_dis(i);
		row.route_dis = route_dis(i);
		rows.push_back(row);
	}

	return rows;
}


// 0.seq分组
map<int, vector<RowData>> GraphTrackit::group_by_seq(const std::vector<RowData>& new_net)
{
	std::map<int, std::vector<RowData>> seq_groups;
	for (const auto& row : new_net) {
		seq_groups[row.seq].push_back(row);
	}
	return seq_groups;
}
