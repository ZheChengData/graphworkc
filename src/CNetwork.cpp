#include "CNetwork.h"

// 初始化LinkIndex
void CNetwork::InitializeLinkIndex() {
	for (const CLink& link : m_Link) {
		LinkIndex[{m_Node[link.InNodeIndex].ID, m_Node[link.OutNodeIndex].ID}] = link.ID;
	}
}

// 清空
void CNetwork::ClearAll() {
	m_Node.clear();
	m_Link.clear();
	m_nNode = 0;
	m_nLink = 0;
	m_path_result.clear();
	dic_cost.clear();
	dic_path.clear();
	ShortestPathCost.clear();
	ShortestPathParent.clear();
	ID2Index_map.clear();

	cout << "Finish clear" << endl;
}

// 移除一条边
void CNetwork::RemoveEdge(int first, int second) {
	// 检查起点和终点是否存在
	if (ID2Index_map.find(first) == ID2Index_map.end() || ID2Index_map.find(second) == ID2Index_map.end()) {
		throw std::runtime_error("The given nodes do not exist in the network");
	}

	// 找到起点和终点的索引
	size_t inNodeIndex = ID2Index_map[first];
	size_t outNodeIndex = ID2Index_map[second];

	// 在 m_Link 中查找对应的边
	bool linkFound = false;
	for (auto it = m_Link.begin(); it != m_Link.end(); ++it) {
		if (it->InNodeIndex == inNodeIndex && it->OutNodeIndex == outNodeIndex) {
			// 找到了要移除的边

			// 从起点节点的 OutgoingLink 中移除
			auto& outgoingLinks = m_Node[inNodeIndex].OutgoingLink;
			outgoingLinks.erase(std::remove(outgoingLinks.begin(), outgoingLinks.end(), it->ID), outgoingLinks.end());

			// 从终点节点的 IncomingLink 中移除
			auto& incomingLinks = m_Node[outNodeIndex].IncomingLink;
			incomingLinks.erase(std::remove(incomingLinks.begin(), incomingLinks.end(), it->ID), incomingLinks.end());

			// 从 m_Link 中删除该边
			//m_Link.erase(it);

			m_nLink -= 1;

			linkFound = true;
			break;
		}
	}

	if (!linkFound) {
		throw std::runtime_error("The specified edge does not exist");
	}

}

// 移除多条边
void CNetwork::RemoveEdges(const std::vector<std::pair<int, int>>& edges) {
	// 遍历每一条边并尝试移除
	for (const auto& edge : edges) {
		try {
			// 尝试移除每一条边
			RemoveEdge(edge.first, edge.second);
		}
		catch (const std::runtime_error& e) {
			// 如果某条边不存在，输出错误信息并继续
			std::cerr << "Error removing edge (" << edge.first << ", " << edge.second << "): " << e.what() << std::endl;
		}
	}
}

// 加一条边
void CNetwork::AddEdgeFromTuple(const py::tuple& t) {
	if (t.size() != 3) {
		throw std::runtime_error("Tuple must have exactly 3 elements");
	}

	// 解析元组的第一个和第二个元素（起点和终点ID）
	int first = t[0].cast<int>(); // 起点ID
	int second = t[1].cast<int>(); // 终点ID

	// 解析元组的第三个元素（字典类型）
	py::dict third = t[2].cast<py::dict>();

	// 从字典中获取 "weight" 键对应的值
	if (!third.contains("weight")) {
		throw std::runtime_error("Dictionary must contain a 'weight' key");
	}
	double travelTime = third["weight"].cast<double>();

	// 添加起点节点
	if (ID2Index_map.find(first) == ID2Index_map.end()) {
		CNode node1;
		node1.ID = first;
		m_Node.push_back(node1); // 将节点加入 m_Node 列表
		size_t index = m_Node.size() - 1;
		ID2Index_map[first] = index; // 更新 ID 到索引的映射
		m_nNode++; // 更新节点总数
	}

	// 添加终点节点
	if (ID2Index_map.find(second) == ID2Index_map.end()) {
		CNode node2;
		node2.ID = second;
		m_Node.push_back(node2); // 将节点加入 m_Node 列表
		size_t index = m_Node.size() - 1;
		ID2Index_map[second] = index; // 更新 ID 到索引的映射
		m_nNode++; // 更新节点总数
	}

	// 创建新的 CLink 实例
	CLink newLink;
	newLink.ID = m_nLink++;
	newLink.InNodeIndex = ID2Index_map[first];
	newLink.OutNodeIndex = ID2Index_map[second];
	newLink.TravelTime = travelTime;
	newLink.Capacity = 9999;

	// 更新节点的出向和入向路段
	m_Node[newLink.InNodeIndex].OutgoingLink.push_back(newLink.ID);
	m_Node[newLink.OutNodeIndex].IncomingLink.push_back(newLink.ID);

	// 将新建的路段加入网络集合
	m_Link.push_back(newLink);
	InitializeLinkIndex();
}

