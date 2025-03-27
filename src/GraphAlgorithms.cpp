#include "GraphAlgorithms.h"

// 定义一个互斥锁
mutex result_mutex;

// 核心算法 ---------------------------------------------------------------------------------------
// 多源花费
py::dict GraphAlgorithms::multi_source_dijkstra_cost(
	const vector< vector<pair<int, double>> >& g,
	const vector<int>& sources,
	int& target,
	double& cut_off,
	string& weight_name)
{
	py::dict res;  // 创建一个 Python 字典来存储结果
	vector<double> dist;
	dist.resize(cur_max_index + 1, numeric_limits<double>::infinity()); 
	priority_queue<pair<double, int>, vector<pair<double, int>>, greater<>> pq;

	// 1.初始化源节点
	for (const auto& s : sources) {
		int i = s;
		auto u_it = map_id_to_index.find(i);
		int start_index = u_it->second;
		dist[start_index] = 0.0;
		pq.emplace(0.0, start_index);
	}

	// 2.Dijkstra算法循环遍历各节点 得到索引对应的最小花费
	while (!pq.empty()) {
		auto current = pq.top();
		double& d = current.first; // distance
		int& u = current.second; // index
		pq.pop();

		// 如果当前距离大于已知的最短路径，跳过
		if (d > dist[u]) continue;

		// 如果达到目标节点，提前退出
		if (vec_index_to_id[u] == target) break;

		// 如果当前路径已超过cutoff值，跳过
		if (d > cut_off) continue;

		if (u >= g.size()) {
			continue;
		}
		else {
			for (auto& edge : g[u]) {
				int v = edge.first;
				double weight = edge.second;

				double new_dist = d + weight;

				// 更新距离表，避免多次查找
				if (new_dist < dist[v]) {
					dist[v] = new_dist;
					pq.emplace(new_dist, v);
				}
			}
		}

	}

	// 3.将索引字典改为节点字典，填充到 Python 字典中
	for (size_t i = 0; i < dist.size(); ++i) {
		if (dist[i] < numeric_limits<double>::infinity()) {
			if (dist[i] <= cut_off) {
				res[py::int_(vec_index_to_id[i])] = py::float_(dist[i]);
			}
		}
	}


	return res;  // 返回 Python 字典
}


// 多源花费 多线程
unordered_map<int, double> GraphAlgorithms::multi_source_dijkstra_cost_threading(
	const vector< vector<pair<int, double>> >& g,
	const vector<int>& sources,
	int& target,
	double& cut_off,
	string& weight_name)
{
	unordered_map<int, double> res;  // 创建一个 Python 字典来存储结果
	vector<double> dist(cur_max_index + 1, numeric_limits<double>::infinity());
	dist.reserve(dist.size());
	priority_queue<pair<double, int>, vector<pair<double, int>>, greater<>> pq;

	// 1.初始化源节点
	for (const auto& s : sources) {
		int i = s;
		auto u_it = map_id_to_index.find(i);
		int start_index = u_it->second;
		dist[start_index] = 0.0;
		pq.emplace(0.0, start_index);
	}

	// 2.Dijkstra算法循环遍历各节点 得到索引对应的最小花费
	while (!pq.empty()) {
		auto current = pq.top();
		double& d = current.first; // distance
		int& u = current.second; // index
		pq.pop();

		// 如果当前距离大于已知的最短路径，跳过
		if (d > dist[u]) continue;

		// 如果达到目标节点，提前退出
		if (u == target) break;

		// 如果当前路径已超过cutoff值，跳过
		if (d > cut_off) continue;

		if (u >= g.size()) continue;
		else {
			for (auto& edge : g[u]) {
				int v = edge.first;
				double weight = edge.second;

				double new_dist = d + weight;

				// 更新距离表，避免多次查找
				if (new_dist < dist[v]) {
					dist[v] = new_dist;
					pq.emplace(new_dist, v);
				}
			}
		}

	}

	// 3.将索引字典改为节点字典
	for (size_t i = 0; i < dist.size(); ++i) {
		if (dist[i] < numeric_limits<double>::infinity()) {
			if (dist[i] <= cut_off) {
				res[vec_index_to_id[i]] = dist[i];
			}
		}
	}

	return res; 
}


// 多源路径
unordered_map<int, vector<int>> GraphAlgorithms::multi_source_dijkstra_path(
	const vector<vector<pair<int, double>>>& g,
	const vector<int>& sources,
	int target,
	double cut_off,
	string weight_name)
{
	// 1. 检查目标是否是源节点之一
	for (const auto& s : sources) {
		if (s == target) {
			return { {s, {s}} };
		}
	}

	// 2. 初始化容器（关键修复）
	const size_t capacity = cur_max_index + 1;
	vector<double> dist(capacity, numeric_limits<double>::infinity());
	vector<int> pred(capacity, -1); // 正确初始化大小
	vector<vector<int>> paths(capacity); // 正确初始化大小
	priority_queue<pair<double, int>, vector<pair<double, int>>, greater<>> pq;

	// 3. 初始化源节点
	for (const auto& s : sources) {
		auto u_it = map_id_to_index.find(s);
		if (u_it == map_id_to_index.end()) continue; // 跳过无效节点

		const int start_index = u_it->second;
		if (start_index >= capacity) continue; // 确保索引有效性

		dist[start_index] = 0.0;
		pq.emplace(0.0, start_index);
		pred[start_index] = -1;
		paths[start_index] = { vec_index_to_id[start_index] }; // 存储原始ID
	}

	// 4. 遍历优先队列
	while (!pq.empty()) {
		auto current = pq.top();
		double& d = current.first; // distance
		int& u = current.second; // index
		pq.pop();

		if (d > dist[u]) continue;
		if (u == target) break;
		if (d > cut_off) continue;

		if (u >= g.size()) continue;
		else {
			for (const auto& pair : g[u]) {
				auto v = pair.first;
				auto weight = pair.second;
				const double new_dist = d + weight;
				if (new_dist < dist[v]) {
					dist[v] = new_dist;
					pred[v] = u;
					pq.emplace(new_dist, v);

					// 路径更新
					if (pred[v] != -1) {
						paths[v] = paths[pred[v]];
					}
					paths[v].push_back(vec_index_to_id[v]);

				}
			}
		}

	}

	// 5. 转换索引到原始ID
	unordered_map<int, vector<int>> res;
	for (size_t i = 0; i < dist.size(); ++i) {
		if (dist[i] < numeric_limits<double>::infinity() && dist[i] <= cut_off) {
			res[vec_index_to_id[i]] = paths[i];
		}
	}

	return res;
}


// 多源路径 多线程
unordered_map<int, vector<int>> GraphAlgorithms::multi_source_dijkstra_path_threading(
	const vector<vector<pair<int, double>>>& g,
	const vector<int>& sources,
	int target,
	double cut_off,
	string weight_name)
{
	unordered_map<int, vector<int>> res;

	// 1. 检查目标是否是源节点之一
	for (const auto& s : sources) {
		if (s == target) {
			return { {s, {s}} };
		}
	}

	// 2. 初始化容器（关键修复）
	const size_t capacity = cur_max_index + 1;
	vector<double> dist(capacity, numeric_limits<double>::infinity());
	vector<int> pred(capacity, -1); // 正确初始化大小
	vector<vector<int>> paths(capacity); // 正确初始化大小
	priority_queue<pair<double, int>, vector<pair<double, int>>, greater<>> pq;

	// 3. 初始化源节点
	for (const auto& s : sources) {
		auto u_it = map_id_to_index.find(s);
		if (u_it == map_id_to_index.end()) continue; // 跳过无效节点

		const int start_index = u_it->second;
		if (start_index >= capacity) continue; // 确保索引有效性

		dist[start_index] = 0.0;
		pq.emplace(0.0, start_index);
		pred[start_index] = -1;
		paths[start_index] = { vec_index_to_id[start_index] }; // 存储原始ID
	}

	// 4. 遍历优先队列
	while (!pq.empty()) {
		auto current = pq.top();
		double& d = current.first; // distance
		int& u = current.second; // index
		pq.pop();

		if (d > dist[u]) continue;
		if (u == target) break;
		if (d > cut_off) continue;

		if (u >= g.size()) continue;
		else {
			for (const auto& pair : g[u]) {
				auto v = pair.first;
				auto weight = pair.second;
				const double new_dist = d + weight;
				if (new_dist < dist[v]) {
					dist[v] = new_dist;
					pred[v] = u;
					pq.emplace(new_dist, v);

					// 路径更新
					if (pred[v] != -1) {
						paths[v] = paths[pred[v]];
					}
					paths[v].push_back(vec_index_to_id[v]);

				}
			}
		}
		
	}

	// 5. 转换索引到原始ID
	for (size_t i = 0; i < dist.size(); ++i) {
		if (dist[i] < numeric_limits<double>::infinity() && dist[i] <= cut_off) {
			res[vec_index_to_id[i]] = paths[i];
		}
	}

	return res;
};


