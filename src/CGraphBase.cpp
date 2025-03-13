#include "CGraphBase.h"

// 基础操作 ---------------------------------------------------------------------------------------


// 加边
void CGraph::basic_add_edge(
	const int o, const int d,
	const unordered_map<string, double> attribute_dict)
{
	// 节点初始化（保留原有属性）
	if (!m_node_map.count(o)) m_node_map[o] = {};
	if (!m_node_map.count(d)) m_node_map[d] = {};

	// 检查边是否存在
	bool is_new_edge = node_out_list[o].insert(d).second;
	if (!is_new_edge) return;  // 边已存在，直接返回
	
	// 判断形心属性
	auto& o_node = m_node_map[o];
	auto& d_node = m_node_map[d];
	bool is_o_centroid = o_node.count("centroid_") && o_node.at("centroid_") == 1;
	bool is_d_centroid = d_node.count("centroid_") && d_node.at("centroid_") == 1;


	// 根据节点的形心属性决定加边逻辑
	if (m_node_map[o]["centroid_"] == 1 && m_node_map[d]["centroid_"] == 1) {
		m_centroid_start_map[o][d] = attribute_dict;
		m_centroid_end_map[d][o] = attribute_dict;
		
	}
	else if (m_node_map[o]["centroid_"] == 1) {
		// 如果起点是形心点
		m_centroid_start_map[o][d] = attribute_dict;
		
	}
	else if (m_node_map[d]["centroid_"] == 1) {
		// 如果终点是形心点
		m_centroid_end_map[d][o] = attribute_dict;
	}
	else {
		// 非形心边
		G[o][d] = attribute_dict;
		G[d];
	}

	node_in_list[d].emplace(o);
	node_out_list[o].emplace(d);
	// 更新边的计数
	number_link += 1;

	// 遍历所有字段并更新 field_freq 和 full_field_map
	for (const auto& field : attribute_dict) {
		const string& field_name = field.first;

		// 更新 field_freq
		if (field_freq.find(field_name) == field_freq.end()) {
			// 新字段，初始化
			field_freq[field_name] = 1;
		}
		else {
			// 字段已存在，值加 1
			field_freq[field_name] += 1;
		}

		// 如果该字段的值 / number_link == 1，说明该字段已完全出现
		if (field_freq[field_name] / number_link == 1) {
			// 查找 field_name 在 field_vec 中的位置
			auto it = find(field_vec.begin(), field_vec.end(), field_name);
			if (it == field_vec.end()) {
				// 如果没有找到该字段，说明是新字段，添加到 field_vec 中
				field_vec.push_back(field_name);
				// 在 full_field_map 中新增一个 unordered_map
				full_field_map.push_back(unordered_map<int, vector<pair<int, double>>>());
				full_field_reverse_map.push_back(unordered_map<int, vector<pair<int, double>>>());
				// 获取新增字段的位置（索引）
				it = find(field_vec.begin(), field_vec.end(), field_name);
			}

			// 获取字段在 field_vec 中的位置
			int field_index = distance(field_vec.begin(), it);

			// 更新 full_field_map 中对应位置的 unordered_map
			full_field_map[field_index][o].push_back({ d, field.second });

			// 更新 full_field_reserve_map 中对应位置的 unordered_map（同步更新）
			full_field_reverse_map[field_index][d].push_back({ o, field.second });
		}
	}

}


// 删边
void CGraph::basic_remove_edge(
	const int o, const int d) 
{
	int edge_removed = 0;

	// 定义删除函数模板
	auto erase_edge = [&edge_removed](auto& container, int key1, int key2) {
		auto it = container.find(key1);
		if (it != container.end() && it->second.erase(key2)) {
			edge_removed++;
			if (it->second.empty()) container.erase(it);
		}
	};

	// 删除普通边
	erase_edge(G, o, d);
	// 删除形心起点边
	erase_edge(m_centroid_start_map, o, d);
	// 删除形心终点边（注意参数顺序）
	erase_edge(m_centroid_end_map, d, o);

	// 更新邻接表
	if (node_in_list[d].erase(o) && node_in_list[d].empty()) {
		node_in_list.erase(d);
	}
	if (node_out_list[o].erase(d) && node_out_list[o].empty()) {
		node_out_list.erase(o);
	}


	// 更新计数器
	if (edge_removed > 0) number_link = max(0, number_link - 1);
}


// 设置形心点
void CGraph::basic_set_centroid(
	int o)
{
	// 检查节点是否存在
	if (!m_node_map.count(o)) {
		py::print("Error: Node", o, "does not exist");
		return;
	}

	// 若已是行星点则跳过
	auto& node_attr = m_node_map[o];
	if (node_attr.count("centroid_") && node_attr["centroid_"] == 1) {
		py::print("Warning: Node", o, "is already a centroid");
		return;
	}

	// 标记为行星点
	node_attr["centroid_"] = 1;

	// 迁移出边到行星起点容器
	if (node_out_list.count(o)) {
		m_centroid_start_map[o] = move(G[o]); // 移动语义提升性能
		G.erase(o);
	}

	// 迁移入边到行星终点容器
	if (node_in_list.count(o)) {
		for (int i : node_in_list[o]) {
			if (G[i].count(o)) {
				m_centroid_end_map[o][i] = std::move(G[i][o]);
				G[i].erase(o);
			}
		}
	}
}


