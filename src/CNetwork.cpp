#include "CNetwork.h"

// 定义一个互斥锁
mutex result_mutex;


// 基础算法 ---------------------------------------------------------------------------------------
// 多源最短路算法
Dis_and_Path CGraph::multi_source_dijkstra(
	const vector<int>& sources,
	int target,
	double cutoff,
	string weight_name
) {
	// 输入参数检查
	if (sources.empty()) {
		throw invalid_argument("sources must not be empty");
	}

	for (const auto& s : sources) {
		if (G.find(s) == G.end()) {
			// 构建错误信息
			std::ostringstream oss;
			oss << "Node " << s << " not found in graph";

			throw out_of_range(oss.str());
		}
	}

	if (G.find(target) == G.end() && target != -1) {
		throw out_of_range("target node not found in graph");
	}

	// 检查目标是否是源节点之一
	for (const auto& s : sources) {
		if (s == target) {
			return { {{s, 0}}, {{s, {s}}} };
		}
	}

	// 初始化
	unordered_map<int, double> dist;
	unordered_map<int, int> pred;
	priority_queue<
		pair<double, int>,
		vector<pair<double, int>>,
		greater<>
	> pq;

	// 初始化源节点
	for (const auto& s : sources) {
		dist[s] = 0.0;
		pq.emplace(0.0, s);
		pred[s] = -1; // 表示源节点无前驱
	}

	// 遍历优先队列， 更新最短路径
	while (!pq.empty()) {
		std::pair<double, int> top = pq.top();
		pq.pop();
		double d = top.first;  // 获取距离
		int u = top.second;    // 获取节点

		// 跳过已处理的更优路径
		if (d > dist[u]) continue;

		// 提前终止条件
		if (u == target) break;
		if (d > cutoff) continue;

		// 遍历邻居并更新距离
		if (G.find(u) != G.end()) {
			for (const auto& pair : G.at(u)) {
				int v = pair.first;    // 获取邻接节点
				const unordered_map<string, double>& attributes = pair.second;  // 获取边的属性（权重）

				// 提取权重（假设键为 "weight"）
				double weight = 0.0;
				if (attributes.find(weight_name) != attributes.end()) {
					weight = attributes.at(weight_name);
				}
				else {
					weight = 1;
				}

				double new_dist = d + weight;
				// 发现更短路径
				if (dist.find(v) == dist.end() || new_dist < dist[v]) {
					dist[v] = new_dist;
					pred[v] = u;
					pq.emplace(new_dist, v);
				}
			}
		}
	}

	// 路径重建
	std::unordered_map<int, std::vector<int>> paths;
	auto build_path = [&](int node) {
		std::vector<int> path;
		if (pred.find(node) == pred.end()) return path;
		while (node != -1) {
			path.push_back(node);
			node = pred[node];
		}
		std::reverse(path.begin(), path.end());
		return path;
	};

	// 返回结果
	if (target != -1) {
		if (dist.find(target) == dist.end()) {
			throw std::runtime_error("No path to target");
		}
		paths[target] = build_path(target);
	}
	else {
		for (const auto& pair : dist) {
			int node = pair.first;  // 获取键 (节点)
			paths[node] = build_path(node);
		}
	}

	//返回最短路径和花费
	return { dist, paths };
}

// Dijkstra 算法（单源最短路径）
double CGraph::shortest_path_dijkstra(
	int source,
	int target,
	vector<int>& path,
	unordered_set<int>& ignore_nodes,
	const string& weight_name_) // 添加权重字段名称参数
{
	// 距离表
	unordered_map<int, double> dist;
	unordered_map<int, int> prev;
	priority_queue<pair<double, int>, vector<pair<double, int>>, greater<>> pq;

	dist[source] = 0;
	pq.push({ 0, source });

	while (!pq.empty()) {
		std::pair<double, int> top = pq.top();
		double d = top.first;
		int u = top.second;
		pq.pop();

		if (u == target) break; // 找到目标节点

		if (ignore_nodes.find(u) != ignore_nodes.end()) continue; // 如果是被忽略的节点，跳过

		// 遍历当前节点 u 的所有邻居节点
		for (auto& neighbor : G.at(u)) {
			int v = neighbor.first;
			if (ignore_nodes.find(v) != ignore_nodes.end()) continue; // 被忽略的边

			// 使用传入的 weight_name_ 来查找权重
			double weight = 1; // 默认权重为1
			auto it = neighbor.second.find(weight_name_);
			if (it != neighbor.second.end()) {
				weight = it->second; // 找到对应的权重
			}

			if (dist.find(v) == dist.end() || dist[v] > dist[u] + weight) {
				dist[v] = dist[u] + weight;
				prev[v] = u;
				pq.push({ dist[v], v });
			}
		}
	}

	// 构建路径
	path.clear();
	for (int at = target; at != source; at = prev[at]) {
		if (prev.find(at) == prev.end()) return numeric_limits<double>::infinity(); // 无路径
		path.push_back(at);
	}
	path.push_back(source);
	reverse(path.begin(), path.end());

	return dist[target];
}


// 基础算法 ---------------------------------------------------------------------------------------

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

// 结果计算 ---------------------------------------------------------------------------------------

