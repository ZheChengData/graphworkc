#include "GraphAlgorithms.h"


// 定义一个互斥锁
mutex result_mutex;

// 核心算法 ---------------------------------------------------------------------------------------

unordered_map<int, double> GraphAlgorithms::multi_source_dijkstra_cost_centroid(
	const vector<int>& sources,
	int target,
	double cutoff,
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
		if (d > cutoff) continue;

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

unordered_map<int, double> GraphAlgorithms::multi_source_dijkstra_cost(
	const vector<int>& sources,
	int target,
	double cutoff,
	string weight_name)
{
	unordered_map<int, double> dist;
	priority_queue<pair<double, int>, vector<pair<double, int>>, greater<>> pq;

	// 初始化源节点
	for (const auto& s : sources) {
		dist[s] = 0.0;
		pq.emplace(0.0, s);
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
					pq.emplace(new_dist, v);
				}
			}
		}
	}

	return dist;
};

unordered_map<int, vector<int>> GraphAlgorithms::multi_source_dijkstra_path(
	const vector<int>& sources,
	int target,
	double cutoff,
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
				// 修改路径构建逻辑
				if (dist.find(v) == dist.end() || new_dist < dist[v]) {
					dist[v] = new_dist;
					pred[v] = u;
					pq.emplace(new_dist, v);

					// 重构路径生成逻辑
					vector<int> new_path;
					if (pred[v] != -1) {
						new_path = paths[pred[v]];  // 获取前驱节点的完整路径
					}
					new_path.push_back(v);
					paths[v] = new_path;
				}
			}
		}
	}

	//返回序列路径
	return paths;
};

dis_and_path GraphAlgorithms::multi_source_dijkstra(
	const vector<int>& sources,
	int target,
	double cutoff,
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

					// 重构路径生成逻辑
					vector<int> new_path;
					if (pred[v] != -1) {
						new_path = paths[pred[v]];  // 获取前驱节点的完整路径
					}
					new_path.push_back(v);
					paths[v] = new_path;
				}
			}
		}
	}

	//返回最短路径和花费
	return { dist, paths };
}

double GraphAlgorithms::shortest_path_dijkstra(
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

// 核心算法 ---------------------------------------------------------------------------------------

// 调用方法 ---------------------------------------------------------------------------------------

// 多源最短路径计算：返回花费
unordered_map<int, double> GraphAlgorithms::multi_source_cost(
	const py::object& list_o_,
	const py::object& method_,
	const py::object& target_,
	const py::object& cutoff_,
	const py::object& weight_name_)
{
	auto list_o = list_o_.cast<vector<int>>();
	auto method = method_.cast<string>();
	auto target = target_.cast<int>();
	auto cutoff = cutoff_.cast<double>();
	auto weight_name = weight_name_.cast<std::string>();

	// 逻辑执行
	if (method == "Dijkstra") {
		unordered_map<int, double> result = multi_source_dijkstra_cost(list_o, target, cutoff, weight_name);
		return result;
	}
}

unordered_map<int, vector<int>> GraphAlgorithms::multi_source_path(
	const py::object& list_o_,
	const py::object& method_,
	const py::object& target_,
	const py::object& cutoff_,
	const py::object& weight_name_)
{
	
	auto list_o = list_o_.cast<vector<int>>();
	auto method = method_.cast<string>();
	auto target = target_.cast<int>();
	auto cutoff = cutoff_.cast<double>();
	auto weight_name = weight_name_.cast<string>();

	// 逻辑执行
	if (method == "Dijkstra") {
		// 逻辑执行
		unordered_map<int, vector<int>> result = multi_source_dijkstra_path(list_o, target, cutoff, weight_name);
		return result;
	}
}

dis_and_path GraphAlgorithms::multi_source_all(
	const py::object& list_o_,
	const py::object& method_,
	const py::object& target_,
	const py::object& cutoff_,
	const py::object& weight_name_)
{
	auto list_o = list_o_.cast<vector<int>>();
	auto method = method_.cast<string>();
	auto target = target_.cast<int>();
	auto cutoff = cutoff_.cast<double>();
	auto weight_name = weight_name_.cast<string>();


	// 逻辑执行
	if (method == "Dijkstra") {
		dis_and_path result = multi_source_dijkstra(list_o, target, cutoff, weight_name);
		return result;
	}
}

// 单源最短路径计算
unordered_map<int, double> GraphAlgorithms::single_source_cost(
	const py::object& o_,
	const py::object& method_,
	const py::object& target_,
	const py::object& cutoff_,
	const py::object& weight_name_)
{
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
		result = multi_source_dijkstra_cost(list_o, target, cutoff, weight_name);
		return result;
	}
}

unordered_map<int, std::vector<int>> GraphAlgorithms::single_source_path(
	const py::object& o_,
	const py::object& method_,
	const py::object& target_,
	const py::object& cutoff_,
	const py::object& weight_name_)
{
	auto o = o_.cast<int>();
	auto method = method_.cast<string>();
	auto target = target_.cast<int>();
	auto cutoff = cutoff_.cast<double>();
	auto weight_name = weight_name_.cast<string>();
	vector<int> list_o;
	list_o.push_back(o);

	// 逻辑执行
	if (method == "Dijkstra") {
		unordered_map<int, vector<int>> result = multi_source_dijkstra_path(list_o, target, cutoff, weight_name);
		return result;
	}
}

