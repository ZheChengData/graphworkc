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
	// 如果起点是形心点
	else if (m_node_map[o]["centroid_"] == 1) {
		m_centroid_start_map[o][d] = attribute_dict;
		
	}
	// 如果终点是形心点
	else if (m_node_map[d]["centroid_"] == 1) {
		m_centroid_end_map[d][o] = attribute_dict;
	}
	// 非形心边
	else {
		G[o][d] = attribute_dict;
		G[d];
	}

	node_in_list[d].emplace(o);
	node_out_list[o].emplace(d);
	// 更新边的计数
	number_link += 1;

	// 如果节点没被录入，则录入节点
	if (!map_id_to_index.count(o)) {
		vec_index_to_id.resize(vec_index_to_id.size() + 1); // 扩展数组
		map_id_to_index[o] = cur_max_index; // 填充数据
		vec_index_to_id[cur_max_index] = o;
		cur_max_index++;
	}
	if (!map_id_to_index.count(d)) {
		vec_index_to_id.resize(vec_index_to_id.size() + 1); // 扩展数组
		map_id_to_index[d] = cur_max_index; // 填充数据
		vec_index_to_id[cur_max_index] = d;
		cur_max_index++;
	}

	// 遍历所有字段并更新 field_freq 和 full_field_map
	for (const auto& field : attribute_dict) {
		const string& field_name = field.first;

		// 更新 field_freq
		if (field_freq.find(field_name) == field_freq.end()) field_freq[field_name] = 1;
		else field_freq[field_name] += 1;

		// 如果该字段的值 / number_link == 1，说明该字段已完全出现
		if (field_freq[field_name] / number_link == 1) {
			// 查找 field_name 在 field_vec 中的位置
			auto it = find(field_vec.begin(), field_vec.end(), field_name);
			if (it == field_vec.end()) {
				field_vec.push_back(field_name); // 如果没有找到该字段，说明是新字段，添加到 field_vec 中
				full_field_map.push_back(unordered_map<int, vector<pair<int, double>>>());
				full_field_reverse_map.push_back(unordered_map<int, vector<pair<int, double>>>());
				index_to_id_next_vec.resize(index_to_id_next_vec.size() + 1);
				it = find(field_vec.begin(), field_vec.end(), field_name); // 获取新增字段的位置（索引）
			}
			int field_index = distance(field_vec.begin(), it); // 获取字段在 field_vec 中的位置
			
			// 如果不存在，则创建一个空的列表
			// 确保 field_index 的索引不会超出 index_to_id_next_vec 的大小
			if (field_index >= index_to_id_next_vec.size()) {
				index_to_id_next_vec.resize(field_index + 1); // 扩展 vector
			}

			// 确保 map_id_to_index[o] 的索引不会超出 index_to_id_next_vec[field_index] 的大小
			if (map_id_to_index[o] >= index_to_id_next_vec[field_index].size()) {
				index_to_id_next_vec[field_index].resize(map_id_to_index[o] + 1); // 扩展 vector
			}
			// 直接 emplace_back 添加 {map_id_to_index[d], field.second}
			index_to_id_next_vec[field_index][map_id_to_index[o]].emplace_back(map_id_to_index[d], field.second);

			full_field_map[field_index][o].push_back({ d, field.second });
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
	if (node_in_list[d].erase(o) && node_in_list[d].empty()) node_in_list.erase(d);
	if (node_out_list[o].erase(d) && node_out_list[o].empty()) node_out_list.erase(o);

	// 删除 full_field_map 中的边
	for (auto& field_map : full_field_map) {
		// 查找 o -> d 边并删除
		auto it = field_map.find(o);
		if (it != field_map.end()) {
			auto& edges = it->second;
			for (auto edge_it = edges.begin(); edge_it != edges.end(); ) {
				if (edge_it->first == d) {
					edge_it = edges.erase(edge_it); // 删除该边
					edge_removed++;
				}
				else {
					++edge_it;
				}
			}
			// 如果该节点的边为空，移除该节点
			if (edges.empty()) {
				field_map.erase(it);
			}
		}
	}

	// 删除 full_field_reverse_map 中的边
	for (auto& field_map : full_field_reverse_map) {
		// 查找 d -> o 边并删除
		auto it = field_map.find(d);
		if (it != field_map.end()) {
			auto& edges = it->second;
			for (auto edge_it = edges.begin(); edge_it != edges.end(); ) {
				if (edge_it->first == o) {
					edge_it = edges.erase(edge_it); // 删除该边
					edge_removed++;
				}
				else {
					++edge_it;
				}
			}
			// 如果该节点的边为空，移除该节点
			if (edges.empty()) {
				field_map.erase(it);
			}
		}
	}

	// 假设 o 是源节点ID，d 是目标节点ID
	if (map_id_to_index.find(o) != map_id_to_index.end()) {
		int o_index = map_id_to_index[o];  // 全局映射到索引

		// 遍历所有权重字段（第一层）
		for (auto& weight_field : index_to_id_next_vec) {  // 第一层：不同权重字段
			// 检查索引是否在有效范围内
			if (o_index < weight_field.size()) {
				// 获取该权重字段下节点 o 的邻接表
				auto& adjacency_list = weight_field[o_index];

				// 删除邻接表中目标为 d 的边
				auto it = adjacency_list.begin();
				while (it != adjacency_list.end()) {
					if (it->first == map_id_to_index[d]) {
						it = adjacency_list.erase(it);  // 删除边
					}
					else {
						++it;
					}
				}
			}
		}
	}

	// 更新计数器
	if (edge_removed > 0) number_link = max(0, number_link - 1);
}


// 设置形心点
void CGraph::basic_set_centroid(int o) {
	// 检查节点是否存在
	if (!m_node_map.count(o)) {
		py::print("Error: Node", o, "does not exist");
		return;
	}

	// 若已是形心点则跳过
	auto& node_attr = m_node_map[o];
	if (node_attr.count("centroid_") && node_attr["centroid_"] == 1) {
		py::print("Warning: Node", o, "is already a centroid");
		return;
	}

	// 标记为形心点
	node_attr["centroid_"] = 1;

	// 获取节点 o 的索引
	auto o_it = map_id_to_index.find(o);
	if (o_it == map_id_to_index.end()) {
		py::print("Error: Node", o, "has no index mapping");
		return;
	}
	const int o_index = o_it->second;

	// 迁移出边到形心起点容器（并更新索引邻接表）
	if (node_out_list.count(o)) {
		// 遍历所有权重字段，清空该节点的出边
		for (auto& field_adj : index_to_id_next_vec) { // field_adj 对应一个权重字段
			if (o_index < field_adj.size()) {
				field_adj[o_index].clear(); // 清空该字段下该节点的所有出边
			}
		}

		m_centroid_start_map[o] = move(G[o]);
		G.erase(o);
	}

	// 迁移入边到形心终点容器（并更新索引邻接表）
	if (node_in_list.count(o)) {
		for (int i : node_in_list[o]) {
			if (G[i].count(o)) {
				// 获取来源节点 i 的索引
				auto i_it = map_id_to_index.find(i);
				if (i_it == map_id_to_index.end()) continue;
				const int i_index = i_it->second;

				// 遍历所有权重字段，删除 i 到 o 的边
				for (auto& field_adj : index_to_id_next_vec) {
					if (i_index >= field_adj.size()) continue;
					auto& adj_list = field_adj[i_index];
					// 删除邻接表中目标为 o_index 的边
					for (auto it = adj_list.begin(); it != adj_list.end();) {
						if (it->first == o_index) {
							it = adj_list.erase(it);
						}
						else {
							++it;
						}
					}
				}

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
py::dict CGraph::get_node_info(
	const py::object& id)
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
	const py::object& end_) 
{
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
void CGraph::add_edges(
	const py::list& edges_)
{
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
	const py::object& end_) 
{
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
void CGraph::remove_edges(
	const py::list& edges_)
{
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