// 基础函数 ---------------------------------------------------------------------------------------


// 获取基本信息
py::dict CGraph::get_graph_info() {
	py::dict result;

	result["number_of_node"] = m_node_map.size();
	result["number_of_link"] = number_link;

	return result;
}


// 获取点的基本信息 待修改
py::dict CGraph::get_node_info(const py::object& id)
{
	py::dict result;

	try {
		int node_id = id.cast<int>();  // 可能抛出 py::cast_error

		// 检查节点是否存在
		if (m_node_map.find(node_id) == m_node_map.end()) {
			result["error"] = py::str("Node " + std::to_string(node_id) + " does not exist");
			return result;
		}

		// 计算出度（假设G是邻接表：map<int, map<int, Edge>>）
		result["out_degree"] = G[node_id].size();

	}
	catch (const py::cast_error& e) {
		result["error"] = py::str("Invalid node ID type: " + std::string(e.what()));
	}

	return result;
}


// 获取边的基本信息
py::dict CGraph::get_link_info(
	const py::object& start_,
	const py::object& end_) {
	py::dict result;

	try {
		int start = start_.cast<int>();
		int end = end_.cast<int>();

		// 检查节点是否存在
		auto check_node = [&](int node) {
			if (m_node_map.find(node) == m_node_map.end()) {
				result["error"] = py::str("Node " + std::to_string(node) + " does not exist");
				return false;
			}
			return true;
		};

		if (!check_node(start)) return result;
		if (!check_node(end)) return result;

		// 检查边是否存在
		if (G.find(start) == G.end() || G[start].find(end) == G[start].end()) {
			result["error"] = py::str("No edge between " + std::to_string(start) + " and " + std::to_string(end));
			return result;
		}

		// 构建属性字典
		for (auto& pair : G[start][end]) {
			result[pair.first.c_str()] = pair.second;
		}

	}
	catch (const py::cast_error& e) {
		result["error"] = py::str("Invalid node ID type: " + std::string(e.what()));
	}

	return result;
}


// 添加一条边
void CGraph::add_edge(
	const py::object& start_node_,
	const py::object& end_node_,
	const py::dict& attribute_dict_)
{
	int start_node = start_node_.cast<int>();
	int end_node = end_node_.cast<int>();
	auto attribute_dict = attribute_dict_.cast<unordered_map<string, double>>();

	basic_add_edge(start_node, end_node, attribute_dict);
}


// 添加多条边
void CGraph::add_edges(const py::list& edges_) {
	// 遍历每个边的三元组
	for (const auto& edge : edges_) {
		try {
			// 提取边的信息
			auto edge_tuple = edge.cast<py::tuple>();

			// 获取节点 start, end 和属性字典
			auto start_ = edge_tuple[0];
			auto end_ = edge_tuple[1];

			int start = start_.cast<int>();
			int end = end_.cast<int>();
			unordered_map<string, double> attribute_dict = {};
			if (edge_tuple.size() == 3) attribute_dict = edge_tuple[2].cast<unordered_map<string, double>>();

			// 调用基础加边算法
			basic_add_edge(start, end, attribute_dict);
		}
		catch (const py::cast_error& e) {
			std::cout << "Error: Invalid edge format." << std::endl;
			return;
		}
	}
}


// 删除一条边
void CGraph::remove_edge(
	const py::object& start_,
	const py::object& end_) {
	// 检查 start 和 end 是否是整数类型
	if (!py::isinstance<py::int_>(start_) || !py::isinstance<py::int_>(end_)) {
		std::cout << "Error: Node IDs must be of type 'int'." << std::endl;
		return;
	}

	// 转换 start 和 end 为整数类型
	int start = start_.cast<int>();
	int end = end_.cast<int>();

	// 检查图中是否存在这条边
	basic_remove_edge(start, end);
}


// 删除多条边
void CGraph::remove_edges(const py::list& edges_) {
	// 遍历每个二元元组（起点，终点）
	for (const auto& edge : edges_) {
		try {
			// 提取边的信息
			auto edge_tuple = edge.cast<py::tuple>();
			if (edge_tuple.size() != 2) {
				std::cout << "Error: Each edge must be a tuple of (start, end)." << std::endl;
				return;
			}

			// 获取节点 start 和 end
			auto start_ = edge_tuple[0];
			auto end_ = edge_tuple[1];

			// 检查 start 和 end 是否是整数类型
			if (!py::isinstance<py::int_>(start_) || !py::isinstance<py::int_>(end_)) {
				std::cout << "Error: Node IDs must be of type 'int'." << std::endl;
				return;
			}

			// 转换 start 和 end 为整数类型
			int start = start_.cast<int>();
			int end = end_.cast<int>();

			basic_remove_edge(start, end);
		}
		catch (const py::cast_error& e) {
			std::cout << "Error: Invalid edge format." << std::endl;
			return;
		}
	}
}


// 更新节点为形心点
// 修改C++函数为两个重载版本
void CGraph::set_centroid(int node) { basic_set_centroid(node); }
void CGraph::set_centroid(const std::vector<int>& nodes) {
	for (int node : nodes) { basic_set_centroid(node); }
}