// 加多条边
void CNetwork::AddEdgesFromList(const std::vector<py::tuple>& tupleList) {
	// 遍历元组列表
	for (const auto& t : tupleList) {
		try {
			// 同步调用 AddEdgeFromTuple 处理每一个元组
			AddEdgeFromTuple(t);
		}
		catch (const std::runtime_error& e) {
			// 捕获单个元组处理中的错误，输出错误信息
			std::cerr << "Error processing tuple: " << py::str(t) << ". Error: " << e.what() << std::endl;
		}
	}
}


// 最短路径 迪杰斯特拉算法
void CNetwork::SingleSourceDijkstra(int Start) {
	// 清空容器
	dic_path.clear();
	dic_cost.clear();

	// 检查起点是否存在
	if (ID2Index_map.find(Start) == ID2Index_map.end()) {
		cout << "The start node does not exist: " << Start << endl;
	}

	std::priority_queue<NodeDistancePair, std::vector<NodeDistancePair>, std::greater<NodeDistancePair>> pq;
	std::vector<double> ShortestPathCost(m_nNode, std::numeric_limits<double>::max()); // 值为索引
	ShortestPathParent.resize(m_nNode, -1); // 确保类成员变量大小与节点数匹配 值为索引

	ShortestPathCost[ID2Index_map[Start]] = 0.0; // 起点到自身的代价为 0
	pq.push({ Start, 0.0 }); // 将起点加入优先队列


	// 主循环中，实时记录路径
	while (!pq.empty()) {
		int currentNodeID = pq.top().nodeID;
		double currentDistance = pq.top().distance;
		pq.pop();

		// 遍历当前节点的所有出边
		CNode& node = m_Node[ID2Index_map[currentNodeID]];
		for (int linkID : node.OutgoingLink) {
			const CLink& pLink = m_Link[linkID];
			int nextNodeID = m_Node[pLink.OutNodeIndex].ID;
			double nextDistance = currentDistance + pLink.TravelTime;

			if (nextDistance < ShortestPathCost[ID2Index_map[nextNodeID]]) {
				ShortestPathCost[ID2Index_map[nextNodeID]] = nextDistance;
				ShortestPathParent[ID2Index_map[nextNodeID]] = ID2Index_map[currentNodeID];
				pq.push({ nextNodeID, nextDistance });

				// 实时更新路径
				dic_path[nextNodeID] = dic_path[currentNodeID];
				dic_path[nextNodeID].push_back(nextNodeID);
			}
		}
	}



	for (int i = 0; i < m_Node.size(); i++) {
		int target_ID = m_Node[i].ID;
		dic_cost[target_ID] = ShortestPathCost[ID2Index_map[target_ID]];
	}

	m_path_result[Start].dict_cost = std::move(dic_cost);
	m_path_result[Start].dict_path = std::move(dic_path);
}

// 单源最短路径计算
void CNetwork::SingleSourcePath(int Start, string method) {
	// 算法方法判定
	if (method == "Dijkstra") {
		SingleSourceDijkstra(Start);
	}
	else {
		cout << "There is currently no such method available: " << method << endl;
	}


}

