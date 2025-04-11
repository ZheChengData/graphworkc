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
		throw runtime_error("not find path");
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


// 0.根据dir生成反向边
//vector<RowData> GraphAlgorithms::process_neg_dir(const vector<RowData>& net)
//{
//	std::vector<RowData> new_net;
//	for (const auto& row : net) {
//		if (row.dir == 0) {
//			RowData neg_row = row;
//			std::swap(neg_row.from_node, neg_row.to_node);
//			new_net.push_back(neg_row);
//		}
//		new_net.push_back(row);
//	}
//	return new_net;
//}
