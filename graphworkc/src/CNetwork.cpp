#include "CNetwork.h"

vector<std::string> CNetwork::SplitString(const std::string& str, char delimiter) {
	std::vector<std::string> tokens;
	std::istringstream stream(str);
	std::string token;
	while (std::getline(stream, token, delimiter)) {
		tokens.push_back(token);
	}
	return tokens;
}

void CNetwork::ReadNodeCSV(const std::string& filename) {
	std::ifstream file(filename);

	if (!file.is_open()) {
		std::cerr << "无法打开文件: " << filename << std::endl;
		return;
	}

	std::string line;

	// 解析标题行，获取列索引
	std::unordered_map<std::string, int> columnIndex;
	if (std::getline(file, line)) {
		std::stringstream ss(line);
		std::string columnName;
		int index = 0;
		while (std::getline(ss, columnName, ',')) {
			columnIndex[columnName] = index++;
		}
	}

	// 检查是否包含所需列
	if (columnIndex.find("ID") == columnIndex.end()) {
		std::cerr << "CSV 文件缺少必要的列: ID" << std::endl;
		return;
	}

	int idIndex = columnIndex["ID"];

	// 逐行读取数据
	while (std::getline(file, line)) {
		std::stringstream ss(line);
		std::string cell;
		std::vector<std::string> rowData;

		// 按行解析所有列
		while (std::getline(ss, cell, ',')) {
			rowData.push_back(cell);
		}

		// 检查所需列是否为空
		if (rowData.size() <= idIndex || rowData[idIndex].empty()) {
			// 跳过当前行
			continue;
		}

		// 提取需要的列
		CNode node;
		int nodeID;
		try {
			nodeID = std::stoi(rowData[idIndex]);
		}
		catch (const std::exception& e) {
			// 如果转换失败（比如数据格式不正确），跳过当前行
			std::cerr << "解析行失败，跳过该行: " << line << " 错误: " << e.what() << std::endl;
			continue;
		}

		// 确保 m_Node 的大小能够容纳该 ID
		if (nodeID >= m_Node.size()) {
			m_Node.resize(nodeID + 1); // 扩展到 nodeID + 1 的大小
		}

		// 设置节点的 ID
		node.ID = nodeID;

		// 将节点放入对应索引
		m_Node[nodeID] = node;
	}

	// 更新节点总数
	m_nNode = m_Node.size();

	file.close();
}