// 多源最短路径计算：返回花费
unordered_map<int, double> CGraph::multi_source_cost(
	const py::object& list_o_,
	const py::object& method_,
	const py::object& target_,
	const py::object& cutoff_,
	const py::object& weight_name_)
{
	// 检查机制
	// 尝试转换 list_o_
	try {
		auto list_o = list_o_.cast<vector<int>>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: list_o must be of type 'list[int]'." << std::endl;
		return unordered_map<int, double>();
	}

	// 尝试转换 method_
	try {
		auto method = method_.cast<string>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: method must be of type 'string'." << std::endl;
		return unordered_map<int, double>();
	}

	// 尝试转换 target_
	try {
		auto target = target_.cast<int>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: target must be of type 'int'." << std::endl;
		return unordered_map<int, double>();
	}

	// 尝试转换 cutoff_
	try {
		auto cutoff = cutoff_.cast<double>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: cutoff must be of type 'double'." << std::endl;
		return unordered_map<int, double>();
	}

	// 尝试转换 weight_name_
	try {
		auto weight_name = weight_name_.cast<string>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: weight_name must be of type 'string'." << std::endl;
		return unordered_map<int, double>();
	}

	auto list_o = list_o_.cast<vector<int>>();
	auto method = method_.cast<string>();
	auto target = target_.cast<int>();
	auto cutoff = cutoff_.cast<double>();
	auto weight_name = weight_name_.cast<string>();

	// 逻辑执行
	if (method == "Dijkstra") {
		Dis_and_Path result = multi_source_dijkstra(list_o, target, cutoff, weight_name);
		return result.distances;
	}
	else {
		std::cout << "not have this method now." << std::endl;
		return unordered_map<int, double>();
	}

}

unordered_map<int, vector<int>> CGraph::multi_source_path(
	const py::object& list_o_,
	const py::object& method_,
	const py::object& target_,
	const py::object& cutoff_,
	const py::object& weight_name_)
{
	// 检查机制
	// 尝试转换 list_o_
	try {
		auto list_o = list_o_.cast<vector<int>>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: list_o must be of type 'list[int]'." << std::endl;
		return unordered_map<int, vector<int>>();
	}

	// 尝试转换 method_
	try {
		auto method = method_.cast<string>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: method must be of type 'string'." << std::endl;
		return unordered_map<int, vector<int>>();
	}

	// 尝试转换 target_
	try {
		auto target = target_.cast<int>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: target must be of type 'int'." << std::endl;
		return unordered_map<int, vector<int>>();
	}

	// 尝试转换 cutoff_
	try {
		auto cutoff = cutoff_.cast<double>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: cutoff must be of type 'double'." << std::endl;
		return unordered_map<int, vector<int>>();
	}

	// 尝试转换 weight_name_
	try {
		auto weight_name = weight_name_.cast<string>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: weight_name must be of type 'string'." << std::endl;
		return unordered_map<int, vector<int>>();
	}

	auto list_o = list_o_.cast<vector<int>>();
	auto method = method_.cast<string>();
	auto target = target_.cast<int>();
	auto cutoff = cutoff_.cast<double>();
	auto weight_name = weight_name_.cast<string>();

	// 逻辑执行
	Dis_and_Path result = multi_source_dijkstra(list_o, target, cutoff, weight_name);
	return result.paths;
}

Dis_and_Path CGraph::multi_source_all(
	const py::object& list_o_,
	const py::object& method_,
	const py::object& target_,
	const py::object& cutoff_,
	const py::object& weight_name_)
{
	// 检查机制
		// 尝试转换 list_o_
	try {
		auto list_o = list_o_.cast<vector<int>>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: list_o must be of type 'list[int]'." << std::endl;
		return Dis_and_Path();
	}

	// 尝试转换 method_
	try {
		auto method = method_.cast<string>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: method must be of type 'string'." << std::endl;
		return Dis_and_Path();
	}

	// 尝试转换 target_
	try {
		auto target = target_.cast<int>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: target must be of type 'int'." << std::endl;
		return Dis_and_Path();
	}

	// 尝试转换 cutoff_
	try {
		auto cutoff = cutoff_.cast<double>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: cutoff must be of type 'double'." << std::endl;
		return Dis_and_Path();
	}

	// 尝试转换 weight_name_
	try {
		auto weight_name = weight_name_.cast<string>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: weight_name must be of type 'string'." << std::endl;
		return Dis_and_Path();
	}
	auto list_o = list_o_.cast<vector<int>>();
	auto method = method_.cast<string>();
	auto target = target_.cast<int>();
	auto cutoff = cutoff_.cast<double>();
	auto weight_name = weight_name_.cast<string>();


	// 逻辑执行
	Dis_and_Path result = multi_source_dijkstra(list_o, target, cutoff, weight_name);
	return result;
}

// 单源最短路径计算
unordered_map<int, double> CGraph::single_source_cost(
	const py::object& o_,
	const py::object& method_,
	const py::object& target_,
	const py::object& cutoff_,
	const py::object& weight_name_) 
{
	// 检查机制
	// 尝试转换 list_o_
	try {
		auto o = o_.cast<int>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: list_o must be of type 'list[int]'." << std::endl;
		return unordered_map<int, double>();
	}

	// 尝试转换 method_
	try {
		auto method = method_.cast<string>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: method must be of type 'string'." << std::endl;
		return unordered_map<int, double>();
	}

	// 尝试转换 target_
	try {
		auto target = target_.cast<int>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: target must be of type 'int'." << std::endl;
		return unordered_map<int, double>();
	}

	// 尝试转换 cutoff_
	try {
		auto cutoff = cutoff_.cast<double>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: cutoff must be of type 'double'." << std::endl;
		return unordered_map<int, double>();
	}

	// 尝试转换 weight_name_
	try {
		auto weight_name = weight_name_.cast<string>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: weight_name must be of type 'string'." << std::endl;
		return unordered_map<int, double>();
	}

	auto o = o_.cast<int>();
	auto method = method_.cast<string>();
	auto target = target_.cast<int>();
	auto cutoff = cutoff_.cast<double>();
	auto weight_name = weight_name_.cast<string>();
	vector<int> list_o;
	list_o.push_back(o);

	// 逻辑执行
	if (method == "Dijkstra") {
		unordered_map<int, double> result;
		result = multi_source_dijkstra(list_o, target, cutoff, weight_name).distances;
		return result;
	}
	else {
		std::cout << "not have this method now." << std::endl;
		return unordered_map<int, double>();
	}
}

