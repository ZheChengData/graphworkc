#include "CGraphBase.h"

// 定义一个互斥锁
mutex result_mutex;

// 基本操作 ---------------------------------------------------------------------------------------

// 获取基本信息
void CGraph::get_graph_info() {
	cout << "number of node: " << m_node_map.size() << endl;
	cout << "number of link: " << number_link << endl;
}

// 获取点的基本信息
void CGraph::get_node_info(const py::object& id)
{
	// 检查机制

	// 检查 id 是否为整数类型
	if (!py::isinstance<py::int_>(id)) {
		// 抛出自定义的 TypeError
		throw py::type_error("Error: Expected an integer for the 'id' argument, but got: " + std::string(py::str(id.get_type().attr("__name__"))));
	}

	int id_new = id.cast<int>();
	// 检查 start 节点是否存在
	if (m_node_map.find(id_new) == m_node_map.end()) {
		cout << "Error: Node with id " << id_new << " does not exist in the graph." << endl;
		return;
	}

	// 逻辑执行
	cout << "Number of output degree: " << G[id_new].size() << endl;
}

// 获取边的基本信息
void CGraph::get_link_info(const py::object& start_, const py::object& end_) {
	// 检查机制
	// 检查 start 和 end 是否是整数类型
	if (!py::isinstance<py::int_>(start_) || !py::isinstance<py::int_>(end_)) {
		cout << "Error: Node IDs must be of type 'int'." << endl;
		return;
	}

	int start = start_.cast<int>();
	int end = end_.cast<int>();
	// 检查 start 节点是否存在
	if (m_node_map.find(start) == m_node_map.end()) {
		cout << "Error: Node " << start << " does not exist in the graph." << endl;
		return;
	}

	// 检查 end 节点是否存在
	if (m_node_map.find(end) == m_node_map.end()) {
		cout << "Error: Node " << end << " does not exist in the graph." << endl;
		return;
	}

	// 检查 start -> end 边是否存在
	if (G[start].find(end) == G[start].end()) {
		cout << "Error: No edge exists between nodes " << start << " and " << end << "." << endl;
		return;
	}

	// 逻辑执行
	for (const auto& pair : G[start][end]) {
		cout << pair.first << ": " << pair.second << endl;
	}
}

// 基础加点
void CGraph::basic_add_node(
	const int o,
	const unordered_map<string, double> attribute_dict,
	const bool is_planet) {
	m_node_map[o] = attribute_dict;
	if (is_planet) m_node_map[o]["planet_"] = 1;
}

// 基础删点
void CGraph::basic_remove_node(const int node) {
	// 1. 检查节点存在性
	if (m_node_map.find(node) == m_node_map.end()) {
		return; // 节点不存在直接返回
	}

	// 2. 处理行星点关联
	int is_planet = 0;
	if (m_node_map.find(node) != m_node_map.end()) {
		is_planet = m_node_map[node].find("planet_") != m_node_map[node].end();

		// 删除行星点映射关系
		if (is_planet == 1) {
			// 作为行星起点
			if (m_planet_start_map.find(node) != m_planet_start_map.end()) {
				for (auto& entry : m_planet_start_map[node]) { 
					int dst = entry.first; // 键为终点节点
					// 更新对端节点的入边列表
					auto& in_list = node_in_list[dst];
					in_list.erase(remove(in_list.begin(), in_list.end(), node), in_list.end());
				}
				number_link -= m_planet_start_map[node].size();
				m_planet_start_map.erase(node);
			}

			// 作为行星终点
			if (m_planet_end_map.find(node) != m_planet_end_map.end()) {
				for (auto& entry : m_planet_end_map[node]) { 
					int src = entry.first; // 键为起点节点
					// 更新对端节点的出边列表
					auto& out_list = node_out_list[src];
					out_list.erase(std::remove(out_list.begin(), out_list.end(), node), out_list.end());
				}
				number_link -= m_planet_end_map[node].size();
				m_planet_end_map.erase(node);
			}
		}
	}

	// 3. 删除普通图中的边
	// 处理出边
	if (node_out_list.find(node) != node_out_list.end()) {
		for (int dst : node_out_list[node]) {
			if (G.find(node) != G.end() && G[node].find(dst) != G[node].end()) {
				G[node].erase(dst);
				number_link--;

				// 更新对端节点的入边列表
				auto& in_list = node_in_list[dst];
				in_list.erase(remove(in_list.begin(), in_list.end(), node), in_list.end());
			}
		}
		node_out_list.erase(node);
	}

	// 处理入边
	if (node_in_list.find(node) != node_in_list.end()) {
		for (int src : node_in_list[node]) {
			if (G.find(src) != G.end() && G[src].find(node) != G[src].end()) {
				G[src].erase(node);
				number_link--;

				// 更新对端节点的出边列表
				auto& out_list = node_out_list[src];
				out_list.erase(remove(out_list.begin(), out_list.end(), node), out_list.end());
			}
		}
		node_in_list.erase(node);
	}

	// 4. 删除节点自身
	G.erase(node);
	m_node_map.erase(node);
}

