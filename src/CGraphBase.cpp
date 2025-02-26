#include "CGraphBase.h"

// 定义一个互斥锁
mutex result_mutex;

// 基本操作 ---------------------------------------------------------------------------------------

// 获取基本信息
void CGraph::get_graph_info() {
	cout << "number of node: " << G.size() << endl;
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
	if (G.find(id_new) == G.end()) {
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
	if (G.find(start) == G.end()) {
		cout << "Error: Node " << start << " does not exist in the graph." << endl;
		return;
	}

	// 检查 end 节点是否存在
	if (G.find(end) == G.end()) {
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

// 加一条边
void CGraph::add_edge(const py::object& u_, const py::object& v_, const py::object& attribute_dict_) {
	// 检查机制
	// 检查 u 和 v 是否是整数类型
	if (!py::isinstance<py::int_>(u_) || !py::isinstance<py::int_>(v_)) {
		cout << "Error: Node IDs must be of type 'int'." << endl;
		return;
	}

	// 尝试转换 attribute_dict_
	try {
		auto attribute_dict = attribute_dict_.cast<std::unordered_map<std::string, double>>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: Attribute dictionary must be of type 'dict{string, float}'." << std::endl;
		return;
	}
	
	int u = u_.cast<int>();
	int v = v_.cast<int>();
	auto attribute_dict = attribute_dict_.cast<std::unordered_map<std::string, double>>();
	// 逻辑执行
	G[u][v] = attribute_dict;
	number_link += 1;
}

// 加多条边
void CGraph::add_edges(const py::list& edges_) {
	// 遍历每个边的三元组
	for (const auto& edge : edges_) {
		try {
			// 提取边的信息
			auto edge_tuple = edge.cast<py::tuple>();
			if (edge_tuple.size() != 3) {
				std::cout << "Error: Each edge must be a tuple of (u, v, attribute_dict)." << std::endl;
				return;
			}

			// 获取节点 u, v 和属性字典
			auto u_ = edge_tuple[0];
			auto v_ = edge_tuple[1];
			auto attribute_dict_ = edge_tuple[2];

			// 检查 u 和 v 是否是整数类型
			if (!py::isinstance<py::int_>(u_) || !py::isinstance<py::int_>(v_)) {
				std::cout << "Error: Node IDs must be of type 'int'." << std::endl;
				return;
			}

			// 尝试转换 attribute_dict_
			std::unordered_map<std::string, double> attribute_dict;
			try {
				attribute_dict = attribute_dict_.cast<std::unordered_map<std::string, double>>();
			}
			catch (const py::cast_error& e) {
				std::cout << "Error: Attribute dictionary must be of type 'dict{string, float}'." << std::endl;
				return;
			}

			// 转换节点 u 和 v 为整数类型
			int u = u_.cast<int>();
			int v = v_.cast<int>();

			// 逻辑执行：添加边
			G[u][v] = attribute_dict;
			number_link += 1;
		}
		catch (const py::cast_error& e) {
			std::cout << "Error: Invalid edge format." << std::endl;
			return;
		}
	}
}

// 删除一条边
void CGraph::remove_edge(const py::object& u_, const py::object& v_) {
	// 检查 u 和 v 是否是整数类型
	if (!py::isinstance<py::int_>(u_) || !py::isinstance<py::int_>(v_)) {
		std::cout << "Error: Node IDs must be of type 'int'." << std::endl;
		return;
	}

	// 转换 u 和 v 为整数类型
	int u = u_.cast<int>();
	int v = v_.cast<int>();

	// 检查图中是否存在这条边
	if (G.find(u) != G.end() && G[u].find(v) != G[u].end()) {
		// 删除边
		G[u].erase(v);
		number_link -= 1;
		std::cout << "Edge (" << u << ", " << v << ") removed successfully." << std::endl;
	}
	else {
		std::cout << "Error: Edge (" << u << ", " << v << ") does not exist." << std::endl;
	}
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

			// 检查图中是否存在这条边
			if (G.find(u) != G.end() && G[u].find(v) != G[u].end()) {
				// 删除边
				G[u].erase(v);
				number_link -= 1;
				std::cout << "Edge (" << u << ", " << v << ") removed successfully." << std::endl;
			}
			else {
				std::cout << "Error: Edge (" << u << ", " << v << ") does not exist." << std::endl;
			}
		}
		catch (const py::cast_error& e) {
			std::cout << "Error: Invalid edge format." << std::endl;
			return;
		}
	}
}

// 基本操作 ---------------------------------------------------------------------------------------
