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
		std::cerr << "�޷����ļ�: " << filename << std::endl;
		return;
	}

	std::string line;

	// ���������У���ȡ������
	std::unordered_map<std::string, int> columnIndex;
	if (std::getline(file, line)) {
		std::stringstream ss(line);
		std::string columnName;
		int index = 0;
		while (std::getline(ss, columnName, ',')) {
			columnIndex[columnName] = index++;
		}
	}

	// ����Ƿ����������
	if (columnIndex.find("ID") == columnIndex.end()) {
		std::cerr << "CSV �ļ�ȱ�ٱ�Ҫ����: ID" << std::endl;
		return;
	}

	int idIndex = columnIndex["ID"];

	// ���ж�ȡ����
	while (std::getline(file, line)) {
		std::stringstream ss(line);
		std::string cell;
		std::vector<std::string> rowData;

		// ���н���������
		while (std::getline(ss, cell, ',')) {
			rowData.push_back(cell);
		}

		// ����������Ƿ�Ϊ��
		if (rowData.size() <= idIndex || rowData[idIndex].empty()) {
			// ������ǰ��
			continue;
		}

		// ��ȡ��Ҫ����
		CNode node;
		int nodeID;
		try {
			nodeID = std::stoi(rowData[idIndex]);
		}
		catch (const std::exception& e) {
			// ���ת��ʧ�ܣ��������ݸ�ʽ����ȷ����������ǰ��
			std::cerr << "������ʧ�ܣ���������: " << line << " ����: " << e.what() << std::endl;
			continue;
		}

		// ȷ�� m_Node �Ĵ�С�ܹ����ɸ� ID
		if (nodeID >= m_Node.size()) {
			m_Node.resize(nodeID + 1); // ��չ�� nodeID + 1 �Ĵ�С
		}

		// ���ýڵ�� ID
		node.ID = nodeID;

		// ���ڵ�����Ӧ����
		m_Node[nodeID] = node;
	}

	// ���½ڵ�����
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

	// ��ȡ�ļ�ͷ�Ի�ȡ������
	if (std::getline(file, row)) {
		auto header = SplitString(row, ',');
		for (size_t i = 0; i < header.size(); ++i) {
			columnIndex[header[i]] = i;
		}
	}
	else {
		throw std::runtime_error("File is empty or missing header row!");
	}

	// �����Ҫ�����Ƿ����
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

	// ��̬��λ������
	int FromNodeIdx = columnIndex["FROM_NODE"];
	int ToNodeIdx = columnIndex["TO_NODE"];
	int AbCpacityIdx = columnIndex["AB_CAPACIT"];
	int BaCpacityIdx = columnIndex["BA_CAPACIT"];
	int AbFftIdx = columnIndex["AB_FFT"];
	int BaFftIdx = columnIndex["BA_FFT"];
	int DirIdx = columnIndex["DIR"];
	int alphaIdx = columnIndex["ALPHA"];
	int betaIdx = columnIndex["BETA"];

	// ��ȡ������
	while (std::getline(file, row)) {
		if (row.empty()) continue;

		auto data = SplitString(row, ',');

		// ����������Ƿ�����
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
			// �����˫�������·��
			CLink pLink;
			pLink.ID = m_nLink++;
			pLink.pInNode = make_shared<CNode>(m_Node[inNodeIndex]);
			pLink.pOutNode = make_shared<CNode>(m_Node[outNodeIndex]);
			pLink.FreeFlowTravelTime = std::stod(data[AbFftIdx]);
			pLink.TravelTime = std::stod(data[AbFftIdx]);
			pLink.Capacity = std::stod(data[AbCpacityIdx]);
			pLink.Alpha = std::stod(data[alphaIdx]);
			pLink.Power = std::stod(data[betaIdx]);

			// ���½ڵ�ĳ��������·��
			pLink.pInNode->OutgoingLink.push_back(pLink.ID);
			m_Node[pLink.pInNode->ID].OutgoingLink.push_back(pLink.ID);
			pLink.pOutNode->IncomingLink.push_back(pLink.ID);
			m_Node[pLink.pOutNode->ID].IncomingLink.push_back(pLink.ID);

			// ���·�ε�����
			m_Link.push_back(pLink);
		}

		if (dir == 0 || dir == -1) {
			// ˫��·�εķ���·��
			CLink reverseLink;
			reverseLink.ID = m_nLink++;
			reverseLink.pInNode = make_shared<CNode>(m_Node[outNodeIndex]);
			reverseLink.pOutNode = make_shared<CNode>(m_Node[inNodeIndex]);
			reverseLink.FreeFlowTravelTime = std::stod(data[BaFftIdx]);
			reverseLink.TravelTime = std::stod(data[BaFftIdx]);
			reverseLink.Capacity = std::stod(data[BaCpacityIdx]);
			reverseLink.Alpha = std::stod(data[alphaIdx]);
			reverseLink.Power = std::stod(data[betaIdx]);

			// ���½ڵ�ĳ��������·��
			reverseLink.pInNode->OutgoingLink.push_back(reverseLink.ID);
			m_Node[reverseLink.pInNode->ID].OutgoingLink.push_back(reverseLink.ID);
			reverseLink.pOutNode->IncomingLink.push_back(reverseLink.ID);
			m_Node[reverseLink.pOutNode->ID].IncomingLink.push_back(reverseLink.ID);

			// ���·�ε�����
			m_Link.push_back(reverseLink);
		}

	}

	file.close();

	InitializeLinkIndex();
}