unordered_map<int, std::vector<int>> CGraph::single_source_path(
	const py::object& o_,
	const py::object& method_,
	const py::object& target_,
	const py::object& cutoff_,
	const py::object& weight_name_)
{
	// 检查机制
	// 尝试转换 list_o_
	try {
		auto o = o_.cast<int>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: list_o must be of type 'list[int]'." << std::endl;
		return unordered_map<int, vector<int>>();
	}

	// 尝试转换 method_
	try {
		auto method = method_.cast<string>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: method must be of type 'string'." << std::endl;
		return unordered_map<int, vector<int>>();
	}

	// 尝试转换 target_
	try {
		auto target = target_.cast<int>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: target must be of type 'int'." << std::endl;
		return unordered_map<int, vector<int>>();
	}

	// 尝试转换 cutoff_
	try {
		auto cutoff = cutoff_.cast<double>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: cutoff must be of type 'double'." << std::endl;
		return unordered_map<int, vector<int>>();
	}

	// 尝试转换 weight_name_
	try {
		auto weight_name = weight_name_.cast<string>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: weight_name must be of type 'string'." << std::endl;
		return unordered_map<int, vector<int>>();
	}

	auto o = o_.cast<int>();
	auto method = method_.cast<string>();
	auto target = target_.cast<int>();
	auto cutoff = cutoff_.cast<double>();
	auto weight_name = weight_name_.cast<string>();
	vector<int> list_o;
	list_o.push_back(o);

	// 逻辑执行
	if (method == "Dijkstra") {
		unordered_map<int, vector<int>> result;
		result = multi_source_dijkstra(list_o, target, cutoff, weight_name).paths;
		return result;
	}
	else {
		std::cout << "not have this method now." << std::endl;
		return unordered_map<int, vector<int>>();
	}
}

Dis_and_Path CGraph::single_source_all(
	const py::object& o_,
	const py::object& method_,
	const py::object& target_,
	const py::object& cutoff_,
	const py::object& weight_name_)
{
	// 检查机制
	// 尝试转换 list_o_
	try {
		auto o = o_.cast<int>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: list_o must be of type 'list[int]'." << std::endl;
		return Dis_and_Path();
	}

	// 尝试转换 method_
	try {
		auto method = method_.cast<string>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: method must be of type 'string'." << std::endl;
		return Dis_and_Path();
	}

	// 尝试转换 target_
	try {
		auto target = target_.cast<int>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: target must be of type 'int'." << std::endl;
		return Dis_and_Path();
	}

	// 尝试转换 cutoff_
	try {
		auto cutoff = cutoff_.cast<double>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: cutoff must be of type 'double'." << std::endl;
		return Dis_and_Path();
	}

	// 尝试转换 weight_name_
	try {
		auto weight_name = weight_name_.cast<string>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: weight_name must be of type 'string'." << std::endl;
		return Dis_and_Path();
	}

	auto o = o_.cast<int>();
	auto method = method_.cast<string>();
	auto target = target_.cast<int>();
	auto cutoff = cutoff_.cast<double>();
	auto weight_name = weight_name_.cast<string>();
	vector<int> list_o;
	list_o.push_back(o);

	// 逻辑执行
	if (method == "Dijkstra") {
		Dis_and_Path result = multi_source_dijkstra(list_o, target, cutoff, weight_name);
		return result;
	}
	else {
		std::cout << "not have this method now." << std::endl;
		return Dis_and_Path();
	}
}

