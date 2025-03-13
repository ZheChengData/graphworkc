#include "GraphAlgorithms.h"

// 定义一个互斥锁
mutex result_mutex;

// 核心算法 ---------------------------------------------------------------------------------------

// 多源花费
unordered_map<int, double> GraphAlgorithms::multi_source_dijkstra_cost(
	const unordered_map<int, vector<pair<int, double>>>& g,
	const vector<int>& sources,
	int& target,
	double& cut_off,
	string& weight_name)
{
	unordered_map<int, double> dist;
	priority_queue<pair<double, int>, vector<pair<double, int>>, greater<>> pq;

	// 初始化源节点
	for (const auto& s : sources) {
		dist[s] = 0.0;
		pq.emplace(0.0, s);
	}

	while (!pq.empty()) {
		auto current = pq.top();
		double& d = current.first;
		int& u = current.second;
		pq.pop();

		if (d > dist[u]) continue;
		if (u == target) break;
		if (d > cut_off) continue;

		// 检查节点是否存在邻接表
		auto u_it = g.find(u);
		if (u_it == g.end()) continue;

		auto& neighbors = u_it->second;
		for (auto& edge : neighbors) {
			int v = edge.first;
			double weight = edge.second;

			double new_dist = d + weight;
			if (!dist.count(v) || new_dist < dist[v]) {
				dist[v] = new_dist;
				pq.emplace(new_dist, v);
			}
		}
	}

	return dist;
}


// 多源路径
unordered_map<int, vector<int>> GraphAlgorithms::multi_source_dijkstra_path(
	const unordered_map<int, vector<pair<int, double>>>& g,
	const vector<int>& sources,
	int target,
	double cut_off,
	string weight_name)
{
	unordered_map<int, vector<int>> paths;
	// 检查目标是否是源节点之一
	for (const auto& s : sources) {
		if (s == target) {
			return { {s, {s}} };
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
		pred[s] = -1;
		paths[s] = { s };
	}

	// 遍历优先队列， 更新最短路径
	while (!pq.empty()) {
		pair<double, int> top = pq.top();
		pq.pop();
		double d = top.first;  // 获取距离
		int u = top.second;    // 获取节点

		// 跳过已处理的更优路径
		if (d > dist[u]) continue;

		// 提前终止条件
		if (u == target) break;
		if (d > cut_off) continue;

		// 检查节点是否存在邻接表
		auto u_it = g.find(u);
		if (u_it == g.end()) continue;

		// 遍历邻居并更新距离
		auto& neighbors = u_it->second;
		for (const auto& edge : neighbors) {
			int v = edge.first;
			double weight = edge.second;

			double new_dist = d + weight;
			// 发现更短路径
			// 修改路径构建逻辑
			if (!dist.count(v) || new_dist < dist[v]) {
				dist[v] = new_dist;
				pred[v] = u;
				pq.emplace(new_dist, v);

				// 重构路径生成逻辑
				vector<int> new_path;
				if (pred[v] != -1) {
					new_path = paths[pred[v]];
				}
				new_path.push_back(v);
				paths[v] = new_path;
			}
		}
		
	}

	//返回序列路径
	return paths;
};


// 多源路径花费
dis_and_path GraphAlgorithms::multi_source_dijkstra(
	const unordered_map<int, vector<pair<int, double>>>& g,
	const vector<int>& sources,
	int target,
	double cut_off,
	string weight_name)
{
	// 检查目标是否是源节点之一
	for (const auto& s : sources) {
		if (s == target) {
			return { {{s, 0}}, {{s, {s}}} };
		}
	}
	unordered_map<int, vector<int>> paths;
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
		paths[s] = { s };
	}

	// 遍历优先队列， 更新最短路径
	while (!pq.empty()) {
		pair<double, int> top = pq.top();
		pq.pop();
		double d = top.first;  // 获取距离
		int u = top.second;    // 获取节点

		// 跳过已处理的更优路径
		if (d > dist[u]) continue;

		// 提前终止条件
		if (u == target) break;
		if (d > cut_off) continue;

		// 检查节点是否存在邻接表
		auto u_it = g.find(u);
		if (u_it == g.end()) continue;

		// 遍历邻居并更新距离
		auto& neighbors = u_it->second;
		for (const auto& edge : neighbors) {
			int v = edge.first;
			double weight = edge.second;

			double new_dist = d + weight;
			// 发现更短路径
			if (!dist.count(v) || new_dist < dist[v]) {
				dist[v] = new_dist;
				pred[v] = u;
				pq.emplace(new_dist, v);

				// 重构路径生成逻辑
				vector<int> new_path;
				if (pred[v] != -1) {
					new_path = paths[pred[v]];
				}
				new_path.push_back(v);
				paths[v] = new_path;
			}
		}
	}

	//返回最短路径和花费
	return { dist, paths };
}


// 多源路径花费形心点
unordered_map<int, double> GraphAlgorithms::multi_source_dijkstra_cost_centroid(
	const vector<int>& sources,
	int target,
	double cut_off,
	string weight_name)

{
	unordered_map<int, double> dist;
	priority_queue<pair<double, int>, vector<pair<double, int>>, greater<>> pq;

	// 初始化源节点
	for (const auto& s : sources) {
		dist[s] = 0.0;
		pq.emplace(0.0, s);
	}

	while (!pq.empty()) {
		auto current = pq.top();
		double d = current.first;
		int u = current.second;
		pq.pop();

		if (d > dist[u]) continue;
		if (u == target) break;
		if (d > cut_off) continue;

		// 修复：检查节点是否存在邻接表
		auto u_it = GTemp.find(u);
		if (u_it == GTemp.end()) continue;

		const auto& neighbors = u_it->second;
		for (const auto& edge : neighbors) {
			int v = edge.first;
			const auto& attrs = edge.second;

			// 提取权重
			double weight = 1.0;
			auto attr_it = attrs.find(weight_name);
			if (attr_it != attrs.end()) {
				weight = attr_it->second;
			}
			else {
				// 可选：抛出异常或记录日志
				// throw runtime_error("Weight '" + weight_name + "' missing");
			}

			double new_dist = d + weight;
			if (!dist.count(v) || new_dist < dist[v]) {
				dist[v] = new_dist;
				pq.emplace(new_dist, v);
			}
		}
	}

	return dist;
};


