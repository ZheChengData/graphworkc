#include <iostream>
#include "CNetwork.h"
#include "SelfFile.h"
using namespace std;
int main() {
	CNode Node;
	CNetwork Network;
	SelfFile File;
}

PYBIND11_MODULE(graphworkc, m) {
	m.doc() = "module using pybind11";
	py::class_<path_result>(m, "path_result", "A class representing the shortest path result between nodes.")
		.def(py::init<>()) 
		.def_readwrite("dict_cost", &path_result::dict_cost)
		.def_readwrite("dict_path", &path_result::dict_path);

	py::class_<CLink>(m, "CLink")
		.def(py::init<>())
		.def_readwrite("ID", &CLink::ID)
		.def_readwrite("InNodeIndex", &CLink::InNodeIndex)
		.def_readwrite("OutNodeIndex", &CLink::OutNodeIndex)
		.def_readwrite("TravelTime", &CLink::TravelTime);

	py::class_<CNode>(m, "CNode")
		.def(py::init<>())
		.def_readwrite("ID", &CNode::ID)
		.def_readwrite("IncomingLink", &CNode::IncomingLink)
		.def_readwrite("OutgoingLink", &CNode::OutgoingLink);
	

	py::class_<CNetwork>(m, "CNetwork")
		.def(py::init<>())
		.def_readwrite("m_Node", &CNetwork::m_Node)
		.def_readwrite("m_Link", &CNetwork::m_Link)
		.def_readwrite("m_nNode", &CNetwork::m_nNode)
		.def_readwrite("m_nLink", &CNetwork::m_nLink)

		// 存储所有源节点最短路径计算结果的变量
		.def_readwrite("m_path_result", &CNetwork::m_path_result, R"pbdoc(
		A variable that stores the shortest path cost and shortest path of all source nodes;
		
		Args:
			m_path_result (unordered_map < int, path_result>):
			dict_cost (unordered_map<int, double>):
			dict_path (unordered_map<int, std::vector<int>>):

		Example:
			// 获取源节点ID为1的花费和路径	
			ID1_cost = network.m_path_result[1].dict_cost;
			ID1_path = network.m_path_result[1].dict_path;
				
			// 获取源节点ID为2 到 ID为3的花费
			ID2_to_3_cost = network.m_path_result[2].dict_cost[3];
		)pbdoc")

		// 生成路径列表
		.def("PathToCsv", &CNetwork::PathToCsv, R"pbdoc(
		A function that create a list relative od shortest path to csv.
		
		Args:
			start_nodes  (vector<int>): all start nodes ;
			end_nodex    (vector<int>): all end nodes   ;
			csv_path     (string)     : Generated CSV path; 

		Example:
            network.CostMartixToCsv([1,3],[1,2,3,4],"cost.csv")

		)pbdoc")


		// 生成花费矩阵
		.def("CostMartixToCsv", &CNetwork::CostMartixToCsv, R"pbdoc(
		A function that create a martix relative od cost to csv.

		Args:
			start_nodes  (vector<int>): all start nodes ;
			end_nodex    (vector<int>): all end nodes   ;
			csv_path     (string)     : Generated CSV path; 
			
		Example:
            network.CostMartixToCsv([1,3],[1,2,3,4],"cost.csv")
								


		)pbdoc")

		// 移除多条边
		.def("RemoveEdges", &CNetwork::RemoveEdges, R"pbdoc(
		A function that remove edges.

		Args:
			edges (vector<pair<int, int>>): (starting node,ending node) pair of edges 

		Example:
            network.RemoveEdge([(1, 2),(2, 3),(4, 5)])		

		)pbdoc")

		// 移除一条边
		.def("RemoveEdge", &CNetwork::RemoveEdge, R"pbdoc(
		A function that remove one edge.
			
		Args:
			Start (int): The starting node.
			End (int): The ending node.

		Example:
            network.RemoveEdge(1, 2)

		)pbdoc")

		.def("ClearAll", &CNetwork::ClearAll,
			R"pbdoc(
		A function that clear all network.

		)pbdoc")

		// 多源最短路径计算
		.def("MultiSourcePath", &CNetwork::MultiSourcePath,
			R"pbdoc(
        A function that calculates the shortest paths from multiple source nodes to all other nodes.

        Args:
            StartNodes (list of int): A list of starting nodes.
            method (str): The method to use for the shortest path algorithm (e.g., "Dijkstra", "Bellman-Ford").

        Example:
            network.MultiSourcePath([1, 2, 3], "Dijkstra")
     )pbdoc")

		// 单源最短路径计算
		.def("SingleSourcePath", &CNetwork::SingleSourcePath,
			R"pbdoc(
        A function that calculates the shortest paths from a single source node to all other nodes.

        Args:
            Start (int): The starting node.
            method (str): The method to use for the shortest path algorithm (e.g., "Dijkstra", "Bellman-Ford").

        Example:
            network.SingleSourcePath(1, "Dijkstra")
     )pbdoc")

		// 添加单条边
		.def("AddEdgeFromTuple", &CNetwork::AddEdgeFromTuple,
			R"pbdoc(
        A function that adds a single edge to the network from a given tuple.

        Args:
            t (tuple): A tuple with exactly 3 elements:
                       - t[0] (int): The ID of the starting node.
                       - t[1] (int): The ID of the ending node.
                       - t[2] (dict): A dictionary containing the edge attributes, 
                                      including a "weight" key for the edge's weight.

        Raises:
            RuntimeError: If the tuple does not have exactly 3 elements or if the dictionary does not contain the "weight" key.

        Example:
            network.AddEdgeFromTuple((1, 2, {"weight": 10.5}))
     )pbdoc")

		// 添加多条边
		.def("AddEdgesFromList", &CNetwork::AddEdgesFromList,
			R"pbdoc(
        A function that adds multiple edges to the network from a list of tuples.

        Args:
            tupleList (list of tuple): A list of tuples where each tuple contains:
                                       - t[0] (int): The ID of the starting node.
                                       - t[1] (int): The ID of the ending node.
                                       - t[2] (dict): A dictionary containing the edge attributes, 
                                                      including a "weight" key for the edge's weight.

        Raises:
            RuntimeError: If any tuple in the list does not have exactly 3 elements or if the dictionary does not contain the "weight" key.

        Example:
            edges = [
                (1, 2, {"weight": 10.5}),
                (2, 3, {"weight": 7.2}),
                (3, 4, {"weight": 15.3})
            ]
            network.AddEdgesFromList(edges)
     )pbdoc");
}