// 多个单源最短路径计算
vector<unordered_map<int, double>> CGraph::multi_single_source_cost(
	const py::object& list_o_,
	const py::object& method_,
	const py::object& target_,
	const py::object& cutoff_,
	const py::object& weight_name_,
	const py::object& num_thread_
) {
	// 检查机制
	// 尝试转换 list_o_
	try {
		auto list_o = list_o_.cast<vector<int>>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: list_o must be of type 'list[int]'." << std::endl;
		return vector<unordered_map<int, double>>();
	}

	// 尝试转换 method_
	try {
		auto method = method_.cast<string>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: method must be of type 'string'." << std::endl;
		return vector<unordered_map<int, double>>();
	}

	// 尝试转换 target_
	try {
		auto target = target_.cast<int>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: target must be of type 'int'." << std::endl;
		return vector<unordered_map<int, double>>();
	}

	// 尝试转换 cutoff_
	try {
		auto cutoff = cutoff_.cast<double>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: cutoff must be of type 'double'." << std::endl;
		return vector<unordered_map<int, double>>();
	}

	// 尝试转换 weight_name_
	try {
		auto weight_name = weight_name_.cast<string>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: weight_name must be of type 'string'." << std::endl;
		return vector<unordered_map<int, double>>();
	}

	try {
		auto num_thread = num_thread_.cast<int>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: list_o must be of type 'list[int]'." << std::endl;
		return vector<unordered_map<int, double>>();
	}

	auto list_o = list_o_.cast<vector<int>>();
	auto method = method_.cast<string>();
	auto target = target_.cast<int>();
	auto cutoff = cutoff_.cast<double>();
	auto weight_name = weight_name_.cast<string>();
	auto num_thread = num_thread_.cast<int>();

	// 逻辑执行
	vector<unordered_map<int, double>> final_result(list_o.size());  // 初始化结果容器，大小为 list_o.size()
	final_result.reserve(list_o.size());
	vector<thread> threads;
	atomic<size_t> index(0);
	size_t max_threads = std::thread::hardware_concurrency();
	if (num_thread >= max_threads) num_thread = max_threads - 1;

	// 使用互斥锁来保护 final_result 的访问
	std::mutex result_mutex;

	while (index < list_o.size()) {
		// 启动最大数量的线程
		while (threads.size() < num_thread && index < list_o.size()) {
			threads.push_back(thread([&]() {
				size_t i = index++;  // 获取当前线程处理的节点索引
				if (i < list_o.size()) {
					// 每个线程处理一个节点
					vector<int> cur_list;
					cur_list.push_back(list_o[i]);
					unordered_map<int, double> result;

					// 使用给定的方法计算路径
					if (method == "Dijkstra") {
						result = multi_source_dijkstra(cur_list, target, cutoff, weight_name).distances;

						// 使用互斥锁保护对 final_result 的访问
						std::lock_guard<std::mutex> lock(result_mutex);
						final_result[i] = result;  // 确保结果顺序正确
					}
				}
			}));
		}

		// 等待线程池中的线程完成
		for (auto& t : threads) {
			if (t.joinable()) {
				t.join();
			}
		}
		threads.clear();
	}

	return final_result;
}

vector<unordered_map<int, vector<int>>> CGraph::multi_single_source_path(
	const py::object& list_o_,
	const py::object& method_,
	const py::object& target_,
	const py::object& cutoff_,
	const py::object& weight_name_,
	const py::object& num_thread_
) {
	// 检查机制
	// 尝试转换 list_o_
	try {
		auto list_o = list_o_.cast<vector<int>>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: list_o must be of type 'list[int]'." << std::endl;
		return vector<unordered_map<int, vector<int>>>();
	}

	// 尝试转换 method_
	try {
		auto method = method_.cast<string>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: method must be of type 'string'." << std::endl;
		return vector<unordered_map<int, vector<int>>>();
	}

	// 尝试转换 target_
	try {
		auto target = target_.cast<int>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: target must be of type 'int'." << std::endl;
		return vector<unordered_map<int, vector<int>>>();
	}

	// 尝试转换 cutoff_
	try {
		auto cutoff = cutoff_.cast<double>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: cutoff must be of type 'double'." << std::endl;
		return vector<unordered_map<int, vector<int>>>();
	}

	// 尝试转换 weight_name_
	try {
		auto weight_name = weight_name_.cast<string>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: weight_name must be of type 'string'." << std::endl;
		return vector<unordered_map<int, vector<int>>>();
	}

	try {
		auto num_thread = num_thread_.cast<int>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: num_thread must be of type 'int'." << std::endl;
		return vector<unordered_map<int, vector<int>>>();
	}

	auto list_o = list_o_.cast<vector<int>>();
	auto method = method_.cast<string>();
	auto target = target_.cast<int>();
	auto cutoff = cutoff_.cast<double>();
	auto weight_name = weight_name_.cast<string>();
	auto num_thread = num_thread_.cast<int>();

	// 逻辑执行
	vector<unordered_map<int, vector<int>>> final_result(list_o.size());  // 初始化 final_result 容器，大小与 list_o 相同
	vector<thread> threads;
	atomic<size_t> index(0);
	size_t max_threads = std::thread::hardware_concurrency();
	if (num_thread >= max_threads) num_thread = max_threads - 1;

	// 使用互斥锁来保护 final_result 的访问
	std::mutex result_mutex;

	while (index < list_o.size()) {
		// 启动最大数量的线程
		while (threads.size() < num_thread && index < list_o.size()) {
			threads.push_back(thread([&]() {
				size_t i = index++;  // 获取当前线程处理的节点索引
				if (i < list_o.size()) {
					// 每个线程处理一个节点
					vector<int> cur_list;
					cur_list.push_back(list_o[i]);
					if (method == "Dijkstra") {
						unordered_map<int, vector<int>> result = multi_source_dijkstra(cur_list, target, cutoff, weight_name).paths;

						// 使用互斥锁保护对 final_result 的访问
						std::lock_guard<std::mutex> lock(result_mutex);
						final_result[i] = result;  // 确保将结果存储在正确的索引位置
					}
				}
			}));
		}

		// 等待线程池中的线程完成
		for (auto& t : threads) {
			if (t.joinable()) {
				t.join();
			}
		}
		threads.clear();
	}

	return final_result;
}