// 非全勤权重邻接字典获取
unordered_map<int, vector<pair<int, double>>> GraphAlgorithms::weight_reverse_func(
	string weight_name)
{
	unordered_map<int, vector<pair<int, double>>> res_G;
	for (auto& entry : G) {
		int u = entry.first;
		auto& edges = entry.second;
		for (auto& edge : edges) {
			int v = edge.first;
			auto& attrs = edge.second;
			double weight = 1.0;
			auto attr_it = attrs.find(weight_name);
			if (attr_it != attrs.end()) {
				weight = attr_it->second;
			}

			res_G[v].emplace_back(u, weight);
		}
	}

	return res_G;
}


// 非全勤权重前导字典获取
unordered_map<int, vector<pair<int, double>>> GraphAlgorithms::weight_func(
	string weight_name)
{
	unordered_map<int, vector<pair<int, double>>> res_G;
	for (auto& entry : G) {
		int u = entry.first;
		auto& edges = entry.second;
		for (auto& edge : edges) {
			int v = edge.first;
			auto& attrs = edge.second;
			double weight = 1.0;
			auto attr_it = attrs.find(weight_name);
			if (attr_it != attrs.end()) {
				weight = attr_it->second;
			}

			res_G[u].emplace_back(v, weight);
		}
	}

	return res_G;
}


// 获取正向权重
const unordered_map<int, vector<pair<int, double>>>&
GraphAlgorithms::get_weight_map(const string& weight_name)
{
	// 检查 weight_name 是否存在于 field_vec
	auto field_it = find(field_vec.begin(), field_vec.end(), weight_name);
	if (field_it != field_vec.end()) {
		// 直接返回 full_field_map 中对应位置的引用
		int field_index = distance(field_vec.begin(), field_it);
		return full_field_map[field_index]; // 返回常引用
	}
	else {
		// 若未找到，调用 weight_func 并返回其结果的引用（假设 weight_func 返回持久对象）
		static auto cached_map = weight_func(weight_name); // 静态缓存（可选）
		return cached_map; // 需根据 weight_func 的实际行为调整
	}
}


// 获取反向权重
const unordered_map<int, vector<pair<int, double>>>&
GraphAlgorithms::get_weight_reverse_map(const string& weight_name)
{
	// 检查 weight_name 是否存在于 field_vec
	auto field_it = find(field_vec.begin(), field_vec.end(), weight_name);
	if (field_it != field_vec.end()) {
		// 直接返回 full_field_map 中对应位置的引用
		int field_index = distance(field_vec.begin(), field_it);
		return full_field_reverse_map[field_index]; // 返回常引用
	}
	else {
		// 若未找到，调用 weight_func 并返回其结果的引用（假设 weight_func 返回持久对象）
		static auto cached_map = weight_reverse_func(weight_name); // 静态缓存（可选）
		return cached_map; // 需根据 weight_func 的实际行为调整
	}
}


// 构建反向图的邻接表
unordered_map<int, vector<pair<int, double>>> build_reverse_graph(
	const unordered_map<int, vector<pair<int, double>>>& g)
{
	unordered_map<int, vector<pair<int, double>>> reverse_g;
	for (auto it = g.begin(); it != g.end(); ++it) {
		int u = it->first;
		const auto& neighbors = it->second;  // 获取 u 的邻居

		for (auto jt = neighbors.begin(); jt != neighbors.end(); ++jt) {
			int v = jt->first;
			double w = jt->second;  // 获取 v 和权重 w
			reverse_g[v].emplace_back(u, w);  // 反向边：v ← u
		}
	}
	return reverse_g;
}


// 双向Dijkstra算法
dis_and_path GraphAlgorithms::bidirectional_dijkstra(
	const std::unordered_map<int, std::vector<std::pair<int, double>>>& reverse_g,
	const std::unordered_map<int, std::vector<std::pair<int, double>>>& g,
	const std::vector<int>& sources,
	int target,
	double cut_off)
{
	// 结果存储结构
	dis_and_path result;

	// 检查目标是否是源节点
	for (int s : sources) {
		if (s == target) {
			result.cost.emplace(s, 0.0);
			result.paths.emplace(s, std::vector<int>{s});
			return result;
		}
	}

	// 正向搜索数据结构
	std::unordered_map<int, double> dist_forward;
	std::unordered_map<int, int> pred_forward;
	std::priority_queue<std::pair<double, int>,
		std::vector<std::pair<double, int>>,
		std::greater<>> pq_forward;

	// 反向搜索数据结构
	std::unordered_map<int, double> dist_backward;
	std::unordered_map<int, int> pred_backward;
	std::priority_queue<std::pair<double, int>,
		std::vector<std::pair<double, int>>,
		std::greater<>> pq_backward;

	// 初始化正向搜索
	for (int s : sources) {
		if (g.count(s)) {
			dist_forward[s] = 0.0;
			pred_forward[s] = -1;
			pq_forward.emplace(0.0, s);
		}
	}

	// 初始化反向搜索
	dist_backward[target] = 0.0;
	pred_backward[target] = -1;
	pq_backward.emplace(0.0, target);

	// 最优路径跟踪
	double best_cost = std::numeric_limits<double>::max();
	int meet_node = -1;

	// 交替扩展策略
	while (!pq_forward.empty() && !pq_backward.empty()) {
		// 选择当前更小的队列扩展
		if (pq_forward.top().first <= pq_backward.top().first) {
			// 正向扩展
			auto top = pq_forward.top();
			double d = top.first;
			int u = top.second;
			pq_forward.pop();

			if (d > dist_forward[u]) continue;
			if (d > cut_off) continue;

			// 提前终止检查
			if (dist_backward.count(u) && (d + dist_backward[u] < best_cost)) {
				best_cost = d + dist_backward[u];
				meet_node = u;
			}

			auto it = g.find(u);
			if (it == g.end()) continue;

			for (const auto&pair : it->second) {
				auto v = pair.first;
				auto w = pair.second;
				const double new_dist = d + w;
				if (!dist_forward.count(v) || new_dist < dist_forward[v]) {
					dist_forward[v] = new_dist;
					pred_forward[v] = u;
					pq_forward.emplace(new_dist, v);
				}
			}
		}
		else {
			// 反向扩展
			auto top = pq_backward.top();
			double d = top.first;
			int u = top.second;

			pq_backward.pop();

			if (d > dist_backward[u]) continue;
			if (d > cut_off) continue;

			// 提前终止检查
			if (dist_forward.count(u) && (d + dist_forward[u] < best_cost)) {
				best_cost = d + dist_forward[u];
				meet_node = u;
			}

			auto it = reverse_g.find(u);
			if (it == reverse_g.end()) continue;

			for (const auto&pair : it->second) {
				auto v = pair.first;
				auto w = pair.second;
				const double new_dist = d + w;
				if (!dist_backward.count(v) || new_dist < dist_backward[v]) {
					dist_backward[v] = new_dist;
					pred_backward[v] = u;
					pq_backward.emplace(new_dist, v);
				}
			}
		}

		// 终止条件：当前最小距离之和超过已知最优
		if (pq_forward.top().first + pq_backward.top().first >= best_cost) {
			break;
		}
	}

	// 路径重构
	if (meet_node != -1) {
		// 正向路径回溯
		std::vector<int> forward_path;
		for (int u = meet_node; u != -1; u = pred_forward[u]) {
			forward_path.push_back(u);
		}
		std::reverse(forward_path.begin(), forward_path.end());

		// 反向路径回溯
		std::vector<int> backward_path;
		for (int u = meet_node; u != -1; u = pred_backward[u]) {
			backward_path.push_back(u);
		}

		// 合并路径
		forward_path.insert(forward_path.end(),
			backward_path.begin() + 1,
			backward_path.end());

		result.cost.emplace(target, best_cost);
		result.paths.emplace(target, forward_path);
	}
	else {
		result.cost.emplace(target, numeric_limits<double>::infinity());
		result.paths.emplace(target, std::vector<int>{});
	}

	return result;
}