// 多源路径+花费
dis_and_path GraphAlgorithms::multi_source_dijkstra(
	const vector<vector<pair<int, double>>>& g,
	const vector<int>& sources,
	int target,
	double cut_off,
	string weight_name)
{
	unordered_map<int, vector<int>> res_paths;
	unordered_map<int, double> res_distances;

	// 1. 检查目标是否是源节点之一
	for (const auto& s : sources) {
		if (s == target) {
			// 正确设置距离和路径
			res_distances[s] = 0.0;
			res_paths[s] = { s };
			return { res_distances, res_paths }; // 确保结构体成员顺序正确
		}
	}

	// 2. 初始化容器
	const size_t capacity = cur_max_index + 1;
	vector<double> dist(capacity, numeric_limits<double>::infinity());
	vector<int> pred(capacity, -1);
	vector<vector<int>> paths(capacity);
	priority_queue<pair<double, int>, vector<pair<double, int>>, greater<>> pq;

	// 3. 初始化源节点
	for (const auto& s : sources) {
		auto u_it = map_id_to_index.find(s);
		if (u_it == map_id_to_index.end()) {
			// 可选：记录警告或抛出异常
			continue;
		}
		const int start_index = u_it->second;
		if (start_index >= capacity) {
			// 处理索引越界（如调整capacity或报错）
			continue;
		}

		dist[start_index] = 0.0;
		pq.emplace(0.0, start_index);
		pred[start_index] = -1;
		paths[start_index] = { vec_index_to_id[start_index] }; // 存储原始ID
	}

	// 4. 处理优先队列
	while (!pq.empty()) {
		auto current = pq.top();
		double& d = current.first; // distance
		int& u = current.second; // index
		pq.pop();

		if (d > dist[u]) continue;
		if (u == target) break;
		if (d > cut_off) continue;

		if (u >= g.size()) continue;
		else {
			for (const auto& pair : g[u]) {
				auto v_idx = pair.first;
				auto weight = pair.second;
				const double new_dist = d + weight;
				if (new_dist < dist[v_idx]) {
					dist[v_idx] = new_dist;
					pred[v_idx] = u;
					pq.emplace(new_dist, v_idx);

					// 构建路径
					if (pred[v_idx] != -1) {
						paths[v_idx] = paths[pred[v_idx]];
					}
					paths[v_idx].push_back(vec_index_to_id[v_idx]);
				}
			}
		}
	}

	// 5. 收集结果
	for (size_t i = 0; i < dist.size(); ++i) {
		if (dist[i] < numeric_limits<double>::infinity() && dist[i] <= cut_off) {
			const int node_id = vec_index_to_id[i];
			res_distances[node_id] = dist[i];
			res_paths[node_id] = paths[i];
		}
	}

	return { res_distances, res_paths };
}


// 多源路径+花费 多线程
dis_and_path GraphAlgorithms::multi_source_dijkstra_threading(
	const vector<vector<pair<int, double>>>& g,
	const vector<int>& sources,
	int target,
	double cut_off,
	string weight_name)
{
	unordered_map<int, vector<int>> res_paths;
	unordered_map<int, double> res_distances;

	// 1. 检查目标是否是源节点之一
	for (const auto& s : sources) {
		if (s == target) {
			// 正确设置距离和路径
			res_distances[s] = 0.0;
			res_paths[s] = { s };
			return { res_distances, res_paths }; // 确保结构体成员顺序正确
		}
	}

	// 2. 初始化容器
	const size_t capacity = cur_max_index + 1;
	vector<double> dist(capacity, numeric_limits<double>::infinity());
	vector<int> pred(capacity, -1);
	vector<vector<int>> paths(capacity);
	priority_queue<pair<double, int>, vector<pair<double, int>>, greater<>> pq;

	// 3. 初始化源节点
	for (const auto& s : sources) {
		auto u_it = map_id_to_index.find(s);
		if (u_it == map_id_to_index.end()) {
			// 可选：记录警告或抛出异常
			continue;
		}
		const int start_index = u_it->second;
		if (start_index >= capacity) {
			// 处理索引越界（如调整capacity或报错）
			continue;
		}

		dist[start_index] = 0.0;
		pq.emplace(0.0, start_index);
		pred[start_index] = -1;
		paths[start_index] = { vec_index_to_id[start_index] }; // 存储原始ID
	}

	// 4. 处理优先队列
	while (!pq.empty()) {
		auto current = pq.top();
		double& d = current.first; // distance
		int& u = current.second; // index
		pq.pop();

		if (d > dist[u]) continue;
		if (u == target) break;
		if (d > cut_off) continue;

		if (u >= g.size()) continue;
		else {
			for (const auto& pair : g[u]) {
				auto v_idx = pair.first;
				auto weight = pair.second;
				const double new_dist = d + weight;
				if (new_dist < dist[v_idx]) {
					dist[v_idx] = new_dist;
					pred[v_idx] = u;
					pq.emplace(new_dist, v_idx);

					// 构建路径
					if (pred[v_idx] != -1) {
						paths[v_idx] = paths[pred[v_idx]];
					}
					paths[v_idx].push_back(vec_index_to_id[v_idx]);
				}
			}
		}
	}

	// 5. 收集结果
	for (size_t i = 0; i < dist.size(); ++i) {
		if (dist[i] < numeric_limits<double>::infinity() && dist[i] <= cut_off) {
			const int node_id = vec_index_to_id[i];
			res_distances[node_id] = dist[i];
			res_paths[node_id] = paths[i];
		}
	}

	return { res_distances, res_paths };
}