vector<Dis_and_Path> CGraph::multi_single_source_all(
	const py::object& list_o_,
	const py::object& method_,
	const py::object& target_,
	const py::object& cutoff_,
	const py::object& weight_name_,
	const py::object& num_thread_
) {
	// 检查机制
	// 尝试转换 list_o_
	try {
		auto list_o = list_o_.cast<vector<int>>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: list_o must be of type 'list[int]'." << std::endl;
		return vector<Dis_and_Path>();
	}

	// 尝试转换 method_
	try {
		auto method = method_.cast<string>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: method must be of type 'string'." << std::endl;
		return vector<Dis_and_Path>();
	}

	// 尝试转换 target_
	try {
		auto target = target_.cast<int>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: target must be of type 'int'." << std::endl;
		return vector<Dis_and_Path>();
	}

	// 尝试转换 cutoff_
	try {
		auto cutoff = cutoff_.cast<double>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: cutoff must be of type 'double'." << std::endl;
		return vector<Dis_and_Path>();
	}

	// 尝试转换 weight_name_
	try {
		auto weight_name = weight_name_.cast<string>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: weight_name must be of type 'string'." << std::endl;
		return vector<Dis_and_Path>();
	}

	// 尝试转换 num_thread_
	try {
		auto num_thread = num_thread_.cast<int>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: num_thread must be of type 'int'." << std::endl;
		return vector<Dis_and_Path>();
	}

	auto list_o = list_o_.cast<vector<int>>();
	auto method = method_.cast<string>();
	auto target = target_.cast<int>();
	auto cutoff = cutoff_.cast<double>();
	auto weight_name = weight_name_.cast<string>();
	auto num_thread = num_thread_.cast<int>();

	// 逻辑执行
	vector<Dis_and_Path> final_result(list_o.size());  // 初始化 final_result 容器，大小与 list_o 相同
	vector<thread> threads;
	atomic<size_t> index(0);
	size_t max_threads = std::thread::hardware_concurrency();
	if (num_thread >= max_threads) num_thread = max_threads - 1;

	// 使用互斥锁来保护 final_result 的访问
	std::mutex result_mutex;

	while (index < list_o.size()) {
		// 启动最大数量的线程
		while (threads.size() < num_thread && index < list_o.size()) {
			threads.push_back(thread([&]() {
				size_t i = index++;  // 获取当前线程处理的节点索引
				if (i < list_o.size()) {
					// 每个线程处理一个节点
					vector<int> cur_list;
					cur_list.push_back(list_o[i]);

					// 执行 Dijkstra 或其他算法
					if (method == "Dijkstra") {
						Dis_and_Path result = multi_source_dijkstra(cur_list, target, cutoff, weight_name);

						// 使用互斥锁保护对 final_result 的访问
						std::lock_guard<std::mutex> lock(result_mutex);
						final_result[i] = result;  // 确保将结果存储在正确的索引位置
					}
				}
			}));
		}

		// 等待线程池中的线程完成
		for (auto& t : threads) {
			if (t.joinable()) {
				t.join();
			}
		}
		threads.clear();
	}

	return final_result;
}

// 多个多源最短路径计算
vector<unordered_map<int, double>> CGraph::multi_multi_source_cost(
	const py::object& list_o_,
	const py::object& method_,
	const py::object& target_,
	const py::object& cutoff_,
	const py::object& weight_name_,
	const py::object& num_thread_
) 
{
	// 检查机制
	// 尝试转换 list_o_
	try {
		auto list_o = list_o_.cast<vector<vector<int>>>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: list_o must be of type 'list[list[int]]'." << std::endl;
		return vector<unordered_map<int, double>>();
	}

	// 尝试转换 method_
	try {
		auto method = method_.cast<string>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: method must be of type 'string'." << std::endl;
		return vector<unordered_map<int, double>>();
	}

	// 尝试转换 target_
	try {
		auto target = target_.cast<int>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: target must be of type 'int'." << std::endl;
		return vector<unordered_map<int, double>>();
	}

	// 尝试转换 cutoff_
	try {
		auto cutoff = cutoff_.cast<double>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: cutoff must be of type 'double'." << std::endl;
		return vector<unordered_map<int, double>>();
	}

	// 尝试转换 weight_name_
	try {
		auto weight_name = weight_name_.cast<string>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: weight_name must be of type 'string'." << std::endl;
		return vector<unordered_map<int, double>>();
	}

	// 尝试转换 num_thread_
	try {
		auto num_thread = num_thread_.cast<int>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: num_thread must be of type 'int'." << std::endl;
		return vector<unordered_map<int, double>>();
	}

	auto list_o = list_o_.cast<vector<vector<int>>>();
	auto method = method_.cast<string>();
	auto target = target_.cast<int>();
	auto cutoff = cutoff_.cast<double>();
	auto weight_name = weight_name_.cast<string>();
	auto num_thread = num_thread_.cast<int>();

	// 逻辑执行
	vector<unordered_map<int, double>> final_result(list_o.size());  // 初始化 final_result 容器，大小与 list_o 相同
	vector<thread> threads;
	atomic<size_t> index(0);
	size_t max_threads = std::thread::hardware_concurrency();
	if (num_thread >= max_threads) num_thread = max_threads - 1;

	// 使用互斥锁来保护 final_result 的访问
	std::mutex result_mutex;

	while (index < list_o.size()) {
		// 启动最大数量的线程
		while (threads.size() < num_thread && index < list_o.size()) {
			threads.push_back(thread([&]() {
				size_t i = index++;  // 获取当前线程处理的节点索引
				if (i < list_o.size()) {
					// 每个线程处理一个节点
					vector<int> cur_list;
					cur_list = list_o[i];

					// 执行 Dijkstra 或其他算法
					if (method == "Dijkstra") {
						unordered_map<int, double> result = multi_source_dijkstra(cur_list, target, cutoff, weight_name).distances;

						// 使用互斥锁保护对 final_result 的访问
						std::lock_guard<std::mutex> lock(result_mutex);
						final_result[i] = result;  // 确保将结果存储在正确的索引位置
					}
				}
			}));
		}

		// 等待线程池中的线程完成
		for (auto& t : threads) {
			if (t.joinable()) {
				t.join();
			}
		}
		threads.clear();
	}

	return final_result;
}