// 多源最短路径计算
void CNetwork::MultiSourcePath(const std::vector<int>& StartNodes, const std::string& method) {
	// 遍历所有起始节点
	for (int start : StartNodes) {
		// 检查起点是否存在
		if (ID2Index_map.find(start) == ID2Index_map.end()) {
			cout << "The start node does not exist: " << start << endl;
			continue; // 跳过不存在的起点
		}

		// 调用单源最短路径算法
		SingleSourcePath(start, method);
	}
}

// 生成路径花费矩阵
void CNetwork::CostMartixToCsv(const std::vector<int> vec_start, const std::vector<int> vec_end, const std::string file_path) {
	// 检查 m_path_result 是否为空
	if (m_path_result.empty()) {
		throw std::runtime_error("Path result data is empty. Cannot generate cost matrix.");
	}

	// 打开文件
	std::ofstream csv_file(file_path);
	if (!csv_file.is_open()) {
		throw std::runtime_error("Failed to open file for writing: " + file_path);
	}

	// 写入 CSV 表头
	csv_file << "Start/End";
	for (int end : vec_end) {
		csv_file << "," << end;
	}
	csv_file << "\n";

	// 遍历 vec_start，生成每一行
	for (int start : vec_start) {
		// 写入起点
		csv_file << start;

		// 检查是否存在对应的起点数据
		if (m_path_result.find(start) == m_path_result.end()) {
			// 起点不存在，填充无效值
			for (size_t i = 0; i < vec_end.size(); ++i) {
				csv_file << ",N/A";
			}
			csv_file << "\n";
			continue;
		}

		// 获取起点的 path_result
		const auto& path_result = m_path_result[start];

		// 遍历 vec_end，获取每一个终点的最短路径花费
		for (int end : vec_end) {
			if (path_result.dict_cost.find(end) != path_result.dict_cost.end()) {
				// 存在终点，写入花费
				csv_file << "," << std::fixed << std::setprecision(2) << path_result.dict_cost.at(end);
			}
			else {
				// 不存在终点，写入 N/A
				csv_file << ",N/A";
			}
		}
		csv_file << "\n";
	}

	// 关闭文件
	csv_file.close();
}

// 最短路径数据输出
void CNetwork::PathToCsv(const std::vector<int> vec_start, const std::vector<int> vec_end, const std::string file_path) {
	// 检查 m_path_result 是否为空
	if (m_path_result.empty()) {
		throw std::runtime_error("Path result data is empty. Cannot generate CSV.");
	}

	// 打开 CSV 文件
	std::ofstream csv_file(file_path);
	if (!csv_file.is_open()) {
		throw std::runtime_error("Failed to open file for writing: " + file_path);
	}

	// 写入表头
	csv_file << "Start,End,Cost,Path\n";

	// 遍历起点和终点列表
	for (int start : vec_start) {
		if (m_path_result.find(start) == m_path_result.end()) {
			// 起点数据不存在，跳过
			continue;
		}

		const auto& path_result = m_path_result[start];

		for (int end : vec_end) {
			csv_file << start << "," << end << ",";

			if (path_result.dict_path.find(end) != path_result.dict_path.end() &&
				path_result.dict_cost.find(end) != path_result.dict_cost.end()) {
				const auto& path = path_result.dict_path.at(end);
				double cost = path_result.dict_cost.at(end);

				// 写入成本
				csv_file << cost << ",";

				// 写入路径（用双引号包裹）
				std::ostringstream oss;
				oss << "[";
				for (size_t i = 0; i < path.size(); ++i) {
					oss << path[i];
					if (i < path.size() - 1) {
						oss << ", ";
					}
				}
				oss << "]";
				csv_file << "\"" << oss.str() << "\"";
			}
			else {
				csv_file << "N/A,N/A";
			}

			csv_file << "\n";
		}
	}

	// 关闭文件
	csv_file.close();
}