// 基础加边
void CGraph::basic_add_edge(
	const int o, const int d,
	const unordered_map<string, double> attribute_dict,
	const int is_planet) {

	// 参数校验
	if (is_planet < 0 || is_planet > 3) {
		throw std::invalid_argument("is_planet must be 0-3");
	}

	// 节点初始化（保留原有属性）
	if (!m_node_map.count(o)) m_node_map[o] = {};
	if (!m_node_map.count(d)) m_node_map[d] = {};

	// 邻接表去重
	auto& in_list = node_in_list[d];
	if (find(in_list.begin(), in_list.end(), o) == in_list.end()) {
		in_list.push_back(o);
	}

	auto& out_list = node_out_list[o];
	if (find(out_list.begin(), out_list.end(), d) == out_list.end()) {
		out_list.push_back(d);
	}

	// 边存储逻辑
	if (is_planet == 0) {
		G[o][d] = attribute_dict;
	}
	else if (is_planet == 1) {
		m_planet_start_map[o][d] = attribute_dict;
		m_node_map[o]["planet_"] = 1;  // 不覆盖其他属性
	}
	else if (is_planet == 2) {
		m_planet_end_map[d][o] = attribute_dict;
		m_node_map[d]["planet_"] = 1;
	}
	else if (is_planet == 3) {
		m_planet_start_map[o][d] = attribute_dict;
		m_planet_end_map[d][o] = attribute_dict;
		m_node_map[o]["planet_"] = 1;
		m_node_map[d]["planet_"] = 1;
	}

	number_link += 1;
}

// 基础删边
void CGraph::basic_remove_edge(
	const int o, const int d) {
	bool edge_removed = false;

	// 删除普通边
	if (G.count(o) && G[o].count(d)) {
		G[o].erase(d);
		edge_removed = true;
	}

	// 删除行星起点边
	if (m_planet_start_map.count(o) && m_planet_start_map[o].count(d)) {
		m_planet_start_map[o].erase(d);
		edge_removed = true;
	}

	// 删除行星终点边
	if (m_planet_end_map.count(d) && m_planet_end_map[d].count(o)) {
		m_planet_end_map[d].erase(o);
		edge_removed = true;
	}

	// 更新邻接表
	if (node_in_list.count(d)) {
		auto& in_vec = node_in_list[d];
		in_vec.erase(remove(in_vec.begin(), in_vec.end(), o), in_vec.end());
		if (in_vec.empty()) node_in_list.erase(d);
	}

	if (node_out_list.count(o)) {
		auto& out_vec = node_out_list[o];
		out_vec.erase(remove(out_vec.begin(), out_vec.end(), d), out_vec.end());
		if (out_vec.empty()) node_out_list.erase(o);
	}

	// 更新计数器
	if (edge_removed) number_link = max(0, number_link - 1);
}