void CNetwork::ReadLinkCSV(const std::string& DataPath) {
	std::ifstream file(DataPath);
	if (!file.is_open()) {
		throw std::runtime_error(DataPath + " does not exist!");
	}

	m_Link.clear();
	m_nLink = 0;

	std::string row;
	std::unordered_map<std::string, int> columnIndex;

	// 读取文件头以获取列索引
	if (std::getline(file, row)) {
		auto header = SplitString(row, ',');
		for (size_t i = 0; i < header.size(); ++i) {
			columnIndex[header[i]] = i;
		}
	}
	else {
		throw std::runtime_error("File is empty or missing header row!");
	}

	// 检查需要的列是否存在
	const std::vector<std::string> requiredColumns = {
		"FROM_NODE",
		"TO_NODE",
		"AB_CAPACIT",
		"BA_CAPACIT",
		"AB_FFT",
		"BA_FFT",
		"DIR",
		"ALPHA",
		"BETA"
	};

	for (const auto& col : requiredColumns) {
		if (columnIndex.find(col) == columnIndex.end()) {
			throw std::runtime_error("Missing required column: " + col);
		}
	}

	// 动态定位列索引
	int FromNodeIdx = columnIndex["FROM_NODE"];
	int ToNodeIdx = columnIndex["TO_NODE"];
	int AbCpacityIdx = columnIndex["AB_CAPACIT"];
	int BaCpacityIdx = columnIndex["BA_CAPACIT"];
	int AbFftIdx = columnIndex["AB_FFT"];
	int BaFftIdx = columnIndex["BA_FFT"];
	int DirIdx = columnIndex["DIR"];
	int alphaIdx = columnIndex["ALPHA"];
	int betaIdx = columnIndex["BETA"];

	// 读取数据行
	while (std::getline(file, row)) {
		if (row.empty()) continue;

		auto data = SplitString(row, ',');

		// 检查行数据是否完整
		if (data.size() <= std::max({ FromNodeIdx, ToNodeIdx, AbCpacityIdx, BaCpacityIdx, AbFftIdx, BaFftIdx, DirIdx })) {
			std::cerr << "Invalid row: " << row << std::endl;
			continue;
		}

		int dir = std::stoi(data[DirIdx]);
		int inNodeIndex = std::stoi(data[FromNodeIdx]);
		int outNodeIndex = std::stoi(data[ToNodeIdx]);

		if (inNodeIndex < 0 || inNodeIndex >= m_Node.size() ||
			outNodeIndex < 0 || outNodeIndex >= m_Node.size()) {
			continue;
		}

		if (dir == 1 || dir == 0) {
			// 单向或双向的正向路段
			CLink pLink;
			pLink.ID = m_nLink++;
			pLink.pInNode = make_shared<CNode>(m_Node[inNodeIndex]);
			pLink.pOutNode = make_shared<CNode>(m_Node[outNodeIndex]);
			pLink.FreeFlowTravelTime = std::stod(data[AbFftIdx]);
			pLink.TravelTime = std::stod(data[AbFftIdx]);
			pLink.Capacity = std::stod(data[AbCpacityIdx]);
			pLink.Alpha = std::stod(data[alphaIdx]);
			pLink.Power = std::stod(data[betaIdx]);

			// 更新节点的出向和入向路段
			pLink.pInNode->OutgoingLink.push_back(pLink.ID);
			m_Node[pLink.pInNode->ID].OutgoingLink.push_back(pLink.ID);
			pLink.pOutNode->IncomingLink.push_back(pLink.ID);
			m_Node[pLink.pOutNode->ID].IncomingLink.push_back(pLink.ID);

			// 添加路段到集合
			m_Link.push_back(pLink);
		}

		if (dir == 0 || dir == -1) {
			// 双向路段的反向路段
			CLink reverseLink;
			reverseLink.ID = m_nLink++;
			reverseLink.pInNode = make_shared<CNode>(m_Node[outNodeIndex]);
			reverseLink.pOutNode = make_shared<CNode>(m_Node[inNodeIndex]);
			reverseLink.FreeFlowTravelTime = std::stod(data[BaFftIdx]);
			reverseLink.TravelTime = std::stod(data[BaFftIdx]);
			reverseLink.Capacity = std::stod(data[BaCpacityIdx]);
			reverseLink.Alpha = std::stod(data[alphaIdx]);
			reverseLink.Power = std::stod(data[betaIdx]);

			// 更新节点的出向和入向路段
			reverseLink.pInNode->OutgoingLink.push_back(reverseLink.ID);
			m_Node[reverseLink.pInNode->ID].OutgoingLink.push_back(reverseLink.ID);
			reverseLink.pOutNode->IncomingLink.push_back(reverseLink.ID);
			m_Node[reverseLink.pOutNode->ID].IncomingLink.push_back(reverseLink.ID);

			// 添加路段到集合
			m_Link.push_back(reverseLink);
		}

	}

	file.close();

	InitializeLinkIndex();
}

void CNetwork::ReadODpairsCSV(const std::string& DataPath) {
	// 检查文件是否存在
	std::ifstream file(DataPath);
	if (!file.is_open()) {
		std::cerr << DataPath << " does not exist!" << std::endl;
		return;
	}
	// 初始化变量
	m_Origin.clear();
	m_nOrigin = 0;

	std::string line;
	std::unordered_map<std::string, int> columnIndex;
	bool headerProcessed = false;

	// 读取文件内容
	while (std::getline(file, line)) {
		if (line.empty()) continue;

		std::istringstream lineStream(line);
		std::string cell;
		std::vector<std::string> row;

		// 按逗号分割每一行
		while (std::getline(lineStream, cell, ',')) {
			row.push_back(cell);
		}

		// 处理头部行
		if (!headerProcessed) {
			for (size_t i = 0; i < row.size(); ++i) {
				columnIndex[row[i]] = i;
			}

			// 检查必要的列是否存在
			if (columnIndex.find("o_node_id") == columnIndex.end() ||
				columnIndex.find("d_node_id") == columnIndex.end() ||
				columnIndex.find("volume") == columnIndex.end()) {
				std::cerr << "Missing required columns: o_node_id, d_node_id, or volume." << std::endl;
				return;
			}
			headerProcessed = true;
			continue;
		}

		// 检查是否有空数据
		bool hasEmptyCell = std::any_of(row.begin(), row.end(), [](const std::string& cell) {
			return cell.empty();
			});
		if (hasEmptyCell) {
			std::cerr << "Skipping row due to empty cell: " << line << std::endl;
			continue;
		}

		// 提取所需列数据
		int o_zone_id = std::stoi(row[columnIndex["o_node_id"]]);
		int d_zone_id = std::stoi(row[columnIndex["d_node_id"]]);
		double volume = std::stod(row[columnIndex["volume"]]);

		// 获取起点和终点节点指针
		CNode* oNode = &m_Node[o_zone_id];
		CNode* dNode = &m_Node[d_zone_id];

		// 处理起点节点
		if (oNode->Origin_ID == -1) {
			COrigin newOrigin;
			newOrigin.ID = m_nOrigin++;
			newOrigin.pOriginNode = oNode;
			oNode->Origin_ID = newOrigin.ID;
			m_Origin.push_back(newOrigin);
		}

		// 获取对应的起点对象
		COrigin& pOrigin = m_Origin[oNode->Origin_ID];

		// 添加目的地节点和需求
		pOrigin.DestinationNode.push_back(dNode->ID);
		pOrigin.ODDemand.push_back(volume);
	}

	file.close();
}