vector<unordered_map<int, vector<int>>> CGraph::multi_multi_source_path(
	const py::object& list_o_,
	const py::object& method_,
	const py::object& target_,
	const py::object& cutoff_,
	const py::object& weight_name_,
	const py::object& num_thread_
) 
{
	// 检查机制
	// 尝试转换 list_o_
	try {
		auto list_o = list_o_.cast<vector<vector<int>>>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: list_o must be of type 'list[list[int]]'." << std::endl;
		return vector<unordered_map<int, vector<int>>>();
	}

	// 尝试转换 method_
	try {
		auto method = method_.cast<string>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: method must be of type 'string'." << std::endl;
		return vector<unordered_map<int, vector<int>>>();
	}

	// 尝试转换 target_
	try {
		auto target = target_.cast<int>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: target must be of type 'int'." << std::endl;
		return vector<unordered_map<int, vector<int>>>();
	}

	// 尝试转换 cutoff_
	try {
		auto cutoff = cutoff_.cast<double>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: cutoff must be of type 'double'." << std::endl;
		return vector<unordered_map<int, vector<int>>>();
	}

	// 尝试转换 weight_name_
	try {
		auto weight_name = weight_name_.cast<string>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: weight_name must be of type 'string'." << std::endl;
		return vector<unordered_map<int, vector<int>>>();
	}

	// 尝试转换 num_thread_
	try {
		auto num_thread = num_thread_.cast<int>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: num_thread must be of type 'int'." << std::endl;
		return vector<unordered_map<int, vector<int>>>();
	}

	auto list_o = list_o_.cast<vector<vector<int>>>();
	auto method = method_.cast<string>();
	auto target = target_.cast<int>();
	auto cutoff = cutoff_.cast<double>();
	auto weight_name = weight_name_.cast<string>();
	auto num_thread = num_thread_.cast<int>();

	// 逻辑执行
	vector<unordered_map<int, vector<int>>> final_result(list_o.size());  // 初始化 final_result 容器，大小与 list_o 相同
	vector<thread> threads;
	atomic<size_t> index(0);
	size_t max_threads = std::thread::hardware_concurrency();
	if (num_thread >= max_threads) num_thread = max_threads - 1;

	// 使用互斥锁来保护 final_result 的访问
	std::mutex result_mutex;

	while (index < list_o.size()) {
		// 启动最大数量的线程
		while (threads.size() < num_thread && index < list_o.size()) {
			threads.push_back(thread([&]() {
				size_t i = index++;  // 获取当前线程处理的节点索引
				if (i < list_o.size()) {
					// 每个线程处理一个节点
					vector<int> cur_list;
					cur_list = list_o[i];

					// 执行 Dijkstra 或其他算法
					if (method == "Dijkstra") {
						unordered_map<int, vector<int>> result = multi_source_dijkstra(cur_list, target, cutoff, weight_name).paths;

						// 使用互斥锁保护对 final_result 的访问
						std::lock_guard<std::mutex> lock(result_mutex);
						final_result[i] = result;  // 确保将结果存储在正确的索引位置
					}
				}
			}));
		}

		// 等待线程池中的线程完成
		for (auto& t : threads) {
			if (t.joinable()) {
				t.join();
			}
		}
		threads.clear();
	}

	return final_result;
}

vector<Dis_and_Path> CGraph::multi_multi_source_all(
	const py::object& list_o_,
	const py::object& method_,
	const py::object& target_,
	const py::object& cutoff_,
	const py::object& weight_name_,
	const py::object& num_thread_
)
{
	// 检查机制
	// 尝试转换 list_o_
	try {
		auto list_o = list_o_.cast<vector<vector<int>>>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: list_o must be of type 'list[list[int]]'." << std::endl;
		return vector<Dis_and_Path>();
	}

	// 尝试转换 method_
	try {
		auto method = method_.cast<string>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: method must be of type 'string'." << std::endl;
		return vector<Dis_and_Path>();
	}

	// 尝试转换 target_
	try {
		auto target = target_.cast<int>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: target must be of type 'int'." << std::endl;
		return vector<Dis_and_Path>();
	}

	// 尝试转换 cutoff_
	try {
		auto cutoff = cutoff_.cast<double>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: cutoff must be of type 'double'." << std::endl;
		return vector<Dis_and_Path>();
	}

	// 尝试转换 weight_name_
	try {
		auto weight_name = weight_name_.cast<string>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: weight_name must be of type 'string'." << std::endl;
		return vector<Dis_and_Path>();
	}

	// 尝试转换 num_thread_
	try {
		auto num_thread = num_thread_.cast<int>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: num_thread must be of type 'int'." << std::endl;
		return vector<Dis_and_Path>();
	}

	auto list_o = list_o_.cast<vector<vector<int>>>();
	auto method = method_.cast<string>();
	auto target = target_.cast<int>();
	auto cutoff = cutoff_.cast<double>();
	auto weight_name = weight_name_.cast<string>();
	auto num_thread = num_thread_.cast<int>();

	// 逻辑执行
	vector<Dis_and_Path> final_result(list_o.size());  // 初始化 final_result 容器，大小与 list_o 相同
	vector<thread> threads;
	atomic<size_t> index(0);
	size_t max_threads = std::thread::hardware_concurrency();
	if (num_thread >= max_threads) num_thread = max_threads - 1;

	// 使用互斥锁来保护 final_result 的访问
	std::mutex result_mutex;

	while (index < list_o.size()) {
		// 启动最大数量的线程
		while (threads.size() < num_thread && index < list_o.size()) {
			threads.push_back(thread([&]() {
				size_t i = index++;  // 获取当前线程处理的节点索引
				if (i < list_o.size()) {
					// 每个线程处理一个节点
					vector<int> cur_list;
					cur_list = list_o[i];

					// 执行 Dijkstra 或其他算法
					if (method == "Dijkstra") {
						Dis_and_Path result = multi_source_dijkstra(cur_list, target, cutoff, weight_name);

						// 使用互斥锁保护对 final_result 的访问
						std::lock_guard<std::mutex> lock(result_mutex);
						final_result[i] = result;  // 确保将结果存储在正确的索引位置
					}
				}
			}));
		}

		// 等待线程池中的线程完成
		for (auto& t : threads) {
			if (t.joinable()) {
				t.join();
			}
		}
		threads.clear();
	}

	return final_result;
}