// 双向Dijkstra算法 有ignore边
dis_and_path GraphAlgorithms::bidirectional_dijkstra_ignore(
	const unordered_map<int, vector<std::pair<int, double>>>& reverse_g,
	const unordered_map<int, vector<std::pair<int, double>>>& g,
	const vector<int>& sources,
	int target,
	double cut_off,
	const set<int>& ignore_nodes,
	const set<pair<int, int>>& ignore_edges)
{
	// 结果存储结构
	dis_and_path result;

	// 检查目标是否是源节点
	for (int s : sources) {
		if (s == target) {
			result.cost.emplace(s, 0.0);
			result.paths.emplace(s, std::vector<int>{s});
			return result;
		}
	}

	// 正向搜索数据结构
	std::unordered_map<int, double> dist_forward;
	std::unordered_map<int, int> pred_forward;
	std::priority_queue<std::pair<double, int>,
		std::vector<std::pair<double, int>>,
		std::greater<>> pq_forward;

	// 反向搜索数据结构
	std::unordered_map<int, double> dist_backward;
	std::unordered_map<int, int> pred_backward;
	std::priority_queue<std::pair<double, int>,
		std::vector<std::pair<double, int>>,
		std::greater<>> pq_backward;

	// 初始化正向搜索
	for (int s : sources) {
		if (g.count(s)) {
			dist_forward[s] = 0.0;
			pred_forward[s] = -1;
			pq_forward.emplace(0.0, s);
		}
	}

	// 初始化反向搜索
	dist_backward[target] = 0.0;
	pred_backward[target] = -1;
	pq_backward.emplace(0.0, target);

	// 最优路径跟踪
	double best_cost = std::numeric_limits<double>::max();
	int meet_node = -1;

	// 交替扩展策略
	while (!pq_forward.empty() && !pq_backward.empty()) {
		// 选择当前更小的队列扩展
		if (pq_forward.top().first <= pq_backward.top().first) {
			// 正向扩展
			auto top = pq_forward.top();
			double d = top.first;
			int u = top.second;
			pq_forward.pop();

			// 忽略已访问节点或被忽略的节点
			if (d > dist_forward[u] || ignore_nodes.count(u)) continue;
			if (d > cut_off) continue;

			// 提前终止检查
			if (dist_backward.count(u) && (d + dist_backward[u] < best_cost)) {
				best_cost = d + dist_backward[u];
				meet_node = u;
			}

			auto it = g.find(u);
			if (it == g.end()) continue;

			for (const auto& pair : it->second) {
				auto v = pair.first;
				auto w = pair.second;

				// 忽略被忽略的边（原图中的u→v）
				if (ignore_edges.count({ u, v })) continue;

				const double new_dist = d + w;
				if (!dist_forward.count(v) || new_dist < dist_forward[v]) {
					dist_forward[v] = new_dist;
					pred_forward[v] = u;
					pq_forward.emplace(new_dist, v);
				}
			}
		}
		else {
			// 反向扩展
			auto top = pq_backward.top();
			double d = top.first;
			int u = top.second;
			pq_backward.pop();

			// 忽略已访问节点或被忽略的节点
			if (d > dist_backward[u] || ignore_nodes.count(u)) continue;
			if (d > cut_off) continue;

			// 提前终止检查
			if (dist_forward.count(u) && (d + dist_forward[u] < best_cost)) {
				best_cost = d + dist_forward[u];
				meet_node = u;
			}

			auto it = reverse_g.find(u);
			if (it == reverse_g.end()) continue;

			for (const auto& pair : it->second) {
				auto v = pair.first;
				auto w = pair.second;

				// 忽略被忽略的边（原图中的v→u）
				if (ignore_edges.count({ v, u })) continue;

				const double new_dist = d + w;
				if (!dist_backward.count(v) || new_dist < dist_backward[v]) {
					dist_backward[v] = new_dist;
					pred_backward[v] = u;
					pq_backward.emplace(new_dist, v);
				}
			}
		}

		// 终止条件：当前最小距离之和超过已知最优，或任一队列为空
		if (pq_forward.empty() || pq_backward.empty()) {
			break;
		}
		if (pq_forward.top().first + pq_backward.top().first >= best_cost) {
			break;
		}
	}

	// 路径重构
	if (meet_node != -1) {
		// 正向路径回溯
		std::vector<int> forward_path;
		for (int u = meet_node; u != -1; u = pred_forward[u]) {
			forward_path.push_back(u);
		}
		std::reverse(forward_path.begin(), forward_path.end());

		// 反向路径回溯
		std::vector<int> backward_path;
		for (int u = meet_node; u != -1; u = pred_backward[u]) {
			backward_path.push_back(u);
		}

		// 合并路径（正向路径 + 反向路径[1:]）
		if (!backward_path.empty()) {
			forward_path.insert(forward_path.end(),
				backward_path.begin() + 1, backward_path.end());
		}

		result.cost.emplace(target, best_cost);
		result.paths.emplace(target, forward_path);
	}
	else {
		result.cost.emplace(target, std::numeric_limits<double>::infinity());
		result.paths.emplace(target, std::vector<int>{});
	}

	return result;
}


