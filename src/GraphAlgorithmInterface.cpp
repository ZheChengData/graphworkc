#include "GraphAlgorithmInterface.h"
// 调用方法 ---------------------------------------------------------------------------------------

// 单源最短路
py::dict GraphAlgorithmInterface::single_source_cost(
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


unordered_map<int, vector<int>> GraphAlgorithmInterface::single_source_path(
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


dis_and_path GraphAlgorithmInterface::single_source_all(
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
py::dict GraphAlgorithmInterface::multi_source_cost(
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


unordered_map<int, vector<int>> GraphAlgorithmInterface::multi_source_path(
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


dis_and_path GraphAlgorithmInterface::multi_source_all(
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
vector<unordered_map<int, double>> GraphAlgorithmInterface::multi_single_source_cost(
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


vector<unordered_map<int, vector<int>>> GraphAlgorithmInterface::multi_single_source_path(
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


vector<dis_and_path> GraphAlgorithmInterface::multi_single_source_all(
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
vector<unordered_map <int, double>> GraphAlgorithmInterface::multi_multi_source_cost(
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


vector<unordered_map<int, vector<int>>> GraphAlgorithmInterface::multi_multi_source_path(
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


vector<dis_and_path> GraphAlgorithmInterface::multi_multi_source_all(
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
vector<unordered_map<int, double>> GraphAlgorithmInterface::multi_multi_source_cost_centroid(
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
py::array_t<double>  GraphAlgorithmInterface::cost_matrix(
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
py::dict GraphAlgorithmInterface::path_dict(
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
py::dict GraphAlgorithmInterface::path_dict_pairwise(
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
vector<vector<int>> GraphAlgorithmInterface::k_shortest_paths(
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
double GraphAlgorithmInterface::shortest_path_cost(
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
vector<int> GraphAlgorithmInterface::shortest_path_path(
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
pair<double, vector<int>> GraphAlgorithmInterface::shortest_path_all(
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