py::array_t<double>  CGraph::cost_matrix_to_numpy(
	const py::object& starts_,
	const py::object& ends_,
	const py::object& method_,
	const py::object& cutoff_,
	const py::object& weight_name_,
	const py::object& num_thread_
) 
{
	// 输入检查
	// 尝试转换 starts_
	try {
		auto test_list = starts_.cast<vector<int>>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: starts_ must be of type 'list[int]'." << std::endl;
		return py::array_t<double>();
	}

	// 尝试转换 ends_
	try {
		auto test_list = ends_.cast<vector<int>>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: ends_ must be of type 'list[int]'." << std::endl;
		return py::array_t<double>();
	}

	// 尝试转换 method_
	try {
		auto method = method_.cast<string>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: method must be of type 'string'." << std::endl;
		return py::array_t<double>();
	}

	// 尝试转换 cutoff_
	try {
		auto cutoff = cutoff_.cast<double>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: cutoff must be of type 'double'." << std::endl;
		return py::array_t<double>();
	}

	// 尝试转换 weight_name_
	try {
		auto weight_name = weight_name_.cast<string>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: weight_name must be of type 'string'." << std::endl;
		return py::array_t<double>();
	}

	// 尝试转换 num_thread_
	try {
		auto num_thread = num_thread_.cast<int>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: num_thread must be of type 'int'." << std::endl;
		return py::array_t<double>();
	}


	// 逻辑运行
	// 获取起点列表和终点列表的大小
	auto starts = starts_.cast<vector<int>>();
	auto ends = ends_.cast<vector<int>>();
	size_t num_starts = starts.size();
	size_t num_ends = ends.size();

	// 创建一个二维数组来存储所有起点到终点的花费
	py::array_t<double> result({ num_starts, num_ends }); // 2D NumPy数组
	py::buffer_info buf_info = result.request();
	double* ptr = static_cast<double*>(buf_info.ptr); // 获取数组的指针

	py::object target_ = py::int_(-1);
	vector<vector<int>> multi_list_;
	for (auto i : starts) {
		vector<int> cur_vec{ i };
		multi_list_.push_back(cur_vec);
	}
	// 将 multi_list_ 转换成 py::object（这个已经是 py::object 类型）
	py::object multi_list_obj = py::cast(multi_list_);  // 直接使用 py::list
	

	vector<Dis_and_Path> multi_result =  multi_multi_source_all(multi_list_obj, method_, target_, cutoff_, weight_name_, num_thread_);
	// 填充cost matrix
	for (int i = 0; i < num_starts; ++i) {
		for (int j = 0; j < num_ends; ++j) {
			auto it = multi_result[i].distances.find(ends[j]);
			if (it != multi_result[i].distances.end()) {
				ptr[i * num_ends + j] = it->second;
			}
			else {
				// 如果找不到该键，您可以选择赋一个默认值，或者跳过
				ptr[i * num_ends + j] = -1; // 或者其他默认值
			}
		}
	}

	return result; // 返回NumPy数组
}