dis_and_path GraphAlgorithms::single_source_all(
	const py::object& o_,
	const py::object& method_,
	const py::object& target_,
	const py::object& cutoff_,
	const py::object& weight_name_)
{
	auto o = o_.cast<int>();
	auto method = method_.cast<string>();
	auto target = target_.cast<int>();
	auto cutoff = cutoff_.cast<double>();
	auto weight_name = weight_name_.cast<string>();
	vector<int> list_o;
	list_o.push_back(o);

	// 逻辑执行
	if (method == "Dijkstra") {
		dis_and_path result = multi_source_dijkstra(list_o, target, cutoff, weight_name);
		return result;
	}
}

// 多个单源最短路径计算
vector<unordered_map<int, double>> GraphAlgorithms::multi_single_source_cost(
	const py::object& list_o_,
	const py::object& method_,
	const py::object& target_,
	const py::object& cutoff_,
	const py::object& weight_name_,
	const py::object& num_thread_
) {
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
						result = multi_source_dijkstra_cost(cur_list, target, cutoff, weight_name);

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
	const py::object& cutoff_,
	const py::object& weight_name_,
	const py::object& num_thread_
) {
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
						unordered_map<int, vector<int>> result = multi_source_dijkstra_path(cur_list, target, cutoff, weight_name);

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
	const py::object& cutoff_,
	const py::object& weight_name_,
	const py::object& num_thread_)
{
	auto list_o = list_o_.cast<vector<int>>();
	auto method = method_.cast<string>();
	auto target = target_.cast<int>();
	auto cutoff = cutoff_.cast<double>();
	auto weight_name = weight_name_.cast<string>();
	auto num_thread = num_thread_.cast<int>();

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
						dis_and_path result = multi_source_dijkstra(cur_list, target, cutoff, weight_name);

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
	const py::object& cutoff_,
	const py::object& weight_name_,
	const py::object& num_thread_)
{
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
						unordered_map<int, double> result = multi_source_dijkstra_cost(cur_list, target, cutoff, weight_name);

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

vector<unordered_map<int, double>> GraphAlgorithms::multi_multi_source_cost_centroid(
	const py::object& list_o_,
	const py::object& method_,
	const py::object& target_,
	const py::object& cutoff_,
	const py::object& weight_name_,
	const py::object& num_thread_
)
{
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
						unordered_map<int, double> result = multi_source_dijkstra_cost_centroid(cur_list, target, cutoff, weight_name);

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
	const py::object& cutoff_,
	const py::object& weight_name_,
	const py::object& num_thread_
)
{	
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
						unordered_map<int, vector<int>> result = multi_source_dijkstra_path(cur_list, target, cutoff, weight_name);

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
	const py::object& cutoff_,
	const py::object& weight_name_,
	const py::object& num_thread_
)
{
	auto list_o = list_o_.cast<vector<vector<int>>>();
	auto method = method_.cast<string>();
	auto target = target_.cast<int>();
	auto cutoff = cutoff_.cast<double>();
	auto weight_name = weight_name_.cast<string>();
	auto num_thread = num_thread_.cast<int>();

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
						dis_and_path result = multi_source_dijkstra(cur_list, target, cutoff, weight_name);

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

py::array_t<double>  GraphAlgorithms::cost_matrix_to_numpy(
	const py::object& starts_,
	const py::object& ends_,
	const py::object& method_,
	const py::object& cutoff_,
	const py::object& weight_name_,
	const py::object& num_thread_
)
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
		vector<unordered_map<int, double>> multi_result = multi_multi_source_cost_centroid(multi_list_obj, method_, target_, cutoff_, weight_name_, num_thread_);

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

py::dict GraphAlgorithms::path_list_to_numpy(
	const py::object& starts_,
	const py::object& ends_,
	const py::object& method_,
	const py::object& cutoff_,
	const py::object& weight_name_,
	const py::object& num_thread_
)
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

	vector<unordered_map<int, vector<int>>> multi_result = multi_multi_source_path(multi_list_obj, method_, target_, cutoff_, weight_name_, num_thread_);

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

// 查找最短路径
vector<vector<int>>  GraphAlgorithms::shortest_simple_paths(
	const py::object& start_,
	const py::object& end_,
	const py::object& weight_name_)
{
	int start, end;
	string weight_name;

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

// 调用方法 ---------------------------------------------------------------------------------------

// test ------------------------------------------------------------------------------

unordered_map<int, double> GraphAlgorithms::test1(
	const vector<int>& sources,
	int target,
	double cutoff,
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
		if (d > cutoff) continue;

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
	const py::object& cutoff_,
	const py::object& weight_name_,
	const py::object& num_thread_)
{
	auto list_o = list_o_.cast<vector<int>>();
	auto method = method_.cast<string>();
	auto target = target_.cast<int>();
	auto cutoff = cutoff_.cast<double>();
	auto weight_name = weight_name_.cast<string>();
	auto num_thread = num_thread_.cast<int>();

	// 权重处理
	auto start = chrono::steady_clock::now();
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
				result = test1(cur_list, target, cutoff, weight_name);
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