// 计算指定路径长度
double GraphAlgorithms::calculate_path_length(
	const unordered_map<int, vector<pair<int, double>>>& g,
	const vector<int>& path,
	const string& weight) {
	double len = 0;

	// 遍历路径中的每一对相邻节点 (u, v)
	for (size_t i = 0; i < path.size() - 1; ++i) {
		int u = path[i];
		int v = path[i + 1];

		// 在邻接表中查找边 (u, v) 并获取其权重
		const auto& neighbors = g.at(u); // 获取节点 u 的邻接列表
		for (const auto& neighbor : neighbors) {
			if (neighbor.first == v) { // 找到与 v 相连的边
				len += neighbor.second; // 加上边的权重
				break;
			}
		}
	}

	return len;
}


// 获取K条最短路径 K大于一定值时
vector<vector<int>> GraphAlgorithms::shortest_simple_paths_much(
	int source,
	int target,
	int K,
	const string& weight_name)
{
	// 1.节点检查
	if (G.find(source) == G.end()) {
		throw std::runtime_error("source node not in graph");
	}
	if (G.find(target) == G.end()) {
		throw std::runtime_error("target node not in graph");
	}

	// 2.初始化路径列表
	std::vector<std::vector<int>> listA; // 存储已找到的路径
	PathBuffer listB; // 存储候选路径
	std::vector<int> prev_path; // 上一条路径

	// 3.权重获取
	const auto& weight_map = get_weight_map(weight_name);
	const auto& reverse_map = get_weight_reverse_map(weight_name);
	auto cur_weight_map = weight_map;
	auto cur_reverse_map = reverse_map;

	// 4.主循环：寻找最短简单路径
	while (true) {

		if (prev_path.empty()) {
			// 如果 prev_path 是空，直接计算最短路径
			auto result = multi_source_dijkstra(weight_map, { source }, target, std::numeric_limits<double>::infinity(), weight_name);

			// 检查目标节点是否可达
			if (result.cost.find(target) != result.cost.end() && result.cost[target] < std::numeric_limits<double>::infinity()) {
				double length = result.cost[target];
				std::vector<int> path = result.paths[target];
				listB.push(length, path);
			}
			else {
				throw runtime_error("Target node is unreachable");
			}
		}
		else {
			std::set<int> ignore_nodes;
			std::set<pair<int, int>> ignore_edges;

			unordered_map<int, vector<pair<int, double>>> temp_g;
			unordered_map<int, vector<pair<int, double>>> temp_reverse_g;

			// 5.遍历前缀路径，更新 ignore_edges 和 ignore_nodes
			for (size_t i = 1; i < prev_path.size(); ++i) {
				std::vector<int> root(prev_path.begin(), prev_path.begin() + i);
				double root_length = calculate_path_length(weight_map, root, weight_name);

				// 遍历 listA，避免重复路径
				for (const auto& path : listA) {
					if (equal(root.begin(), root.end(), path.begin())) {
						ignore_edges.insert({ path[i - 1], path[i] });

						int u = path[i - 1];
						int v = path[i];

						// 更新 正向图
						if (weight_map.find(u) != weight_map.end()) {
							auto& adj = cur_weight_map[u];
							for (auto it = adj.begin(); it != adj.end(); ) {
								if (it->first == v) {
									// 将边 (u, v) 从 cur_weight_map 删除之前，先将其添加到 temp_G
									double weight = it->second;  // 获取边的权重

									// 检查 temp_G[u] 是否存在，如果不存在则创建一个空的 vector
									if (temp_g.find(u) == temp_g.end()) {
										temp_g[u] = vector<pair<int, double>>();  // 创建一个空的邻接表
									}

									// 将被删除的边 (u, v) 和权重添加到 temp_G 中
									temp_g[u].push_back({ v, weight });

									it = adj.erase(it);  // 删除并更新迭代器
								}
								else {
									++it;  // 移动到下一个元素
								}
							}
						}

						// 更新 cur_reverse_map（反向图）
						if (reverse_map.find(v) != reverse_map.end()) {
							auto& adj_rev = cur_reverse_map[v];
							for (auto it = adj_rev.begin(); it != adj_rev.end(); ) {
								if (it->first == u) {
									// 将边 (v, u) 从 cur_reverse_map 删除之前，先将其添加到 temp_G
									double weight = it->second;  // 获取边的权重

									// 检查 temp_G[v] 是否存在，如果不存在则创建一个空的 vector
									if (temp_reverse_g.find(v) == temp_reverse_g.end()) {
										temp_reverse_g[v] = vector<pair<int, double>>();  // 创建一个空的邻接表
									}

									// 将被删除的边 (v, u) 和权重添加到 temp_G 中
									temp_reverse_g[v].push_back({ u, weight });

									it = adj_rev.erase(it);  // 删除并更新迭代器
								}
								else {
									++it;  // 移动到下一个元素
								}
							}
						}
					}
				}

				// 计算 spur path
				try {
					auto result = bidirectional_dijkstra(
						cur_reverse_map,
						cur_weight_map,
						{ root.back() },
						target,
						numeric_limits<double>::infinity());

					// 检查目标节点是否可达
					if (result.cost.find(target) != result.cost.end() && result.cost[target] < std::numeric_limits<double>::infinity()) {
						double length = result.cost[target];
						vector<int> spur = result.paths[target];

						// 组合路径
						vector<int> impact_path = root;
						impact_path.insert(impact_path.end(), spur.begin() + 1, spur.end());
						listB.push(root_length + length, impact_path);
					}
					else {
					}
				}
				catch (const std::exception& e) {
				}

				for (const auto& pair : cur_weight_map[root.back()]) {
					temp_g[root.back()].push_back(pair);
				}
				for (const auto& pair : cur_reverse_map[root.back()]) {
					temp_reverse_g[root.back()].push_back(pair);
				}

				cur_weight_map.erase(root.back());
				cur_reverse_map.erase(root.back());
				ignore_nodes.insert(root.back());
			}
			// 回溯移除的边和点
			// 将 temp_G 中的元素合并到 cur_weight_map 中
			for (const auto& pair : temp_g) {
				// 对于 temp_G 中的每一个键值对，如果 cur_weight_map 中已经存在相同的键，合并其值
				cur_weight_map[pair.first].insert(cur_weight_map[pair.first].end(), pair.second.begin(), pair.second.end());
			}
			// 将 temp_reverse_g 中的元素合并到 cur_reverse_map 中
			for (const auto& pair : temp_reverse_g) {
				// 对于 temp_G 中的每一个键值对，如果 cur_reverse_map 中已经存在相同的键，合并其值
				cur_reverse_map[pair.first].insert(cur_reverse_map[pair.first].end(), pair.second.begin(), pair.second.end());
			}
		}

		// 从 listB 中取出最短路径
		if (!listB.empty()) {
			vector<int> path = listB.pop();
			listA.push_back(path);
			prev_path = path; // 更新 prev_path
		}
		else {
			break; // 没有更多路径可找，退出循环
		}

		// 判断是否已找到 K 条路径
		if (listA.size() >= K) {
			break; // 已找到 K 条路径，提前返回
		}
	}

	return vector<vector<int>>(listA.begin(), listA.begin() + K);
}