py::dict CGraph::path_list_to_numpy(
	const py::object& starts_,
	const py::object& ends_,
	const py::object& method_,
	const py::object& cutoff_,
	const py::object& weight_name_,
	const py::object& num_thread_
)
{
	// 输入检查
	// 尝试转换 starts_
	try {
		auto test_list = starts_.cast<vector<int>>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: starts_ must be of type 'list[int]'." << std::endl;
		return py::array_t<double>();
	}

	// 尝试转换 ends_
	try {
		auto test_list = ends_.cast<vector<int>>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: ends_ must be of type 'list[int]'." << std::endl;
		return py::array_t<double>();
	}

	// 尝试转换 method_
	try {
		auto method = method_.cast<string>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: method must be of type 'string'." << std::endl;
		return py::array_t<double>();
	}

	// 尝试转换 cutoff_
	try {
		auto cutoff = cutoff_.cast<double>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: cutoff must be of type 'double'." << std::endl;
		return py::array_t<double>();
	}

	// 尝试转换 weight_name_
	try {
		auto weight_name = weight_name_.cast<string>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: weight_name must be of type 'string'." << std::endl;
		return py::array_t<double>();
	}

	// 尝试转换 num_thread_
	try {
		auto num_thread = num_thread_.cast<int>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: num_thread must be of type 'int'." << std::endl;
		return py::array_t<double>();
	}


	// 逻辑运行
	// 获取起点列表和终点列表的大小
	auto starts = starts_.cast<vector<int>>();
	auto ends = ends_.cast<vector<int>>();
	size_t num_starts = starts.size();
	size_t num_ends = ends.size();

	// 创建一个字典来存储结果
	py::dict result;

	py::object target_ = py::int_(-1);
	vector<vector<int>> multi_list_;
	for (auto i : starts) {
		vector<int> cur_vec{ i };
		multi_list_.push_back(cur_vec);
	}
	py::object multi_list_obj = py::cast(multi_list_);

	vector<Dis_and_Path> multi_result = multi_multi_source_all(multi_list_obj, method_, target_, cutoff_, weight_name_, num_thread_);

	// 填充字典
	for (int i = 0; i < num_starts; ++i) {
		for (int j = 0; j < num_ends; ++j) {
			auto it = multi_result[i].paths.find(ends[j]);
			py::list path_list;

			if (it != multi_result[i].paths.end()) {
				auto cur_path = it->second;
				// 将路径加入到列表
				path_list.append(cur_path);
				result[py::make_tuple(starts[i], ends[j])] = path_list;  // 使用 (起点, 终点) 作为字典的键
			}
			else {
				// 如果没有找到路径，使用空列表
				result[py::make_tuple(starts[i], ends[j])] = py::list();
			}
		}
	}

	return result;  // 返回字典
}

// 查找最短路径
vector<vector<int>>  CGraph::shortest_simple_paths(
	const py::object& start_,
	const py::object& end_,
	const py::object& weight_name_)
{
	int start, end;
	string weight_name;

	// 输入检查
	try {
		start = start_.cast<int>();
		end = end_.cast<int>();
		weight_name = weight_name_.cast<string>();
	}
	catch (const py::cast_error& e) {
		std::cout << "Error: Invalid parameter type. start/end must be int, weight_name must be string." << std::endl;
		return vector<vector<int>>();
	}

	unordered_set<int> ignore_nodes;
	vector<vector<int>> all_paths;
	vector<int> prev_path;

	while (true) {
		vector<int> path;
		double length = shortest_path_dijkstra(start, end, path, ignore_nodes, weight_name);

		if (length == numeric_limits<double>::infinity()) break; // 没有更多路径


		// 排除已找到路径的部分
		if (!prev_path.empty()) {
			// 将路径添加到路径列表
			all_paths.push_back(path);
			for (size_t i = 1; i < prev_path.size(); ++i) {
				ignore_nodes.insert(prev_path[i]);
			}
		}
		prev_path = path;
	}

	if (all_paths.empty()) {
		cout << "No paths found from " << start << " to " << end << endl;
		return all_paths;
	}

	return all_paths;
}

// 结果计算 ----------------------------------------------------------------------------------------


// test -----------------------------------------------------------------------------------------------



//// 从csv加多条边
//Graph CGraph::add_edges_from_csv(string path) {
//	// 打开文件
//	ifstream file(path);
//	if (!file.is_open()) {
//		std::cerr << "Error opening file: " << path << std::endl;
//	}
//
//	string line;
//
//	// 读取标题行
//	getline(file, line);
//	stringstream headerStream(line);
//	string header;
//	vector<string> headers;
//	while (std::getline(headerStream, header, ',')) {
//		headers.push_back(header);
//	}
//
//	// 查找特定字段的索引
//	auto get_column_index = [&headers](const std::string& field) -> int {
//		auto it = std::find(headers.begin(), headers.end(), field);
//		return (it != headers.end()) ? std::distance(headers.begin(), it) : -1;
//	};
//
//	int from_node_idx = get_column_index("FROM_NODE");
//	int to_node_idx = get_column_index("TO_NODE");
//	int length_idx = get_column_index("length");
//	int dir_idx = get_column_index("DIR");
//
//	// 逐行读取数据
//	while (std::getline(file, line)) {
//		std::stringstream ss(line);
//		std::vector<std::string> values;
//		std::string value;
//
//		// 按逗号分割一行数据
//		while (std::getline(ss, value, ',')) {
//			values.push_back(value);
//		}
//
//		// 确保这一行有足够的字段数量
//		if (values.size() <= std::max({ from_node_idx, to_node_idx, length_idx, dir_idx })) {
//			std::cerr << "Skipping invalid row: " << line << std::endl;
//			continue;
//		}
//
//		// 提取所需字段的值
//		int from_node = std::stoi(values[from_node_idx]);
//		int to_node = std::stoi(values[to_node_idx]);
//		double length = std::stod(values[length_idx]);
//		int dir = std::stoi(values[dir_idx]);
//
//		// 根据方向字段决定如何添加路段
//		if (dir == 1) {
//			add_edge(from_node, to_node, length);
//		}
//		else if (dir == 0) {
//			add_edge(from_node, to_node, length);
//			add_edge(to_node, from_node, length);
//		}
//		else if (dir == -1) {
//			add_edge(to_node, from_node, length);
//		}
//	}
//
//	return G;
//}