// 加一个点
void CGraph::add_node(
	const py::object& node_,
	const py::dict& attribute_dict_,
	const py::object& planet_) {

	int o = node_.cast<int>();
	auto attribute_dict = attribute_dict_.cast<unordered_map<string, double>>();
	bool is_planet = planet_.cast<bool>();

	basic_add_node(o, attribute_dict, is_planet);
}

// 加多个点
void CGraph::add_nodes(const py::list& nodes) {
	for (const auto& node : nodes) {
		// 类型检查
		if (!py::isinstance<py::tuple>(node)) {
			std::cerr << "Skipping invalid node (not a tuple): "
				<< py::str(node).cast<string>() << endl;
			continue;
		}

		auto node_tuple = node.cast<py::tuple>();
		const int min_size = 1; // 最小需要节点ID
		const int max_size = 3; // ID + 属性 + 行星标记

		// 参数长度校验
		if (node_tuple.size() < min_size || node_tuple.size() > max_size) {
			std::cerr << "Skipping invalid node tuple. Expected format:\n"
				<< "(node_id) 或 (node_id, attr_dict) 或 (node_id, attr_dict, is_planet)"
				<< endl;
			continue;
		}

		// 解析节点ID
		const auto& id_obj = node_tuple[0];
		if (!py::isinstance<py::int_>(id_obj)) {
			std::cerr << "Skipping invalid node ID: "
				<< py::str(id_obj).cast<string>() << endl;
			continue;
		}
		const int node_id = id_obj.cast<int>();

		// 解析属性字典
		unordered_map<string, double> attrs;
		if (node_tuple.size() >= 2) {
			try {
				attrs = node_tuple[1].cast<unordered_map<string, double>>();
			}
			catch (const py::cast_error&) {
				std::cerr << "Skipping node " << node_id
					<< ": invalid attribute dict" << endl;
				continue;
			}
		}

		// 解析行星标记
		bool is_planet = false;
		if (node_tuple.size() == 3) {
			try {
				is_planet = node_tuple[2].cast<bool>();
			}
			catch (...) {
				std::cerr << "Skipping node " << node_id
					<< ": is_planet must be 0 or 1" << endl;
				continue;
			}
		}

		// 调用底层方法
		basic_add_node(node_id, attrs, is_planet);
	}
}

// 删除一个点
void CGraph::remove_node(
	const py::object& node_) {
	// 检查 node_ 是否是整数类型
	if (!py::isinstance<py::int_>(node_)) {
		std::cout << "Error: Node IDs must be of type 'int'." << std::endl;
		return;
	}

	// 转换 node_ 为整数类型
	int node = node_.cast<int>();

	// 检查图中是否存在这条边
	basic_remove_node(node);
}

// 删除多个点
void CGraph::remove_nodes(
	const py::list& nodes_) {
	// 遍历列表
	for (const auto& node_ : nodes_) {
		// 检查 node 是否是整数类型
		if (!py::isinstance<py::int_>(node_)) {
			std::cout << "Error: Node IDs must be of type 'int'." << std::endl;
			return;
		}

		// 转换 node 为整数类型
		int node = node_.cast<int>();

		// 删除点
		basic_remove_node(node);
	}
}

// 加一条边
void CGraph::add_edge(
	const py::object& start_node_,
	const py::object& end_node_,
	const py::dict& attribute_dict_,
	const py::object& planet_) {
	// 检查机制
	if (1) {
		// 检查 u 和 v 是否是整数类型
		if (!py::isinstance<py::int_>(start_node_) || !py::isinstance<py::int_>(end_node_)) {
			cout << "Error: Node IDs must be of type 'int'." << endl;
			return;
		}

		// 尝试转换 attribute_dict_
		try {
			auto attribute_dict = attribute_dict_.cast<unordered_map<string, double>>();
		}
		catch (const py::cast_error& e) {
			cout << "Error: Attribute dictionary must be of type 'dict{string, float}'." << endl;
			return;
		}
	}
	
	int start_node = start_node_.cast<int>();
	int end_node = end_node_.cast<int>();
	auto attribute_dict = attribute_dict_.cast<unordered_map<string, double>>();
	int is_planet = planet_.cast<int>();

	// 1
	basic_add_edge(start_node, end_node, attribute_dict, is_planet);
}