// 获取K条最短路径 K小于一定值时
vector<vector<int>> GraphAlgorithms::shortest_simple_paths_few(
	int source,
	int target,
	int K,
	const string& weight_name)
{
	// 1.节点检查
	if (G.find(source) == G.end()) {
		throw std::runtime_error("source node not in graph");
	}
	if (G.find(target) == G.end()) {
		throw std::runtime_error("target node not in graph");
	}

	// 2.初始化路径列表
	vector<vector<int>> listA; // 存储已找到的路径
	PathBuffer listB; // 存储候选路径
	vector<int> prev_path; // 上一条路径

	// 3.权重获取
	const auto& weight_map = get_weight_map(weight_name);
	const auto& reverse_map = get_weight_reverse_map(weight_name);

	double finale_time = 0.0;
	// 4.主循环：寻找最短简单路径
	while (true) {
		if (prev_path.empty()) {
			// 第一次最短路获取
			auto result = multi_source_dijkstra(
				weight_map,
				{ source },
				target,
				std::numeric_limits<double>::infinity(),
				weight_name);

			// 检查目标节点是否可达
			if (result.cost.find(target) != result.cost.end() && result.cost[target] < std::numeric_limits<double>::infinity()) {
				double length = result.cost[target];
				std::vector<int> path = result.paths[target];
				listB.push(length, path);
			}
			else {
				throw runtime_error("Target node is unreachable");
			}
		}
		else {
			set<int> ignore_nodes;
			set<pair<int, int>> ignore_edges;

			// 5.遍历前缀路径，更新 ignore_edges 和 ignore_nodes
			for (size_t i = 1; i < prev_path.size(); ++i) {
				vector<int> root(prev_path.begin(), prev_path.begin() + i);
				double root_length = calculate_path_length(weight_map, root, weight_name);

				// 遍历 listA，避免重复路径
				for (const auto& path : listA) {
					if (equal(root.begin(), root.end(), path.begin())) {
						ignore_edges.insert({ path[i - 1], path[i] });
					}
				}

				// 计算 spur path
				try {
					// 双向Dijkstra计算最短路径 
					auto result = bidirectional_dijkstra_ignore(
						reverse_map,
						weight_map,
						{ root.back() },
						target,
						numeric_limits<double>::infinity(),
						ignore_nodes,
						ignore_edges);

					if (result.cost.find(target) != result.cost.end() && result.cost[target] < numeric_limits<double>::infinity()) {
						double length = result.cost[target];
						vector<int> spur = result.paths[target];

						// 组合路径
						vector<int> impact_path = root;
						impact_path.insert(impact_path.end(), spur.begin() + 1, spur.end());
						listB.push(root_length + length, impact_path);
					}
					else {
					}
				}
				catch (const exception& e) {
				}
				ignore_nodes.insert(root.back());
			}

		}


		// 从 listB 中取出最短路径
		if (!listB.empty()) {
			vector<int> path = listB.pop();
			listA.push_back(path);
			prev_path = path;
		}
		else {
			break;
		}

		// 判断是否已找到 K 条路径
		if (listA.size() >= K) {
			break;
		}
	}

	return vector<vector<int>>(listA.begin(), listA.begin() + min(static_cast<size_t>(K), listA.size()));
}


// 调用方法 ---------------------------------------------------------------------------------------


// 多源最短路径计算
unordered_map<int, double> GraphAlgorithms::multi_source_cost(
	const py::object& list_o_,
	const py::object& method_,
	const py::object& target_,
	const py::object& cut_off_,
	const py::object& weight_name_)
{
	auto list_o = list_o_.cast<vector<int>>();
	auto method = method_.cast<string>();
	auto target = target_.cast<int>();
	auto cut_off = cut_off_.cast<double>();
	auto weight_name = weight_name_.cast<std::string>();

	const auto& weight_map = get_weight_map(weight_name);
	// 逻辑执行
	if (method == "Dijkstra") {
		unordered_map<int, double> result = multi_source_dijkstra_cost(weight_map, list_o, target, cut_off, weight_name);
		return result;
	}
}