void CNetwork::DisplayInfo() const {
	cout << "Number of Nodes: " << m_nNode << endl;
	cout << "Number of Links: " << m_nLink << endl;
}

void CNetwork::AddEdgeFrom(const py::tuple& t) {
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
	// 检查并添加起点节点
	if (find(m_Node.begin(), m_Node.end(), first) == m_Node.end()) {
		CNode newNode;
		newNode.ID = first; // 设置节点ID
		if (first >= m_Node.size()) {
			m_Node.resize(first + 1); // 扩展到 nodeID + 1 的大小
		}
		m_Node[first] = newNode.ID;
		m_nNode++; // 更新节点总数
	}
	// 检查并添加终点节点
	if (find(m_Node.begin(), m_Node.end(), second) == m_Node.end()) {
		CNode newNode1;
		newNode1.ID = second; // 设置节点ID
		if (second >= m_Node.size()) {
			m_Node.resize(second + 1); // 扩展到 nodeID + 1 的大小
		}
		m_Node[second] = newNode1.ID;
		m_nNode++; // 更新节点总数
	}


	// 创建新的 CLink 实例
	CLink newLink;
	newLink.ID = m_nLink++;
	newLink.pInNode = make_shared<CNode>(m_Node[first]);
	newLink.pOutNode = make_shared<CNode>(m_Node[second]);
	newLink.TravelTime = travelTime;
	newLink.Capacity = 9999;

	// 更新节点的出向和入向路段
	newLink.pInNode->OutgoingLink.push_back(newLink.ID);
	m_Node[newLink.pInNode->ID].OutgoingLink.push_back(newLink.ID);
	newLink.pOutNode->IncomingLink.push_back(newLink.ID);
	m_Node[newLink.pOutNode->ID].IncomingLink.push_back(newLink.ID);

	// 将新建的路段加入网络集合
	m_Link.push_back(newLink);
	for (const CLink& link : m_Link) {
		LinkIndex[{link.pInNode->ID, link.pOutNode->ID}] = link.ID;
	}
}

void CNetwork::AddEdgeFromC(int first,int second,double travelTime) {

	// 检查并添加起点节点
	if (find(m_Node.begin(), m_Node.end(), first) == m_Node.end()) {
		CNode newNode;
		newNode.ID = first; // 设置节点ID
		if (first >= m_Node.size()) {
			m_Node.resize(first + 1); // 扩展到 nodeID + 1 的大小
		}
		m_Node[first] = newNode.ID;
		m_nNode++; // 更新节点总数
	}
	// 检查并添加终点节点
	if (find(m_Node.begin(), m_Node.end(), second) == m_Node.end()) {
		CNode newNode1;
		newNode1.ID = second; // 设置节点ID
		if (second >= m_Node.size()) {
			m_Node.resize(second + 1); // 扩展到 nodeID + 1 的大小
		}
		m_Node[second] = newNode1.ID;
		m_nNode++; // 更新节点总数
	}


	// 创建新的 CLink 实例
	CLink newLink;
	newLink.ID = m_nLink++;
	newLink.pInNode = make_shared<CNode>(m_Node[first]);
	newLink.pOutNode = make_shared<CNode>(m_Node[second]);
	newLink.TravelTime = travelTime;
	newLink.Capacity = 9999;

	// 更新节点的出向和入向路段
	newLink.pInNode->OutgoingLink.push_back(newLink.ID);
	m_Node[newLink.pInNode->ID].OutgoingLink.push_back(newLink.ID);
	newLink.pOutNode->IncomingLink.push_back(newLink.ID);
	m_Node[newLink.pOutNode->ID].IncomingLink.push_back(newLink.ID);

	// 将新建的路段加入网络集合
	m_Link.push_back(newLink);
	for (const CLink& link : m_Link) {
		LinkIndex[{link.pInNode->ID, link.pOutNode->ID}] = link.ID;
	}
}