void CNetwork::ReadODpairsCSV(const std::string& DataPath) {
	// ����ļ��Ƿ����
	std::ifstream file(DataPath);
	if (!file.is_open()) {
		std::cerr << DataPath << " does not exist!" << std::endl;
		return;
	}
	// ��ʼ������
	m_Origin.clear();
	m_nOrigin = 0;

	std::string line;
	std::unordered_map<std::string, int> columnIndex;
	bool headerProcessed = false;

	// ��ȡ�ļ�����
	while (std::getline(file, line)) {
		if (line.empty()) continue;

		std::istringstream lineStream(line);
		std::string cell;
		std::vector<std::string> row;

		// �����ŷָ�ÿһ��
		while (std::getline(lineStream, cell, ',')) {
			row.push_back(cell);
		}

		// ����ͷ����
		if (!headerProcessed) {
			for (size_t i = 0; i < row.size(); ++i) {
				columnIndex[row[i]] = i;
			}

			// ����Ҫ�����Ƿ����
			if (columnIndex.find("o_node_id") == columnIndex.end() ||
				columnIndex.find("d_node_id") == columnIndex.end() ||
				columnIndex.find("volume") == columnIndex.end()) {
				std::cerr << "Missing required columns: o_node_id, d_node_id, or volume." << std::endl;
				return;
			}
			headerProcessed = true;
			continue;
		}

		// ����Ƿ��п�����
		bool hasEmptyCell = std::any_of(row.begin(), row.end(), [](const std::string& cell) {
			return cell.empty();
			});
		if (hasEmptyCell) {
			std::cerr << "Skipping row due to empty cell: " << line << std::endl;
			continue;
		}

		// ��ȡ����������
		int o_zone_id = std::stoi(row[columnIndex["o_node_id"]]);
		int d_zone_id = std::stoi(row[columnIndex["d_node_id"]]);
		double volume = std::stod(row[columnIndex["volume"]]);

		// ��ȡ�����յ�ڵ�ָ��
		CNode* oNode = &m_Node[o_zone_id];
		CNode* dNode = &m_Node[d_zone_id];

		// �������ڵ�
		if (oNode->Origin_ID == -1) {
			COrigin newOrigin;
			newOrigin.ID = m_nOrigin++;
			newOrigin.pOriginNode = oNode;
			oNode->Origin_ID = newOrigin.ID;
			m_Origin.push_back(newOrigin);
		}

		// ��ȡ��Ӧ��������
		COrigin& pOrigin = m_Origin[oNode->Origin_ID];

		// ���Ŀ�ĵؽڵ������
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

	// ����Ԫ��ĵ�һ���͵ڶ���Ԫ�أ������յ�ID��
	int first = t[0].cast<int>(); // ���ID
	int second = t[1].cast<int>(); // �յ�ID

	// ����Ԫ��ĵ�����Ԫ�أ��ֵ����ͣ�
	py::dict third = t[2].cast<py::dict>();

	// ���ֵ��л�ȡ "weight" ����Ӧ��ֵ
	if (!third.contains("weight")) {
		throw std::runtime_error("Dictionary must contain a 'weight' key");
	}
	double travelTime = third["weight"].cast<double>();
	// ��鲢������ڵ�
	if (find(m_Node.begin(), m_Node.end(), first) == m_Node.end()) {
		CNode newNode;
		newNode.ID = first; // ���ýڵ�ID
		if (first >= m_Node.size()) {
			m_Node.resize(first + 1); // ��չ�� nodeID + 1 �Ĵ�С
		}
		m_Node[first] = newNode.ID;
		m_nNode++; // ���½ڵ�����
	}
	// ��鲢����յ�ڵ�
	if (find(m_Node.begin(), m_Node.end(), second) == m_Node.end()) {
		CNode newNode1;
		newNode1.ID = second; // ���ýڵ�ID
		if (second >= m_Node.size()) {
			m_Node.resize(second + 1); // ��չ�� nodeID + 1 �Ĵ�С
		}
		m_Node[second] = newNode1.ID;
		m_nNode++; // ���½ڵ�����
	}


	// �����µ� CLink ʵ��
	CLink newLink;
	newLink.ID = m_nLink++;
	newLink.pInNode = make_shared<CNode>(m_Node[first]);
	newLink.pOutNode = make_shared<CNode>(m_Node[second]);
	newLink.TravelTime = travelTime;
	newLink.Capacity = 9999;

	// ���½ڵ�ĳ��������·��
	newLink.pInNode->OutgoingLink.push_back(newLink.ID);
	m_Node[newLink.pInNode->ID].OutgoingLink.push_back(newLink.ID);
	newLink.pOutNode->IncomingLink.push_back(newLink.ID);
	m_Node[newLink.pOutNode->ID].IncomingLink.push_back(newLink.ID);

	// ���½���·�μ������缯��
	m_Link.push_back(newLink);
	for (const CLink& link : m_Link) {
		LinkIndex[{link.pInNode->ID, link.pOutNode->ID}] = link.ID;
	}
}

void CNetwork::AddEdgeFromC(int first,int second,double travelTime) {

	// ��鲢������ڵ�
	if (find(m_Node.begin(), m_Node.end(), first) == m_Node.end()) {
		CNode newNode;
		newNode.ID = first; // ���ýڵ�ID
		if (first >= m_Node.size()) {
			m_Node.resize(first + 1); // ��չ�� nodeID + 1 �Ĵ�С
		}
		m_Node[first] = newNode.ID;
		m_nNode++; // ���½ڵ�����
	}
	// ��鲢����յ�ڵ�
	if (find(m_Node.begin(), m_Node.end(), second) == m_Node.end()) {
		CNode newNode1;
		newNode1.ID = second; // ���ýڵ�ID
		if (second >= m_Node.size()) {
			m_Node.resize(second + 1); // ��չ�� nodeID + 1 �Ĵ�С
		}
		m_Node[second] = newNode1.ID;
		m_nNode++; // ���½ڵ�����
	}


	// �����µ� CLink ʵ��
	CLink newLink;
	newLink.ID = m_nLink++;
	newLink.pInNode = make_shared<CNode>(m_Node[first]);
	newLink.pOutNode = make_shared<CNode>(m_Node[second]);
	newLink.TravelTime = travelTime;
	newLink.Capacity = 9999;

	// ���½ڵ�ĳ��������·��
	newLink.pInNode->OutgoingLink.push_back(newLink.ID);
	m_Node[newLink.pInNode->ID].OutgoingLink.push_back(newLink.ID);
	newLink.pOutNode->IncomingLink.push_back(newLink.ID);
	m_Node[newLink.pOutNode->ID].IncomingLink.push_back(newLink.ID);

	// ���½���·�μ������缯��
	m_Link.push_back(newLink);
	for (const CLink& link : m_Link) {
		LinkIndex[{link.pInNode->ID, link.pOutNode->ID}] = link.ID;
	}
}

std::pair<std::vector<double>, std::vector<int>> CNetwork::ComputeShortestPathsFromSource(int Start) {
	// ��Դ���·���㷨
	const int nNode = m_Node.size(); // �����нڵ������
	const int nLink = m_Link.size(); // ������·�Σ�Link��������

	std::priority_queue<NodeDistancePair, std::vector<NodeDistancePair>, std::greater<NodeDistancePair>> pq;

	std::vector<double> ShortestPathCost(nNode, std::numeric_limits<double>::max());
	std::vector<int> ShortestPathParent(nNode, -1);

	ShortestPathCost[Start] = 0.0; // ��㵽����Ĵ���Ϊ 0
	pq.push({ Start, 0.0 }); // �����������ȶ���

	while (!pq.empty()) {
		int currentNodeID = pq.top().nodeID; // ��ǰ�ڵ�� ID
		double currentDistance = pq.top().distance; // ��ǰ�ڵ㵽���ľ���
		pq.pop(); // ��ȡ���ȶ����д�����С�Ľڵ㼰�����
		// ������ǰ�ڵ�����г���
		CNode& node = m_Node[currentNodeID];
		for (int linkIndex : node.OutgoingLink) {
			const CLink& pLink = m_Link[linkIndex];
			int nextNodeID = pLink.pOutNode->ID; // ��ȡ·��
			double nextDistance = currentDistance + pLink.TravelTime; // ��ȡ·�ε�Ŀ��ڵ�
			// �������ͨ����ǰ�ڵ� currentNodeID ����Ŀ��ڵ� nextNodeID �Ĵ��� nextDistance С�ڵ�ǰ��¼�Ĵ���
			if (nextDistance < ShortestPathCost[nextNodeID]) {
				ShortestPathCost[nextNodeID] = nextDistance; // �������·������
				ShortestPathParent[nextNodeID] = currentNodeID; // ���¸��ڵ�
				pq.push({ nextNodeID, nextDistance }); // ��Ŀ��ڵ�������ȶ���
			}
		}
	}

	return { ShortestPathCost, ShortestPathParent }; // ShortestPathParent ��һ�����飬���洢�˴���㣨Start����������ÿ���ڵ�����·���ϸ��ڵ�ġ����ڵ㡱
}

std::vector<int> CNetwork::ReconstructPath(int Start, int End, const std::vector<int>& ShortestPathParent) {
	std::vector<int> ShortestPath_Link;// �洢·���ϵ�link-ID
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