unordered_map<int, vector<int>> GraphAlgorithms::multi_source_path(
	const py::object& list_o_,
	const py::object& method_,
	const py::object& target_,
	const py::object& cut_off_,
	const py::object& weight_name_)
{
	
	auto list_o = list_o_.cast<vector<int>>();
	auto method = method_.cast<string>();
	auto target = target_.cast<int>();
	auto cut_off = cut_off_.cast<double>();
	auto weight_name = weight_name_.cast<string>();

	const auto& weight_map = get_weight_map(weight_name);
	// 逻辑执行
	if (method == "Dijkstra") {
		// 逻辑执行
		unordered_map<int, vector<int>> result = multi_source_dijkstra_path(weight_map, list_o, target, cut_off, weight_name);
		return result;
	}
}


dis_and_path GraphAlgorithms::multi_source_all(
	const py::object& list_o_,
	const py::object& method_,
	const py::object& target_,
	const py::object& cut_off_,
	const py::object& weight_name_)
{
	auto list_o = list_o_.cast<vector<int>>();
	auto method = method_.cast<string>();
	auto target = target_.cast<int>();
	auto cut_off = cut_off_.cast<double>();
	auto weight_name = weight_name_.cast<string>();

	const auto& weight_map = get_weight_map(weight_name);
	// 逻辑执行
	if (method == "Dijkstra") {
		dis_and_path result = multi_source_dijkstra(weight_map, list_o, target, cut_off, weight_name);
		return result;
	}
}


// 单源最短路径计算
unordered_map<int, double> GraphAlgorithms::single_source_cost(
	const py::object& o_,
	const py::object& method_,
	const py::object& target_,
	const py::object& cut_off_,
	const py::object& weight_name_)
{
	auto o = o_.cast<int>();
	auto method = method_.cast<string>();
	auto target = target_.cast<int>();
	auto cut_off = cut_off_.cast<double>();
	auto weight_name = weight_name_.cast<string>();
	vector<int> list_o;
	list_o.push_back(o);

	const auto& weight_map = get_weight_map(weight_name);

	// 逻辑执行
	if (method == "Dijkstra") {
		unordered_map<int, double> result;
		result = multi_source_dijkstra_cost(weight_map,list_o, target, cut_off, weight_name);
		return result;
	}
}


unordered_map<int, std::vector<int>> GraphAlgorithms::single_source_path(
	const py::object& o_,
	const py::object& method_,
	const py::object& target_,
	const py::object& cut_off_,
	const py::object& weight_name_)
{
	auto o = o_.cast<int>();
	auto method = method_.cast<string>();
	auto target = target_.cast<int>();
	auto cut_off = cut_off_.cast<double>();
	auto weight_name = weight_name_.cast<string>();
	vector<int> list_o;
	list_o.push_back(o);

	const auto& weight_map = get_weight_map(weight_name);
	// 逻辑执行
	if (method == "Dijkstra") {
		unordered_map<int, vector<int>> result = multi_source_dijkstra_path(weight_map, list_o, target, cut_off, weight_name);
		return result;
	}
}


dis_and_path GraphAlgorithms::single_source_all(
	const py::object& o_,
	const py::object& method_,
	const py::object& target_,
	const py::object& cut_off_,
	const py::object& weight_name_)
{
	auto o = o_.cast<int>();
	auto method = method_.cast<string>();
	auto target = target_.cast<int>();
	auto cut_off = cut_off_.cast<double>();
	auto weight_name = weight_name_.cast<string>();
	vector<int> list_o;
	list_o.push_back(o);

	const auto& weight_map = get_weight_map(weight_name);
	// 逻辑执行
	if (method == "Dijkstra") {
		dis_and_path result = multi_source_dijkstra(weight_map, list_o, target, cut_off, weight_name);
		return result;
	}
}


