#include <iostream>
#include "CNetwork.h"
#include "SelfFile.h"
using namespace std;
int main() {
	CNode Node;
	CNetwork Network;
	SelfFile File;
	//Network.ReadNodeCSV(File.node_csv);
	//Network.ReadLinkCSV(File.link_csv);
	//Network.ReadODpairsCSV(File.od_csv);
	//Network.DisplayInfo();
	//tuple<double, vector<int>> a = Network.single_source_dijkstra(1101,8);
	//tuple<unordered_map<int, double>, unordered_map<int, vector<int>>> b = Network.single_source_dijkstra(1101);
	Network.AddEdgeFromC(1, 2, 3.1);
	Network.AddEdgeFromC(1, 3, 2.21);
	Network.AddEdgeFromC(2, 3, 3.5);
	Network.AddEdgeFromC(2, 4, 2.5);
	Network.AddEdgeFromC(3, 4, 1.2);
	tuple<unordered_map<int, double>, unordered_map<int, vector<int>>> b = Network.single_source_dijkstra(1);
}
//pybind11 模块
PYBIND11_MODULE(graphworkC, m) {
	m.doc() = "Example module using pybind11";

	py::class_<CNetwork>(m, "CNetwork")
		.def(py::init<>())  // 添加默认构造函数
		.def("AddEdgeFrom", &CNetwork::AddEdgeFrom, "A function")
		.def("ReadNodeCSV", &CNetwork::ReadNodeCSV, "A function")
		.def("ReadLinkCSV", &CNetwork::ReadLinkCSV, "A function")
		.def("ReadODpairsCSV", &CNetwork::ReadODpairsCSV, "A function")
		.def("DisplayInfo", &CNetwork::DisplayInfo, "A function")
		.def("single_source_dijkstra", py::overload_cast<int, int>(&CNetwork::single_source_dijkstra),
			R"pbdoc(
           A function that calculates the shortest path from `Start` to `End`.

           Args:
               Start (int): The starting node.
               End (int): The target node.

           Returns:
               tuple: A pair containing the distance and the path as a vector of integers.
           )pbdoc")
		.def("single_source_dijkstra", py::overload_cast<int>(&CNetwork::single_source_dijkstra),
			R"pbdoc(
           A function that calculates the shortest paths from `Start` to all other nodes.

           Args:
               Start (int): The starting node.

           Returns:
               tuple: A pair containing a map of distances and a map of paths as vectors of integers.
           )pbdoc")
		.def_readwrite("LinkFlow", &CNetwork::LinkFlow);
}