// 多源路径花费形心点
unordered_map<int, double> GraphAlgorithms::multi_source_dijkstra_cost_centroid(
	const vector< vector<pair<int, double>>>& g,
	const vector<int>& sources,
	const unordered_set<int>& targets,
	double cut_off,
	string weight_name)
{
	unordered_map<int, double> res;
	vector<double> dist(cur_max_index + 1, numeric_limits<double>::infinity()); // 使用索引距离表
	priority_queue<pair<double, int>, vector<pair<double, int>>, greater<>> pq;
	unordered_set<int> remaining_targets(targets.begin(), targets.end());

	// 将目标节点的原始ID转换为索引
	unordered_set<int> remaining_target_indices;
	for (int t : targets) {
		auto it = map_id_to_index.find(t);
		if (it != map_id_to_index.end()) {
			remaining_target_indices.insert(it->second);
		}
	}

	// 1. 初始化源节点（转换为索引）
	for (const auto& s : sources) {
		auto it = map_id_to_index.find(s);
		if (it == map_id_to_index.end()) continue; // 跳过无效源节点
		int start_index = it->second;

		dist[start_index] = 0.0;
		pq.emplace(0.0, start_index);

		// 如果源节点是目标节点，标记为已找到
		if (remaining_target_indices.count(start_index)) {
			remaining_target_indices.erase(start_index);
			if (remaining_target_indices.empty()) break;
		}
	}

	// 2. Dijkstra主循环
	while (!pq.empty() && !remaining_target_indices.empty()) {
		auto current = pq.top();
		double d = current.first;
		int u_index = current.second;
		pq.pop();

		if (d > dist[u_index]) continue;

		// 终止条件1：距离超过cut_off
		if (d > cut_off) {
			continue; // 无需删除，dist[u_index] 仍为无穷大
		}

		// 终止条件2：当前节点是目标节点
		if (remaining_target_indices.count(u_index)) {
			remaining_target_indices.erase(u_index);
			if (remaining_target_indices.empty()) break;
		}

		if (u_index >= g.size()) continue;
		else {
			// 遍历邻接节点
			for (const auto& edge : g[u_index]) {
				int v_index = edge.first;
				double weight = edge.second;
				double new_dist = d + weight;

				if (new_dist < dist[v_index]) {
					dist[v_index] = new_dist;
					pq.emplace(new_dist, v_index);
				}
			}
		}
	}

	// 3. 将结果从索引转换回原始ID
	for (int t : targets) {
		auto it = map_id_to_index.find(t);
		if (it != map_id_to_index.end()) {
			int idx = it->second;
			if (dist[idx] <= cut_off) {
				res[t] = dist[idx];
			}
			else {
				res[t] = -1; // 未找到或超过cut_off
			}
		}
		else {
			res[t] = -1; // 目标节点无效
		}
	}

	return res;
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
	const unordered_map<int, vector<pair<int, double>>>& reverse_g,
	const unordered_map<int, vector<pair<int, double>>>& g,
	const vector<int>& sources,
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
	const unordered_map<int, vector<pair<int, double>>>& reverse_g,
	const unordered_map<int, vector<pair<int, double>>>& g,
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

	int weight_index;
	for (int i = 0; i < field_vec.size(); i++) {
		if (field_vec[i] == weight_name) {
			weight_index = i;
			break;
		}
	}
	vector< vector<pair<int, double>> >& cur_weight_map1 = index_to_id_next_vec[weight_index];


	// 4.主循环：寻找最短简单路径
	while (true) {

		if (prev_path.empty()) {
			// 如果 prev_path 是空，直接计算最短路径
			auto result = multi_source_dijkstra_threading(cur_weight_map1, { source }, target, std::numeric_limits<double>::infinity(), weight_name);

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

	int weight_index = -1;
	for (int i = 0; i < field_vec.size(); i++) {
		if (field_vec[i] == weight_name) {
			weight_index = i;
			break;
		}
	}
	vector< vector<pair<int, double>> >& cur_weight_map1 = (weight_index != -1)
		? index_to_id_next_vec[weight_index]
		: get_not_full_weight_map();

	cout << cur_weight_map1.size() << endl;
	// 4.主循环：寻找最短简单路径
	while (true) {
		if (prev_path.empty()) {
			// 第一次最短路获取
			auto result = multi_source_dijkstra_threading(
				cur_weight_map1,
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


// 获取单个OD对的花费
pair<double, vector<int>> GraphAlgorithms::single_source_to_target(
	int source,
	int target,
	const string& weight_name) 
{
	// 1.节点检查
	if (G.find(source) == G.end()) {
		throw std::runtime_error("source node not in graph");
	}
	if (G.find(target) == G.end()) {
		throw std::runtime_error("target node not in graph");
	}

	// 2.权重获取
	const auto& weight_map = get_weight_map(weight_name);
	const auto& reverse_weight_map = get_weight_reverse_map(weight_name);

	// 3.设置初始参数
	set<int> ignore_nodes;
	set<pair<int, int>> ignore_edges;
	double cut_off = numeric_limits<double>::infinity();

	// 双向Dijkstra计算最短路径 
	auto result = bidirectional_dijkstra_ignore(
		reverse_weight_map,
		weight_map,
		{ source },
		target,
		cut_off,
		ignore_nodes,
		ignore_edges);

	if (result.cost.find(target) != result.cost.end() && result.cost[target] < numeric_limits<double>::infinity()) {
		double length = result.cost[target];
		vector<int> spur = result.paths[target];
		return {length, spur};
	}
	else {
		throw std::runtime_error("not find target path not in graph");
		double length = -1;
		vector<int> spur;
		spur.push_back(source);
		return { length, spur };
	}
}


// 创建无权重映射并返回
vector<vector<pair<int, double>>>& GraphAlgorithms::get_not_full_weight_map()
{
	static vector<vector<pair<int, double>>> res(cur_max_index + 1);

	for (auto& entry : G) {
		int u = entry.first;
		auto& edges = entry.second;
		for (auto& edge : edges) {
			int v = edge.first;
			double weight = 1.0;
			res[map_id_to_index[u]].emplace_back(map_id_to_index[v], weight);
		}
	}

	return res;
}

// 调用方法 ---------------------------------------------------------------------------------------

// 单源最短路
py::dict GraphAlgorithms::single_source_cost(
	const py::object& o_,
	const py::object& method_,
	const py::object& target_,
	const py::object& cut_off_,
	const py::object& weight_name_)
{
	// 1.输入参数 初始化
	auto o = o_.cast<int>();
	auto method = method_.cast<string>();
	auto target = target_.cast<int>();
	auto cut_off = cut_off_.cast<double>();
	auto weight_name = weight_name_.cast<string>();
	vector<int> list_o;
	list_o.push_back(o);

	// 2.权重预处理
	int weight_index = -1;
	for (int i = 0; i < field_vec.size(); i++) {
		if (field_vec[i] == weight_name) {
			weight_index = i;
			break;
		}
	}
	vector< vector<pair<int, double>> >& cur_weight_map = (weight_index != -1)
		? index_to_id_next_vec[weight_index]
		: get_not_full_weight_map();
	//const auto& weight_map = get_weight_map(weight_name);

	// 3.执行计算
	if (method == "Dijkstra") {
		py::dict result;
		result = multi_source_dijkstra_cost(
			cur_weight_map, list_o, target, cut_off, weight_name);
		return result;
	}
}


unordered_map<int, vector<int>> GraphAlgorithms::single_source_path(
	const py::object& o_,
	const py::object& method_,
	const py::object& target_,
	const py::object& cut_off_,
	const py::object& weight_name_)
{
	// 1.输入参数 初始化
	auto o = o_.cast<int>();
	auto method = method_.cast<string>();
	auto target = target_.cast<int>();
	auto cut_off = cut_off_.cast<double>();
	auto weight_name = weight_name_.cast<string>();
	vector<int> list_o;
	list_o.push_back(o);

	// 2.权重预处理
	int weight_index = -1;
	for (int i = 0; i < field_vec.size(); i++) {
		if (field_vec[i] == weight_name) {
			weight_index = i;
			break;
		}
	}
	vector< vector<pair<int, double>> >& cur_weight_map = (weight_index != -1)
		? index_to_id_next_vec[weight_index]
		: get_not_full_weight_map();
	//const auto& weight_map = get_weight_map(weight_name);

	// 3.执行计算
	if (method == "Dijkstra") {
		unordered_map<int, vector<int>> result = multi_source_dijkstra_path(
			cur_weight_map, list_o, target, cut_off, weight_name);
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
	// 1.输入参数 初始化
	auto o = o_.cast<int>();
	auto method = method_.cast<string>();
	auto target = target_.cast<int>();
	auto cut_off = cut_off_.cast<double>();
	auto weight_name = weight_name_.cast<string>();
	vector<int> list_o;
	list_o.push_back(o);

	// 2.权重预处理
	int weight_index = -1;
	for (int i = 0; i < field_vec.size(); i++) {
		if (field_vec[i] == weight_name) {
			weight_index = i;
			break;
		}
	}
	vector< vector<pair<int, double>> >& cur_weight_map = (weight_index != -1)
		? index_to_id_next_vec[weight_index]
		: get_not_full_weight_map();
	//const auto& weight_map = get_weight_map(weight_name);
	
	// 3.执行计算
	if (method == "Dijkstra") {
		dis_and_path result = multi_source_dijkstra(
			cur_weight_map, list_o, target, cut_off, weight_name);
		return result;
	}
}


// 多源最短路
py::dict GraphAlgorithms::multi_source_cost(
	const py::object& list_o_,
	const py::object& method_,
	const py::object& target_,
	const py::object& cut_off_,
	const py::object& weight_name_)
{
	// 1.输入参数 初始化
	auto list_o = list_o_.cast<vector<int>>();
	auto method = method_.cast<string>();
	auto target = target_.cast<int>();
	auto cut_off = cut_off_.cast<double>();
	auto weight_name = weight_name_.cast<std::string>();

	// 2.权重预处理
	int weight_index = -1;
	for (int i = 0; i < field_vec.size(); i++) {
		if (field_vec[i] == weight_name) {
			weight_index = i;
			break;
		}
	}
	vector< vector<pair<int, double>> >& cur_weight_map = (weight_index != -1)
		? index_to_id_next_vec[weight_index]
		: get_not_full_weight_map();
	//const auto& weight_map = get_weight_map(weight_name);


	// 3.执行计算
	if (method == "Dijkstra") {
		py::dict result = multi_source_dijkstra_cost(
			cur_weight_map, list_o, target, cut_off, weight_name);
		return result;
	}
	else {
		py::dict result;
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
	// 1.输入参数 初始化
	auto list_o = list_o_.cast<vector<int>>();
	auto method = method_.cast<string>();
	auto target = target_.cast<int>();
	auto cut_off = cut_off_.cast<double>();
	auto weight_name = weight_name_.cast<string>();

	const auto& weight_map = get_weight_map(weight_name);

	// 2.权重预处理
	int weight_index = -1;
	for (int i = 0; i < field_vec.size(); i++) {
		if (field_vec[i] == weight_name) {
			weight_index = i;
			break;
		}
	}
	vector< vector<pair<int, double>> >& cur_weight_map = (weight_index != -1)
		? index_to_id_next_vec[weight_index]
		: get_not_full_weight_map();
	//const auto& weight_map = get_weight_map(weight_name);

	// 3.执行计算
	if (method == "Dijkstra") {
		unordered_map<int, vector<int>> result = multi_source_dijkstra_path_threading(
			cur_weight_map, list_o, target, cut_off, weight_name);
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
	// 1.输入参数 初始化
	auto list_o = list_o_.cast<vector<int>>();
	auto method = method_.cast<string>();
	auto target = target_.cast<int>();
	auto cut_off = cut_off_.cast<double>();
	auto weight_name = weight_name_.cast<string>();

	// 2.权重预处理
	int weight_index = -1;
	for (int i = 0; i < field_vec.size(); i++) {
		if (field_vec[i] == weight_name) {
			weight_index = i;
			break;
		}
	}
	vector< vector<pair<int, double>> >& cur_weight_map = (weight_index != -1)
		? index_to_id_next_vec[weight_index]
		: get_not_full_weight_map();
	//const auto& weight_map = get_weight_map(weight_name);

	// 3.执行计算
	if (method == "Dijkstra") {
		dis_and_path result = multi_source_dijkstra_threading(
			cur_weight_map, list_o, target, cut_off, weight_name);
		return result;
	}
}


// 多个单源最短路
vector<unordered_map<int, double>> GraphAlgorithms::multi_single_source_cost(
	const py::object& list_o_,
	const py::object& method_,
	const py::object& target_,
	const py::object& cut_off_,
	const py::object& weight_name_,
	const py::object& num_thread_)
{
	// 1.输入参数 初始化
	auto list_o = list_o_.cast<vector<int>>();
	auto method = method_.cast<string>();
	auto target = target_.cast<int>();
	auto cut_off = cut_off_.cast<double>();
	auto weight_name = weight_name_.cast<string>();
	auto num_thread = num_thread_.cast<int>();

	// 2.权重预处理
	int weight_index = -1;
	for (int i = 0; i < field_vec.size(); i++) {
		if (field_vec[i] == weight_name) {
			weight_index = i;
			break;
		}
	}
	vector< vector<pair<int, double>> >& cur_weight_map = (weight_index != -1)
		? index_to_id_next_vec[weight_index]
		: get_not_full_weight_map();
	//const auto& weight_map = get_weight_map(weight_name);

	// 3.创建线程池，执行多线程计算
	vector<unordered_map<int, double>> final_result(list_o.size());
	vector<thread> threads;
	atomic<size_t> index(0);
	size_t max_threads = std::thread::hardware_concurrency();
	num_thread = std::max(1, std::min(num_thread, static_cast<int>(max_threads)));

	for (int t = 0; t < num_thread; ++t) {
		threads.emplace_back([&]() {
			while (true) {
				size_t i = index.fetch_add(1, std::memory_order_relaxed);
				if (i >= list_o.size()) break;

				int source = list_o[i];
				if (method == "Dijkstra") {
					unordered_map<int, double> result = multi_source_dijkstra_cost_threading(
						cur_weight_map, { source }, target, cut_off, weight_name);
					final_result[i] = result;
				}
			}
		});
	}
	
	// 4.等待所有线程完成
	for (auto& t : threads) {
		if (t.joinable()) t.join();
	}

	return final_result;
}


vector<unordered_map<int, vector<int>>> GraphAlgorithms::multi_single_source_path(
	const py::object& list_o_,
	const py::object& method_,
	const py::object& target_,
	const py::object& cut_off_,
	const py::object& weight_name_,
	const py::object& num_thread_) 
{
	// 1.输入参数 初始化
	auto list_o = list_o_.cast<vector<int>>();
	auto method = method_.cast<string>();
	auto target = target_.cast<int>();
	auto cut_off = cut_off_.cast<double>();
	auto weight_name = weight_name_.cast<string>();
	auto num_thread = num_thread_.cast<int>();

	// 2.权重预处理
	int weight_index = -1;
	for (int i = 0; i < field_vec.size(); i++) {
		if (field_vec[i] == weight_name) {
			weight_index = i;
			break;
		}
	}
	vector< vector<pair<int, double>> >& cur_weight_map = (weight_index != -1)
		? index_to_id_next_vec[weight_index]
		: get_not_full_weight_map();
	//const auto& weight_map = get_weight_map(weight_name);

	// 3.创建线程池，执行多线程计算
	vector<unordered_map<int, vector<int>>> final_result(list_o.size());  // 初始化 final_result 容器，大小与 list_o 相同
	vector<thread> threads;
	atomic<size_t> index(0);
	size_t max_threads = std::thread::hardware_concurrency();
	num_thread = std::min(static_cast<size_t>(num_thread), max_threads);

	for (int t = 0; t < num_thread; ++t) {
		threads.emplace_back([&]() {
			while (true) {
				size_t i = index.fetch_add(1, std::memory_order_relaxed);
				if (i >= list_o.size()) break;

				vector<int> cur_list = { list_o[i] };
				if (method == "Dijkstra") {
					unordered_map<int, vector<int>> result = multi_source_dijkstra_path_threading(
						cur_weight_map, cur_list, target, cut_off, weight_name);
					final_result[i] = result;
				}
			}
		});
	}

	// 4.等待所有线程完成
	for (auto& t : threads) {
		if (t.joinable()) t.join();
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
	// 1.输入参数 初始化
	auto list_o = list_o_.cast<vector<int>>();
	auto method = method_.cast<string>();
	auto target = target_.cast<int>();
	auto cut_off = cut_off_.cast<double>();
	auto weight_name = weight_name_.cast<string>();
	auto num_thread = num_thread_.cast<int>();

	// 2.权重预处理
	int weight_index = -1;
	for (int i = 0; i < field_vec.size(); i++) {
		if (field_vec[i] == weight_name) {
			weight_index = i;
			break;
		}
	}
	vector< vector<pair<int, double>> >& cur_weight_map = (weight_index != -1)
		? index_to_id_next_vec[weight_index]
		: get_not_full_weight_map();
	//const auto& weight_map = get_weight_map(weight_name);

	// 3.创建线程池，执行多线程计算
	vector<dis_and_path> final_result(list_o.size());  // 初始化 final_result 容器，大小与 list_o 相同
	vector<thread> threads;
	atomic<size_t> index(0);
	size_t max_threads = std::thread::hardware_concurrency();
	num_thread = std::min(static_cast<size_t>(num_thread), max_threads);
	for (int t = 0; t < num_thread; ++t) {
		threads.emplace_back([&]() {
			while (true) {
				size_t i = index.fetch_add(1, std::memory_order_relaxed);
				if (i >= list_o.size()) break;

				if (method == "Dijkstra") {
					dis_and_path result = multi_source_dijkstra_threading(
						cur_weight_map, { list_o[i] }, target, cut_off, weight_name);
					final_result[i] = result;
				}
			}
		});
	}

	// 4.等待所有线程完成
	for (auto& t : threads) {
		if (t.joinable()) t.join();
	}

	return final_result;
}


// 多个多源最短路
vector<unordered_map <int, double>> GraphAlgorithms::multi_multi_source_cost(
	const py::object& list_o_,
	const py::object& method_,
	const py::object& target_,
	const py::object& cut_off_,
	const py::object& weight_name_,
	const py::object& num_thread_)
{
	// 1.输入参数 初始化
	auto list_o = list_o_.cast<vector<vector<int>>>();
	auto method = method_.cast<string>();
	auto target = target_.cast<int>();
	auto cut_off = cut_off_.cast<double>();
	auto weight_name = weight_name_.cast<string>();
	auto num_thread = num_thread_.cast<int>();

	// 2.权重预处理
	int weight_index = -1;
	for (int i = 0; i < field_vec.size(); i++) {
		if (field_vec[i] == weight_name) {
			weight_index = i;
			break;
		}
	}
	vector< vector<pair<int, double>> >& cur_weight_map = (weight_index != -1)
		? index_to_id_next_vec[weight_index]
		: get_not_full_weight_map();
	//const auto& weight_map = get_weight_map(weight_name);

	// 3.创建线程池，执行多线程计算
	vector<unordered_map <int, double>> final_result(list_o.size());  // 初始化 final_result 容器，大小与 list_o 相同
	vector<thread> threads;
	atomic<size_t> index(0);
	size_t max_threads = std::thread::hardware_concurrency();
	num_thread = std::min(static_cast<size_t>(num_thread), max_threads);
	for (int t = 0; t < num_thread; ++t) {
		threads.emplace_back([&]() {
			while (true) {
				size_t i = index.fetch_add(1, std::memory_order_relaxed);
				if (i >= list_o.size()) break;

				vector<int> cur_list = list_o[i];
				if (method == "Dijkstra") {
					unordered_map <int, double> result = multi_source_dijkstra_cost_threading(
						cur_weight_map, cur_list, target, cut_off, weight_name);
					final_result[i] = result;
				}
			}
		});
	}

	// 4.等待所有线程完成
	for (auto& t : threads) {
		if (t.joinable()) t.join();
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
	// 1.输入参数 初始化
	auto list_o = list_o_.cast<vector<vector<int>>>();
	auto method = method_.cast<string>();
	auto target = target_.cast<int>();
	auto cut_off = cut_off_.cast<double>();
	auto weight_name = weight_name_.cast<string>();
	auto num_thread = num_thread_.cast<int>();

	// 2.权重预处理
	int weight_index = -1;
	for (int i = 0; i < field_vec.size(); i++) {
		if (field_vec[i] == weight_name) {
			weight_index = i;
			break;
		}
	}
	vector< vector<pair<int, double>> >& cur_weight_map = (weight_index != -1)
		? index_to_id_next_vec[weight_index]
		: get_not_full_weight_map();
	//const auto& weight_map = get_weight_map(weight_name);

	// 3.创建线程池，执行多线程计算
	vector<unordered_map<int, vector<int>>> final_result(list_o.size());  // 初始化 final_result 容器，大小与 list_o 相同
	vector<thread> threads;
	atomic<size_t> index(0);
	size_t max_threads = std::thread::hardware_concurrency();
	num_thread = std::min(static_cast<size_t>(num_thread), max_threads);
	for (int t = 0; t < num_thread; ++t) {
		threads.emplace_back([&]() {
			while (true) {
				size_t i = index.fetch_add(1, std::memory_order_relaxed);
				if (i >= list_o.size()) break;

				vector<int> cur_list = list_o[i];
				if (method == "Dijkstra") {
					unordered_map<int, vector<int>> result = multi_source_dijkstra_path_threading(
						cur_weight_map, cur_list, target, cut_off, weight_name);
					final_result[i] = result;
				}
			}
		});
	}

	// 4.等待所有线程完成
	for (auto& t : threads) {
		if (t.joinable()) t.join();
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
	// 1.输入参数 初始化
	auto list_o = list_o_.cast<vector<vector<int>>>();
	auto method = method_.cast<string>();
	auto target = target_.cast<int>();
	auto cut_off = cut_off_.cast<double>();
	auto weight_name = weight_name_.cast<string>();
	auto num_thread = num_thread_.cast<int>();

	// 2.权重预处理
	int weight_index = -1;
	for (int i = 0; i < field_vec.size(); i++) {
		if (field_vec[i] == weight_name) {
			weight_index = i;
			break;
		}
	}
	vector< vector<pair<int, double>> >& cur_weight_map = (weight_index != -1)
		? index_to_id_next_vec[weight_index]
		: get_not_full_weight_map();
	//const auto& weight_map = get_weight_map(weight_name);

	// 3.创建线程池，执行多线程计算
	vector<dis_and_path> final_result(list_o.size()); // 存储最终的计算结果
	vector<thread> threads; // 存储所有的线程对象 
	atomic<size_t> index(0); // 追踪当前任务的索引 atomic确保在多线程环境中访问index时是安全的
	size_t max_threads = std::thread::hardware_concurrency(); // 获取系统最大线程并发
	num_thread = std::min(static_cast<size_t>(num_thread), max_threads); // 实际创建的线程数
	for (int t = 0; t < num_thread; ++t) {
		threads.emplace_back([&]() { // 每个线程执行一个Lambda函数，不断从任务队列list_o中取出任务并进行计算
			while (true) {
				size_t i = index.fetch_add(1, std::memory_order_relaxed); // 获取当前任务的索引，并将index增加1
				if (i >= list_o.size()) break; // 当前任务索引大于总值，线程结束

				// 单个任务具体逻辑
				vector<int> cur_list = list_o[i];
				if (method == "Dijkstra") {
					dis_and_path result = multi_source_dijkstra_threading(
						cur_weight_map, cur_list, target, cut_off, weight_name);
					final_result[i] = result;
				}
			}
		});
	}

	// 4.等待所有线程完成
	for (auto& t : threads) {
		if (t.joinable()) t.join();
	}

	return final_result;
}


// 多个多源最短花费(带形心)
vector<unordered_map<int, double>> GraphAlgorithms::multi_multi_source_cost_centroid(
	const vector< vector<pair<int, double>>>& g,
	const py::object& list_o_,
	const py::object& method_,
	const py::object& target_,
	const py::object& cut_off_,
	const py::object& weight_name_,
	const py::object& num_thread_)
{
	// 1.初始化
	auto list_o = list_o_.cast<vector<vector<int>>>();
	auto method = method_.cast<string>();
	auto targets = target_.cast<unordered_set<int>>();
	auto cut_off = cut_off_.cast<double>();
	auto weight_name = weight_name_.cast<string>();
	auto num_thread = num_thread_.cast<int>();

	vector<unordered_map<int, double>> final_result(list_o.size());  // 初始化 final_result 容器，大小与 list_o 相同
	vector<thread> threads;
	atomic<size_t> index(0);
	size_t max_threads = std::thread::hardware_concurrency();
	num_thread = std::min(static_cast<size_t>(num_thread), max_threads); // 实际创建的线程数

	// 2.创建num_thread个线程，每个线程循环处理任务
	for (int t = 0; t < num_thread; ++t) {
		threads.emplace_back([&]() { // 每个线程执行一个Lambda函数，不断从任务队列list_o中取出任务并进行计算
			while (true) {
				size_t i = index.fetch_add(1, std::memory_order_relaxed); // 获取当前任务的索引，并将index增加1
				if (i >= list_o.size()) break; // 当前任务索引大于总值，线程结束

				// 单个任务具体逻辑
				vector<int> cur_list = list_o[i];
				if (method == "Dijkstra") {
					unordered_map<int, double> result = multi_source_dijkstra_cost_centroid(
						g, cur_list, targets, cut_off, weight_name);
					final_result[i] = result;
				}
			}
		});
	}

	// 等待所有线程完成
	for (auto& t : threads) {
		if (t.joinable()) t.join();
	}

	return final_result;
}


// 花费矩阵
py::array_t<double>  GraphAlgorithms::cost_matrix(
	const py::object& starts_,
	const py::object& ends_,
	const py::object& method_,
	const py::object& cut_off_,
	const py::object& weight_name_,
	const py::object& num_thread_)
{	
	// 1.输入初始化
	auto starts = starts_.cast<vector<int>>();
	auto ends = ends_.cast<vector<int>>();
	auto weight_name = weight_name_.cast<string>();
	py::object target_ = py::int_(-1);
	size_t num_starts = starts.size();
	size_t num_ends = ends.size();

	// 2.获取目标节点集合
	std::unordered_set<int> targets(ends.begin(), ends.end());  // 目标集合用于快速查找
	for (int end : ends) {
		// 判断该点是否是形心点
		if (m_node_map[end]["centroid_"] == 1) {
			// 如果是形心点，遍历其入边
			auto it = m_centroid_end_map.find(end);  // 查找该终点是否存在
			if (it != m_centroid_end_map.end()) {
				// 找到入边起点并加入 targets 集合
				for (const auto& entry : it->second) {
					int start = entry.first;  // 获取入边的起点
					targets.insert(start);    // 将起点加入目标集合
				}
			}
			targets.erase(end);
		}
	}
	py::set target_set;  // 创建一个 py::set
	for (int val : targets) {
		target_set.add(val);  // 使用 add() 方法添加元素
	}

	// 3.将形心点加入临时图
	GTemp = G;
	for (auto i : starts) {
		if (m_node_map[i]["centroid_"] == 1) {
			GTemp[i] = m_centroid_start_map[i];
		}
	}

	// 4.权重字典初始化
	vector<vector<pair<int, double>>> weight_vec(cur_max_index + 1);
	for (auto& entry : GTemp) {
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
			weight_vec[map_id_to_index[u]].emplace_back(map_id_to_index[v], weight);
		}
	}

	// 5.最终结果矩阵构建
	py::array_t<double> result({ num_starts, num_ends });
	py::buffer_info buf_info = result.request();
	double* ptr = static_cast<double*>(buf_info.ptr);

	vector<vector<int>> multi_list_;

	// 6.循环计算处理每个批次
	const size_t num_thread = static_cast<size_t>(num_thread_.cast<int>());
	const size_t batch_size = 10 * num_thread;  
	const size_t num_batches = (num_starts + batch_size - 1) / batch_size;

	for (size_t batch_idx = 0; batch_idx < num_batches; ++batch_idx) {
		// 计算当前批次的起点范围
		size_t start_idx = batch_idx * batch_size;
		const size_t end_idx = std::min(start_idx + batch_size, num_starts);

		// 生成当前批次的multi_list_
		vector<vector<int>> multi_list_;
		for (size_t i = start_idx; i < end_idx; ++i) {
			multi_list_.push_back({ starts[i] });
		}

		// 调用多源计算函数（内部多线程）
		py::object multi_list_obj = py::cast(multi_list_);
		vector<unordered_map<int, double>> multi_result = multi_multi_source_cost_centroid(
			weight_vec, multi_list_obj, method_, target_set, cut_off_, weight_name_, num_thread_);

		// 填充当前批次的 cost matrix
		for (size_t i = start_idx; i < end_idx; ++i) {
			for (size_t j = 0; j < num_ends; ++j) {
				// 如果起点等于终点，直接返回0
				if (starts[i] == ends[j]) {
					ptr[i * num_ends + j] = 0;
					continue; 
				}

				// 如果终点是形心点
				if (m_node_map[ends[j]]["centroid_"] != 1) {
					auto it = multi_result[i - start_idx].find(ends[j]);
					if (it != multi_result[i - start_idx].end()) {
						ptr[i * num_ends + j] = it->second;
					}
					else {
						ptr[i * num_ends + j] = -1; // 默认值
					}
				}

				// 如果终点不是形心点
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

	return result;
}


// 路径字典：所有起点到所有终点
py::dict GraphAlgorithms::path_dict(
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
	py::object target_ = py::int_(-1);
	py::object method = method_;
	py::object cut_off = cut_off_;
	py::object weight_name = weight_name_;
	py::object num_thread = num_thread_;

	// 创建一个字典来存储结果
	py::dict result;
	
	vector<vector<int>> multi_list_;
	for (auto i : starts) {
		vector<int> cur_vec{ i };
		multi_list_.push_back(cur_vec);
	}
	py::object multi_list_obj = py::cast(multi_list_);

	vector<unordered_map<int, vector<int>>> multi_result = multi_multi_source_path(
		multi_list_obj,
		method,
		target_,
		cut_off,
		weight_name,
		num_thread);

	// 填充字典
	for (int i = 0; i < num_starts; ++i) {
		for (int j = 0; j < num_ends; ++j) {
			auto it = multi_result[i].find(ends[j]);
			py::list path_list;

			if (it != multi_result[i].end()) {
				auto cur_path = it->second;
				// 将 cur_path 的每个元素加入到 path_list 中，而不是将整个 cur_path 作为一个元素
				for (const auto& node : cur_path) {
					path_list.append(node);
				}
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


// 路径字典：OD一一对应
py::dict GraphAlgorithms::path_dict_pairwise(
	const py::object& starts_,
	const py::object& ends_,
	const py::object& method_,
	const py::object& weight_name_,
	const py::object& num_thread_)
{
	// 1.初始化
	auto starts = starts_.cast<vector<int>>();
	auto ends = ends_.cast<vector<int>>();
	string method = method_.cast<string>();
	string weight_name = weight_name_.cast<string>();
	int num_thread = num_thread_.cast<int>();

	auto cut_off = numeric_limits<double>::infinity();

	size_t num_starts = starts.size();
	size_t num_ends = ends.size();

	py::dict result; // 结果字典

	// 2.生成OD列表
	vector<int> start_list;
	vector<int> end_list;
	for (auto i : starts) {
		start_list.push_back(i);
	}
	for (auto i : ends) {
		end_list.push_back(i);
	}
	py::object start_list_ = py::cast(start_list);
	py::object end_list_ = py::cast(end_list);

	// 3.多线程初始化
	const auto& weight_map = get_weight_map(weight_name);
	const auto& reverse_map = get_weight_reverse_map(weight_name);
	vector<unordered_map<int, vector<int>>> final_result(start_list.size()); // 存储最终的计算结果
	vector<thread> threads; // 存储所有的线程对象 
	atomic<size_t> index(0); // 追踪当前任务的索引 atomic确保在多线程环境中访问index时是安全的
	size_t max_threads = std::thread::hardware_concurrency(); // 获取系统最大线程并发
	num_thread = std::min(static_cast<size_t>(num_thread), max_threads); // 实际创建的线程数

	// 4.多线程循环处理获取结果
	for (int t = 0; t < num_thread; ++t) {
		threads.emplace_back([&]() { // 每个线程执行一个Lambda函数，不断从任务队列list_o中取出任务并进行计算
			while (true) {
				size_t i = index.fetch_add(1, std::memory_order_relaxed); // 获取当前任务的索引，并将index增加1
				if (i >= start_list.size()) break; // 当前任务索引大于总值，线程结束

				// 单个任务具体逻辑
				int start_node = start_list[i];
				int end_node = end_list[i];
				if (method == "Dijkstra") {
					unordered_map<int, vector<int>> result = bidirectional_dijkstra(
						reverse_map, weight_map, { start_node }, end_node, cut_off).paths;

					final_result[i] = result;
				}
			}
		});
	}

	// 等待所有线程完成
	for (auto& t : threads) {
		if (t.joinable()) t.join();
	}

	// 5.转换 final_result 到 py::dict result
	for (size_t i = 0; i < num_starts; ++i) {
		for (const auto& pair : final_result[i]) {
			// 将 (start_node, end_node) 键值对保存到 result 中
			result[py::make_tuple(starts[i], ends[i])] = py::cast(pair.second);
		}
	}

	return result;
}


// 获取K条最短路径 
vector<vector<int>> GraphAlgorithms::k_shortest_paths(
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


// 单源节点到达目标点的最短花费
double GraphAlgorithms::shortest_path_cost(
	const py::object& source_,
	const py::object& target_,
	const py::object& weight_name_) 
{
	auto source = source_.cast<int>();
	auto target = target_.cast<int>();
	auto weight_name = weight_name_.cast<string>();

	auto result = single_source_to_target(source, target, weight_name);
	double cost = result.first;
	return cost;
}


// 单源节点到达目标点的最短花费
vector<int> GraphAlgorithms::shortest_path_path(
	const py::object& source_,
	const py::object& target_,
	const py::object& weight_name_)
{
	auto source = source_.cast<int>();
	auto target = target_.cast<int>();
	auto weight_name = weight_name_.cast<string>();

	auto result = single_source_to_target(source, target, weight_name);
	vector<int> path = result.second;
	return path;
}


// 单源节点到达目标点的最短花费和路径
pair<double, vector<int>> GraphAlgorithms::shortest_path_all(
	const py::object& source_,
	const py::object& target_,
	const py::object& weight_name_) 
{
	auto source = source_.cast<int>();
	auto target = target_.cast<int>();
	auto weight_name = weight_name_.cast<string>();

	auto result = single_source_to_target(source, target, weight_name);
	return result;
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

std::vector<RowData> GraphAlgorithms::convert_dataframe(py::object df)
{
	std::vector<RowData> rows;

	py::array seq_array = df.attr("seq").cast<py::array>();
	py::array from_node_array = df.attr("from_node").cast<py::array>();
	py::array to_node_array = df.attr("to_node").cast<py::array>();
	py::array length_array = df.attr("length").cast<py::array>();
	py::array dir_array = df.attr("dir").cast<py::array>();
	py::array prj_dis_array = df.attr("prj_dis").cast<py::array>();
	py::array route_dis_array = df.attr("route_dis").cast<py::array>();

	auto seq = seq_array.unchecked<int>();
	auto from_node = from_node_array.unchecked<int>();
	auto to_node = to_node_array.unchecked<int>();
	auto length = length_array.unchecked<double>();
	auto dir = dir_array.unchecked<int>();
	auto prj_dis = prj_dis_array.unchecked<double>();
	auto route_dis = route_dis_array.unchecked<double>();

	for (py::ssize_t i = 0; i < seq.shape(0); ++i) {
		RowData row;
		row.seq = seq(i);
		row.from_node = from_node(i);
		row.to_node = to_node(i);
		row.length = length(i);
		row.dir = dir(i);
		row.prj_dis = prj_dis(i);
		row.route_dis = route_dis(i);
		rows.push_back(row);
	}

	return rows;
}

std::vector<RowData> GraphAlgorithms::process_neg_dir(const std::vector<RowData>& net) 
{
	std::vector<RowData> new_net;
	for (const auto& row : net) {
		if (row.dir == 0) {
			RowData neg_row = row;
			std::swap(neg_row.from_node, neg_row.to_node);
			new_net.push_back(neg_row);
		}
		new_net.push_back(row);
	}
	return new_net;
}

std::map<int, vector<RowData>> GraphAlgorithms::group_by_seq(const std::vector<RowData>& new_net) 
{
	std::map<int, std::vector<RowData>> seq_groups;
	for (const auto& row : new_net) {
		seq_groups[row.seq].push_back(row);
	}
	return seq_groups;
}

std::vector<py::array_t<double>> GraphAlgorithms::process_pairs(
	const std::map<int, std::vector<RowData>>& seq_groups,
	const std::vector<int>& unique_sorted_values) {

	std::vector<py::array_t<double>> list_res;

	for (size_t i = 0; i < unique_sorted_values.size() - 1; ++i) {
		int front = unique_sorted_values[i];
		int back = unique_sorted_values[i + 1];

		const auto& net_0 = seq_groups.at(front);
		const auto& net_1 = seq_groups.at(back);

		// Extract from_nodes
		std::vector<int> from_nodes_0, from_nodes_1;
		for (const auto& row : net_0) from_nodes_0.push_back(row.from_node);
		for (const auto& row : net_1) from_nodes_1.push_back(row.from_node);

		// 调用C++内部函数

		py::array_t<double> cost_matrix = cost_matrix_to_numpy1(
			from_nodes_0,
			from_nodes_1,
			"Dijkstra",
			numeric_limits<double>::infinity(),
			"l",
			10);

		auto path_dict = path_list_to_numpy1(
			from_nodes_0,
			from_nodes_1,
			"Dijkstra",
			numeric_limits<double>::infinity(),
			"l",
			10);

		// 创建numpy_x矩阵
		size_t rows = net_0.size();
		size_t cols = net_1.size();
		py::array_t<double> numpy_x({ rows, cols });
		auto buf = numpy_x.mutable_unchecked<2>();

		for (size_t idx0 = 0; idx0 < net_0.size(); ++idx0) {
			const auto& row0 = net_0[idx0];
			for (size_t idx1 = 0; idx1 < net_1.size(); ++idx1) {
				const auto& row1 = net_1[idx1];
				double cur_x = 9999.0;

				// 条件1
				if (row0.from_node == row1.from_node && row0.to_node == row1.to_node) {
					cur_x = cost_matrix.at(idx0, idx1) - row0.route_dis + row1.route_dis;
				}
				// 条件2
				else {
					//auto key = std::make_pair(row0.from_node, row1.from_node);
					auto key = py::make_tuple(row0.from_node, row1.from_node);  // 将 std::pair 转换为 py::tuple
					if (path_dict.contains(key)) {  
						const auto& path = path_dict[key].cast<std::vector<int>>();  // 将字典值转换为 std::vector<int>
						//const auto& path = path_dict[key];
						if (path.size() > 1 &&
							path[1] == row0.to_node &&
							(path[path.size() - 2] != row1.to_node)) {
							cur_x = cost_matrix.at(idx0, idx1) - row0.route_dis + row1.route_dis;
						}
					}
					else if (row0.from_node == row1.to_node && row0.to_node == row1.from_node) {
						cur_x = cost_matrix.at(idx0, idx1) - row0.route_dis + row1.route_dis;
					}
				}

				buf(idx0, idx1) = cur_x;
			}
		}

		list_res.push_back(numpy_x);
	}

	return list_res;
}

// 花费矩阵
py::array_t<double>  GraphAlgorithms::cost_matrix_to_numpy1(
	const vector<int>& starts_,
	const vector<int>& ends_,
	const string& method_,
	const double& cut_off_,
	const string& weight_name_,
	const int& num_thread_)
{
	//// 逻辑运行
	//GTemp = G;
	//// 获取起点列表和终点列表及其大小
	//auto starts = starts_;
	//auto ends = ends_;
	//auto method = method_;
	//auto cut_off = cut_off_;
	//auto weight_name = weight_name_;
	//auto num_thread = num_thread_;
	//size_t num_starts = starts.size();
	//size_t num_ends = ends.size();

	//// 将行星点加入临时图
	//for (auto i : starts) {
	//	if (m_node_map[i]["centroid_"] == 1) {
	//		GTemp[i] = m_centroid_start_map[i];
	//	}
	//}

	//// 创建一个二维数组来存储所有起点到终点的花费
	//py::array_t<double> result({ num_starts, num_ends });
	//py::buffer_info buf_info = result.request();
	//double* ptr = static_cast<double*>(buf_info.ptr);

	//py::object target_ = py::int_(-1);
	//vector<vector<int>> multi_list_;

	//// 这里根据num_thread来分批处理
	//size_t num_batches = (num_starts + num_thread - 1) / num_thread;  // 计算批次数

	//// 循环每个批次
	//for (size_t batch_idx = 0; batch_idx < num_batches; ++batch_idx) {
	//	// 计算当前批次的起点范围
	//	size_t start_idx = batch_idx * num_thread;
	//	size_t end_idx = min((batch_idx + 1) * num_thread, num_starts);

	//	// 生成当前批次的multi_list_
	//	multi_list_.clear();
	//	for (size_t i = start_idx; i < end_idx; ++i) {
	//		vector<int> cur_vec{ starts[i] };
	//		multi_list_.push_back(cur_vec);
	//	}

	//	// 转换成 py::object（已经是 py::list 类型）
	//	py::object multi_list_obj = py::cast(multi_list_);

	//	// 计算当前批次的多源最短路径
	//	vector<unordered_map<int, double>> multi_result = multi_multi_source_cost1(multi_list_, method, -1, cut_off, weight_name, num_thread);

	//	// 填充当前批次的 cost matrix
	//	for (size_t i = start_idx; i < end_idx; ++i) {
	//		for (size_t j = 0; j < num_ends; ++j) {
	//			// 如果起点等于终点，直接返回0
	//			if (starts[i] == ends[j]) {
	//				ptr[i * num_ends + j] = 0;
	//				continue;
	//			}

	//			// 如果终点是行星点
	//			if (m_node_map[ends[j]]["centroid_"] != 1) {
	//				auto it = multi_result[i - start_idx].find(ends[j]);
	//				if (it != multi_result[i - start_idx].end()) {
	//					ptr[i * num_ends + j] = it->second;
	//				}
	//				else {
	//					ptr[i * num_ends + j] = -1; // 默认值
	//				}
	//			}

	//			// 如果终点不是行星点
	//			else {
	//				if (m_centroid_end_map[ends[j]].size() == 0) {
	//					ptr[i * num_ends + j] = -1;
	//				}
	//				else {
	//					double minest_cost = numeric_limits<double>::infinity();
	//					// 遍历前导图
	//					for (const auto& pair : m_centroid_end_map[ends[j]]) {
	//						// 1. 判断 pair.second[weight_name] 是否存在
	//						const auto& weight_it = pair.second.find(weight_name);
	//						const double weight_value = (weight_it != pair.second.end()) ? weight_it->second : 1.0;

	//						// 2. 判断 multi_result[i][pair.first] 是否存在
	//						const auto& result_it = multi_result[i - start_idx].find(pair.first);
	//						if (result_it == multi_result[i - start_idx].end()) {
	//							continue; // 跳过本次循环
	//						}

	//						// 3. 计算当前成本
	//						const double cur_cost = weight_value + result_it->second;
	//						minest_cost = std::min(minest_cost, cur_cost);
	//					}
	//					// 最终赋值逻辑（需处理全跳过的边界情况）
	//					ptr[i * num_ends + j] = (minest_cost != std::numeric_limits<double>::infinity()) ? minest_cost : -1;
	//				}
	//			}
	//		}
	//	}
	//}
	py::array_t<double> result;
	return result; // 返回NumPy数组
}


// 路径字典
py::dict GraphAlgorithms::path_list_to_numpy1(
	const vector<int>& starts_,
	const vector<int>& ends_,
	const string& method_,
	const double& cut_off_,
	const string& weight_name_,
	const int& num_thread_)
{
	// 获取起点列表和终点列表的大小
	auto starts = starts_;
	auto ends = ends_;
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

	vector<unordered_map<int, vector<int>>> multi_result = multi_multi_source_path1(multi_list_,
		method_,
		-1,
		cut_off_,
		weight_name_,
		num_thread_);

	// 填充字典
	for (int i = 0; i < num_starts; ++i) {
		for (int j = 0; j < num_ends; ++j) {
			auto it = multi_result[i].find(ends[j]);
			py::list path_list;

			if (it != multi_result[i].end()) {
				auto cur_path = it->second;
				// 将 cur_path 的每个元素加入到 path_list 中，而不是将整个 cur_path 作为一个元素
				for (const auto& node : cur_path) {
					path_list.append(node);
				}
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

// 多个多源最短路径计算
vector<py::dict> GraphAlgorithms::multi_multi_source_cost1(
	const vector<vector<int>>& list_o_,
	const string& method_,
	const int & target_,
	const double& cut_off_,
	const string& weight_name_,
	const int& num_thread_)
{
	auto list_o = list_o_;
	auto method = method_;
	auto target = target_;
	auto cut_off = cut_off_;
	auto weight_name = weight_name_;
	auto num_thread = num_thread_;

	const auto& weight_map = get_weight_map(weight_name);

	int weight_index;
	for (int i = 0; i < field_vec.size(); i++) {
		if (field_vec[i] == weight_name) {
			weight_index = i;
			break;
		}
	}
	vector< vector<pair<int, double>> >& cur_weight_map = index_to_id_next_vec[weight_index];


	// 逻辑执行
	vector<py::dict> final_result(list_o.size());  // 初始化 final_result 容器，大小与 list_o 相同
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
						py::dict result = multi_source_dijkstra_cost(cur_weight_map, cur_list, target, cut_off, weight_name);

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


vector<unordered_map<int, vector<int>>> GraphAlgorithms::multi_multi_source_path1(
	const vector<vector<int>>& list_o_,
	const string& method_,
	const int& target_,
	const double& cut_off_,
	const string& weight_name_,
	const int& num_thread_)
{
	auto list_o = list_o_;
	//auto method = method_;
	//auto target = target_;
	//auto cut_off = cut_off_;
	//auto weight_name = weight_name_;
	//auto num_thread = num_thread_;

	//const auto& weight_map = get_weight_map(weight_name);
	//// 逻辑执行
	vector<unordered_map<int, vector<int>>> final_result(list_o.size());  // 初始化 final_result 容器，大小与 list_o 相同
	//vector<thread> threads;
	//atomic<size_t> index(0);
	//size_t max_threads = std::thread::hardware_concurrency();
	//if (num_thread >= max_threads) num_thread = max_threads - 1;

	//// 使用互斥锁来保护 final_result 的访问
	//std::mutex result_mutex;

	//while (index < list_o.size()) {
	//	// 启动最大数量的线程
	//	while (threads.size() < num_thread && index < list_o.size()) {
	//		threads.push_back(thread([&]() {
	//			size_t i = index++;  // 获取当前线程处理的节点索引
	//			if (i < list_o.size()) {
	//				// 每个线程处理一个节点
	//				vector<int> cur_list;
	//				cur_list = list_o[i];

	//				// 执行 Dijkstra 或其他算法
	//				if (method == "Dijkstra") {
	//					unordered_map<int, vector<int>> result = multi_source_dijkstra_path_threading(weight_map, cur_list, target, cut_off, weight_name);

	//					// 使用互斥锁保护对 final_result 的访问
	//					std::lock_guard<std::mutex> lock(result_mutex);
	//					final_result[i] = result;  // 确保将结果存储在正确的索引位置
	//				}
	//			}
	//		}));
	//	}

	//	// 等待线程池中的线程完成
	//	for (auto& t : threads) {
	//		if (t.joinable()) {
	//			t.join();
	//		}
	//	}
	//	threads.clear();
	//}

	return final_result;
}