// 多个单源最短路径计算
vector<unordered_map<int, double>> GraphAlgorithms::multi_single_source_cost(
	const py::object& list_o_,
	const py::object& method_,
	const py::object& target_,
	const py::object& cut_off_,
	const py::object& weight_name_,
	const py::object& num_thread_) {
	auto list_o = list_o_.cast<vector<int>>();
	auto method = method_.cast<string>();
	auto target = target_.cast<int>();
	auto cut_off = cut_off_.cast<double>();
	auto weight_name = weight_name_.cast<string>();
	auto num_thread = num_thread_.cast<int>();

	const auto& weight_map = get_weight_map(weight_name);
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
						result = multi_source_dijkstra_cost(weight_map, cur_list, target, cut_off, weight_name);

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


vector<unordered_map<int, vector<int>>> GraphAlgorithms::multi_single_source_path(
	const py::object& list_o_,
	const py::object& method_,
	const py::object& target_,
	const py::object& cut_off_,
	const py::object& weight_name_,
	const py::object& num_thread_) {
	auto list_o = list_o_.cast<vector<int>>();
	auto method = method_.cast<string>();
	auto target = target_.cast<int>();
	auto cut_off = cut_off_.cast<double>();
	auto weight_name = weight_name_.cast<string>();
	auto num_thread = num_thread_.cast<int>();

	const auto& weight_map = get_weight_map(weight_name);
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
						unordered_map<int, vector<int>> result = multi_source_dijkstra_path(weight_map, cur_list, target, cut_off, weight_name);

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


vector<dis_and_path> GraphAlgorithms::multi_single_source_all(
	const py::object& list_o_,
	const py::object& method_,
	const py::object& target_,
	const py::object& cut_off_,
	const py::object& weight_name_,
	const py::object& num_thread_)
{
	auto list_o = list_o_.cast<vector<int>>();
	auto method = method_.cast<string>();
	auto target = target_.cast<int>();
	auto cut_off = cut_off_.cast<double>();
	auto weight_name = weight_name_.cast<string>();
	auto num_thread = num_thread_.cast<int>();

	const auto& weight_map = get_weight_map(weight_name);
	// 逻辑执行
	vector<dis_and_path> final_result(list_o.size());  // 初始化 final_result 容器，大小与 list_o 相同
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
						dis_and_path result = multi_source_dijkstra(weight_map, cur_list, target, cut_off, weight_name);

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
vector<unordered_map<int, double>> GraphAlgorithms::multi_multi_source_cost(
	const py::object& list_o_,
	const py::object& method_,
	const py::object& target_,
	const py::object& cut_off_,
	const py::object& weight_name_,
	const py::object& num_thread_)
{
	auto list_o = list_o_.cast<vector<vector<int>>>();
	auto method = method_.cast<string>();
	auto target = target_.cast<int>();
	auto cut_off = cut_off_.cast<double>();
	auto weight_name = weight_name_.cast<string>();
	auto num_thread = num_thread_.cast<int>();

	const auto& weight_map = get_weight_map(weight_name);
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
						unordered_map<int, double> result = multi_source_dijkstra_cost(weight_map, cur_list, target, cut_off, weight_name);

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


vector<unordered_map<int, vector<int>>> GraphAlgorithms::multi_multi_source_path(
	const py::object& list_o_,
	const py::object& method_,
	const py::object& target_,
	const py::object& cut_off_,
	const py::object& weight_name_,
	const py::object& num_thread_)
{	
	auto list_o = list_o_.cast<vector<vector<int>>>();
	auto method = method_.cast<string>();
	auto target = target_.cast<int>();
	auto cut_off = cut_off_.cast<double>();
	auto weight_name = weight_name_.cast<string>();
	auto num_thread = num_thread_.cast<int>();

	const auto& weight_map = get_weight_map(weight_name);
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
						unordered_map<int, vector<int>> result = multi_source_dijkstra_path(weight_map, cur_list, target, cut_off, weight_name);

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


vector<dis_and_path> GraphAlgorithms::multi_multi_source_all(
	const py::object& list_o_,
	const py::object& method_,
	const py::object& target_,
	const py::object& cut_off_,
	const py::object& weight_name_,
	const py::object& num_thread_)
{
	auto list_o = list_o_.cast<vector<vector<int>>>();
	auto method = method_.cast<string>();
	auto target = target_.cast<int>();
	auto cut_off = cut_off_.cast<double>();
	auto weight_name = weight_name_.cast<string>();
	auto num_thread = num_thread_.cast<int>();

	const auto& weight_map = get_weight_map(weight_name);
	// 逻辑执行
	vector<dis_and_path> final_result(list_o.size());  // 初始化 final_result 容器，大小与 list_o 相同
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
						dis_and_path result = multi_source_dijkstra(weight_map, cur_list, target, cut_off, weight_name);

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


// 花费矩阵
py::array_t<double>  GraphAlgorithms::cost_matrix_to_numpy(
	const py::object& starts_,
	const py::object& ends_,
	const py::object& method_,
	const py::object& cut_off_,
	const py::object& weight_name_,
	const py::object& num_thread_)
{	
	// 逻辑运行
	GTemp = G;
	// 获取起点列表和终点列表及其大小
	auto starts = starts_.cast<vector<int>>();
	auto ends = ends_.cast<vector<int>>();
	auto weight_name = weight_name_.cast<string>();
	auto num_thread = num_thread_.cast<int>();
	size_t num_starts = starts.size();
	size_t num_ends = ends.size();

	// 将行星点加入临时图
	for (auto i : starts) {
		if (m_node_map[i]["centroid_"] == 1) {
			GTemp[i] = m_centroid_start_map[i];
		}
	}

	// 创建一个二维数组来存储所有起点到终点的花费
	py::array_t<double> result({ num_starts, num_ends });
	py::buffer_info buf_info = result.request();
	double* ptr = static_cast<double*>(buf_info.ptr);

	py::object target_ = py::int_(-1);
	vector<vector<int>> multi_list_;

	// 这里根据num_thread来分批处理
	size_t num_batches = (num_starts + num_thread - 1) / num_thread;  // 计算批次数

	// 循环每个批次
	for (size_t batch_idx = 0; batch_idx < num_batches; ++batch_idx) {
		// 计算当前批次的起点范围
		size_t start_idx = batch_idx * num_thread;
		size_t end_idx = min((batch_idx + 1) * num_thread, num_starts);

		// 生成当前批次的multi_list_
		multi_list_.clear();
		for (size_t i = start_idx; i < end_idx; ++i) {
			vector<int> cur_vec{ starts[i] };
			multi_list_.push_back(cur_vec);
		}

		// 转换成 py::object（已经是 py::list 类型）
		py::object multi_list_obj = py::cast(multi_list_);

		// 计算当前批次的多源最短路径
		vector<unordered_map<int, double>> multi_result = multi_multi_source_cost_centroid(multi_list_obj, method_, target_, cut_off_, weight_name_, num_thread_);

		// 填充当前批次的 cost matrix
		for (size_t i = start_idx; i < end_idx; ++i) {
			for (size_t j = 0; j < num_ends; ++j) {
				// 如果起点等于终点，直接返回0
				if (starts[i] == ends[j]) {
					ptr[i * num_ends + j] = 0;
					continue; 
				}

				// 如果终点是行星点
				if (m_node_map[ends[j]]["centroid_"] != 1) {
					auto it = multi_result[i - start_idx].find(ends[j]);
					if (it != multi_result[i - start_idx].end()) {
						ptr[i * num_ends + j] = it->second;
					}
					else {
						ptr[i * num_ends + j] = -1; // 默认值
					}
				}

				// 如果终点不是行星点
				else {
					if (m_centroid_end_map[ends[j]].size() == 0) {
						ptr[i * num_ends + j] = -1;
					}
					else {
						double minest_cost = numeric_limits<double>::infinity();
						// 遍历前导图
						for (const auto& pair : m_centroid_end_map[ends[j]]) {
							// 1. 判断 pair.second[weight_name] 是否存在
							const auto& weight_it = pair.second.find(weight_name);
							const double weight_value = (weight_it != pair.second.end()) ? weight_it->second : 1.0;

							// 2. 判断 multi_result[i][pair.first] 是否存在
							const auto& result_it = multi_result[i - start_idx].find(pair.first);
							if (result_it == multi_result[i - start_idx].end()) {
								continue; // 跳过本次循环
							}

							// 3. 计算当前成本
							const double cur_cost = weight_value + result_it->second;
							minest_cost = std::min(minest_cost, cur_cost);
						}
						// 最终赋值逻辑（需处理全跳过的边界情况）
						ptr[i * num_ends + j] = (minest_cost != std::numeric_limits<double>::infinity()) ? minest_cost : -1;
					}
				}
			}
		}
	}

	return result; // 返回NumPy数组
}


// 路径字典
py::dict GraphAlgorithms::path_list_to_numpy(
	const py::object& starts_,
	const py::object& ends_,
	const py::object& method_,
	const py::object& cut_off_,
	const py::object& weight_name_,
	const py::object& num_thread_)
{
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

	vector<unordered_map<int, vector<int>>> multi_result = multi_multi_source_path(multi_list_obj, method_, target_, cut_off_, weight_name_, num_thread_);

	// 填充字典
	for (int i = 0; i < num_starts; ++i) {
		for (int j = 0; j < num_ends; ++j) {
			auto it = multi_result[i].find(ends[j]);
			py::list path_list;

			if (it != multi_result[i].end()) {
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


// 多个多源最短花费(带形心)
vector<unordered_map<int, double>> GraphAlgorithms::multi_multi_source_cost_centroid(
	const py::object& list_o_,
	const py::object& method_,
	const py::object& target_,
	const py::object& cut_off_,
	const py::object& weight_name_,
	const py::object& num_thread_)
{
	auto list_o = list_o_.cast<vector<vector<int>>>();
	auto method = method_.cast<string>();
	auto target = target_.cast<int>();
	auto cut_off = cut_off_.cast<double>();
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
						unordered_map<int, double> result = multi_source_dijkstra_cost_centroid(cur_list, target, cut_off, weight_name);

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


// 获取K条最短路径 
vector<vector<int>> GraphAlgorithms::shortest_paths(
	const py::object& source_,
	const py::object& target_,
	const py::object& num_k_,
	const py::object& weight_name_)
{
	auto source = source_.cast<int>();
	auto target = target_.cast<int>();
	auto num_k = num_k_.cast<int>();
	auto weight_name = weight_name_.cast<string>();

	return(shortest_simple_paths_few(source, target, num_k, weight_name));
}

// test -------------------------------------------------------------------------------------------
unordered_map<int, double> GraphAlgorithms::test1(
	const vector<int>& sources,
	int target,
	double cut_off,
	string weight_name)
{
	unordered_map<int, double> dist;
	priority_queue<pair<double, int>, vector<pair<double, int>>, greater<>> pq;

	// 初始化源节点
	for (const auto& s : sources) {
		dist[s] = 0.0;
		pq.emplace(0.0, s);
	}

	while (!pq.empty()) {
		auto current = pq.top();
		double d = current.first;
		int u = current.second;
		pq.pop();

		if (d > dist[u]) continue;
		if (u == target) break;
		if (d > cut_off) continue;

		// 检查节点是否存在邻接表
		auto u_it = G_temp.find(u);
		if (u_it == G_temp.end()) continue;

		const auto& neighbors = u_it->second;
		for (const auto& edge : neighbors) {
			int v = edge.first;
			double weight = edge.second;  // 直接获取预存的权重值

			double new_dist = d + weight;
			if (!dist.count(v) || new_dist < dist[v]) {
				dist[v] = new_dist;
				pq.emplace(new_dist, v);
			}
		}
	}

	return dist;
}

vector<unordered_map<int, double>> GraphAlgorithms::test(
	const py::object& list_o_,
	const py::object& method_,
	const py::object& target_,
	const py::object& cut_off_,
	const py::object& weight_name_,
	const py::object& num_thread_)
{
	auto list_o = list_o_.cast<vector<int>>();
	auto method = method_.cast<string>();
	auto target = target_.cast<int>();
	auto cut_off = cut_off_.cast<double>();
	auto weight_name = weight_name_.cast<string>();
	auto num_thread = num_thread_.cast<int>();

	// 权重处理
	auto start = chrono::steady_clock::now();
	G_temp.clear();
	for (auto& entry : G) {
		int u = entry.first;
		auto& edges = entry.second;
		for (auto& edge : edges) {
			int v = edge.first;
			auto& attrs = edge.second;
			double weight = 1.0;
			auto attr_it = attrs.find(weight_name);
			if (attr_it != attrs.end()) {
				weight = attr_it->second;
			}

			G_temp[u].emplace_back(v, weight);
		}
	}
	auto end = chrono::steady_clock::now();
	auto duration = chrono::duration_cast<std::chrono::milliseconds>(end - start);
	std::cout << "权重 耗时：" << duration.count() << " 毫秒" << std::endl;

	// 结果计算
	auto start1 = std::chrono::steady_clock::now();
	vector<unordered_map<int, double>> final_result(list_o.size());
	vector<future<void>> futures;  // 用来管理异步任务

	size_t max_threads = std::thread::hardware_concurrency();
	if (num_thread > max_threads) num_thread = max_threads;

	// 使用 std::async 启动多个线程
	for (size_t i = 0; i < list_o.size(); ++i) {
		futures.push_back(std::async(std::launch::async, [&, i]() {
			vector<int> cur_list = { list_o[i] };
			unordered_map<int, double> result;

			if (method == "Dijkstra") {
				result = test1(cur_list, target, cut_off, weight_name);
			}

			std::lock_guard<std::mutex> lock(result_mutex); // 锁保护结果
			final_result[i] = result;
		}));
	}

	// 等待所有任务完成
	for (auto& fut : futures) {
		fut.get();
	}

	auto end1 = std::chrono::steady_clock::now();
	auto duration1 = chrono::duration_cast<std::chrono::milliseconds>(end1 - start1);
	std::cout << "计算耗时：" << duration1.count() << " 毫秒" << std::endl;

	return final_result;
}