std::pair<std::vector<double>, std::vector<int>> CNetwork::ComputeShortestPathsFromSource(int Start) {
	// 单源最短路径算法
	const int nNode = m_Node.size(); // 网络中节点的总数
	const int nLink = m_Link.size(); // 网络中路段（Link）的总数

	std::priority_queue<NodeDistancePair, std::vector<NodeDistancePair>, std::greater<NodeDistancePair>> pq;

	std::vector<double> ShortestPathCost(nNode, std::numeric_limits<double>::max());
	std::vector<int> ShortestPathParent(nNode, -1);

	ShortestPathCost[Start] = 0.0; // 起点到自身的代价为 0
	pq.push({ Start, 0.0 }); // 将起点加入优先队列

	while (!pq.empty()) {
		int currentNodeID = pq.top().nodeID; // 当前节点的 ID
		double currentDistance = pq.top().distance; // 当前节点到起点的距离
		pq.pop(); // 获取优先队列中代价最小的节点及其代价
		// 遍历当前节点的所有出边
		CNode& node = m_Node[currentNodeID];
		for (int linkIndex : node.OutgoingLink) {
			const CLink& pLink = m_Link[linkIndex];
			int nextNodeID = pLink.pOutNode->ID; // 获取路段
			double nextDistance = currentDistance + pLink.TravelTime; // 获取路段的目标节点
			// 如果发现通过当前节点 currentNodeID 到达目标节点 nextNodeID 的代价 nextDistance 小于当前记录的代价
			if (nextDistance < ShortestPathCost[nextNodeID]) {
				ShortestPathCost[nextNodeID] = nextDistance; // 更新最短路径代价
				ShortestPathParent[nextNodeID] = currentNodeID; // 更新父节点
				pq.push({ nextNodeID, nextDistance }); // 将目标节点加入优先队列
			}
		}
	}

	return { ShortestPathCost, ShortestPathParent }; // ShortestPathParent 是一个数组，它存储了从起点（Start）到网络中每个节点的最短路径上各节点的“父节点”
}

std::vector<int> CNetwork::ReconstructPath(int Start, int End, const std::vector<int>& ShortestPathParent) {
	std::vector<int> ShortestPath_Link;// 存储路径上的link-ID
	int point_out = End;

	if (ShortestPathParent[point_out] == -1) {
		return ShortestPath_Link;
	}

	while (point_out != Start) {
		int point_in = ShortestPathParent[point_out];
		if (point_in == -1) break;

		auto it = LinkIndex.find({ point_in, point_out });
		if (it != LinkIndex.end()) {
			ShortestPath_Link.insert(ShortestPath_Link.begin(), it->second);
		}

		point_out = point_in;
	}

	return ShortestPath_Link;
}

tuple<double,vector<int>> CNetwork::single_source_dijkstra(int Start,int End) {
	unordered_map<int, double> dist;
	unordered_map<int, vector<int>> paths;
	auto result = CNetwork::ComputeShortestPathsFromSource(Start);
	const vector<double>& ShortestPathCost = result.first;
	const vector<int>& ShortestPathParent = result.second;
	for (int i = 0; i < ShortestPathCost.size(); i++) {
		dist[i] = ShortestPathCost[i];
	}
	for (int i = 0; i < CNetwork::m_Node.size(); i++) {
		vector<int> ShortestPath_Link = CNetwork::ReconstructPath(Start, CNetwork::m_Node[i].ID, ShortestPathParent);
		vector<int> shortestPath;
		for (int j = 0; j < ShortestPath_Link.size(); j++) {
			shortestPath.push_back(CNetwork::m_Link[ShortestPath_Link[j]].pInNode->ID);
		}
		shortestPath.push_back(CNetwork::m_Node[i].ID);
		paths[CNetwork::m_Node[i].ID] = shortestPath;
	}
	return make_tuple(dist[End], paths[End]);
}

tuple<unordered_map<int, double>, unordered_map<int, vector<int>>> CNetwork::single_source_dijkstra(int Start) {
	unordered_map<int, double> dist;
	unordered_map<int, vector<int>> paths;
	auto result = CNetwork::ComputeShortestPathsFromSource(Start);
	const vector<double>& ShortestPathCost = result.first;
	const vector<int>& ShortestPathParent = result.second;
	for (int i = 0; i < ShortestPathCost.size(); i++) {
		dist[i] = ShortestPathCost[i];
	}
	for (int i = 0; i < CNetwork::m_Node.size(); i++) {
		vector<int> ShortestPath_Link = CNetwork::ReconstructPath(Start, CNetwork::m_Node[i].ID, ShortestPathParent);
		vector<int> shortestPath;
		for (int j = 0; j < ShortestPath_Link.size(); j++) {
			shortestPath.push_back(CNetwork::m_Link[ShortestPath_Link[j]].pInNode->ID);
		}
		shortestPath.push_back(CNetwork::m_Node[i].ID);
		paths[CNetwork::m_Node[i].ID] = shortestPath;
	}
	return make_tuple(dist, paths);
}