// 加多条边
void CGraph::add_edges(const py::list& edges_) {
	// 遍历每个边的三元组
	for (const auto& edge : edges_) {
		try {
			// 提取边的信息
			auto edge_tuple = edge.cast<py::tuple>();
			if (edge_tuple.size() != 3 && edge_tuple.size() != 4 && edge_tuple.size() != 2) {
				std::cout << "Error: Each edge must be a tuple of (start, end, attribute_dict) or (start, end, attribute_dict, is_planet) or (start, end)." << std::endl;
				return;
			}

			// 获取节点 start, end 和属性字典
			auto start_ = edge_tuple[0];
			auto end_ = edge_tuple[1];

			// 转换节点 u 和 v 为整数类型
			int start = start_.cast<int>();
			int end = end_.cast<int>();
			unordered_map<string, double> attribute_dict = {};
			if (edge_tuple.size() == 3) attribute_dict = edge_tuple[2].cast<unordered_map<string, double>>();
			int is_planet = 0;
			if (edge_tuple.size() == 4) is_planet = edge_tuple[3].cast<int>();

			// 1
			basic_add_edge(start, end, attribute_dict, is_planet);
		}
		catch (const py::cast_error& e) {
			std::cout << "Error: Invalid edge format." << std::endl;
			return;
		}
	}
}

// 删除一条边
void CGraph::remove_edge(
	const py::object& u_,
	const py::object& v_) {
	// 检查 u 和 v 是否是整数类型
	if (!py::isinstance<py::int_>(u_) || !py::isinstance<py::int_>(v_)) {
		std::cout << "Error: Node IDs must be of type 'int'." << std::endl;
		return;
	}

	// 转换 u 和 v 为整数类型
	int u = u_.cast<int>();
	int v = v_.cast<int>();

	// 检查图中是否存在这条边
	basic_remove_edge(u, v);
}

// 删除多条边
void CGraph::remove_edges(const py::list& edges_) {
	// 遍历每个二元元组（起点，终点）
	for (const auto& edge : edges_) {
		try {
			// 提取边的信息
			auto edge_tuple = edge.cast<py::tuple>();
			if (edge_tuple.size() != 2) {
				std::cout << "Error: Each edge must be a tuple of (u, v)." << std::endl;
				return;
			}

			// 获取节点 u 和 v
			auto u_ = edge_tuple[0];
			auto v_ = edge_tuple[1];

			// 检查 u 和 v 是否是整数类型
			if (!py::isinstance<py::int_>(u_) || !py::isinstance<py::int_>(v_)) {
				std::cout << "Error: Node IDs must be of type 'int'." << std::endl;
				return;
			}

			// 转换 u 和 v 为整数类型
			int u = u_.cast<int>();
			int v = v_.cast<int>();

			basic_remove_edge(u, v);
		}
		catch (const py::cast_error& e) {
			std::cout << "Error: Invalid edge format." << std::endl;
			return;
		}
	}
}

// 更新节点为行星点
void CGraph::set_node_as_planet(const py::object& node_) {
	int o = node_.cast<int>();

	// 判断节点是否存在
	if (m_node_map.find(o) == m_node_map.end()) {
		cout << "This node is not present in the diagram." << endl;
		return;
	}

	// 判断节点是否已经是行星点
	if (m_node_map[o]["planet_"] == 1) {
		cout << "The node is already a planetary point." << endl;
		return;
	}
	else {
		m_node_map[o]["planet_"] = 1;
	}

	// 修改节点：主要修改图G 
	// 修改出边
	if (node_out_list[o].size() != 0) {
		for (auto i : node_out_list[o]) {
			m_planet_start_map[o][i] = G[o][i];
		}
		G.erase(o);
	}
	// 修改入边
	if (node_in_list[o].size() != 0) {
		for (auto i : node_in_list[o]) {
			m_planet_end_map[o][i] = G[i][o];
			G[i].erase(o);
		}
	}
}

// 基本操作 ---------------------------------------------------------------------------------------
