#include <iostream>
#include "CGraphBase.h"
#include "GraphAlgorithms.h"

using namespace std;
namespace py = pybind11;

// 声明容器为不透明类型，禁止默认拷贝行为
PYBIND11_MAKE_OPAQUE(std::unordered_map<int, vector<int>>);

int main() {
	return 0;
}


PYBIND11_MODULE(graphwork, m) {
	m.doc() = "module using pybind11";
	py::bind_vector<std::vector<int>>(m, "ListInt", py::module_local(false))
		.def("__repr__", [](const std::vector<int>& vec) {
		std::string repr = "[";
		for (size_t i = 0; i < vec.size(); ++i) {
			repr += std::to_string(vec[i]);
			if (i != vec.size() - 1) repr += ", ";
		}
		repr += "]";
		return repr;
	});


	// 在pybind11模块绑定代码中
	pybind11::class_<Graph>(m, "Graph")
		.def("get_nodes", [](Graph& g) {
		std::vector<int> nodes;
		for (const auto& pair : g) nodes.push_back(pair.first);
		return nodes;
	}, "获取所有起点节点")
		.def("get_edges", [](Graph& g, int src) {
		if (g.find(src) == g.end()) return pybind11::dict();
		pybind11::dict edges;
		for (const auto& dst_pair : g[src]) {
			edges[pybind11::cast(dst_pair.first)] = dst_pair.second;
		}
		return edges;
	}, "获取指定起点的所有边");

	py::bind_vector<std::vector<std::vector<int>>>(
		m, "ListListInt",
		py::module_local(false)
		);

	py::bind_map<std::unordered_map<int, double>>(
		m, "MapIntToDouble",
		py::module_local(false)
		)
		.def("__repr__", [](const std::unordered_map<int, double>& umap) {
		std::string repr = "{";
		for (const std::pair<const int, double>& p : umap) {
			int key = p.first;
			double value = p.second;
			repr += std::to_string(key) + ": " + std::to_string(value) + ", ";
		}
		if (!umap.empty()) repr.pop_back();  // 去掉最后的逗号
		repr += "}";
		return repr;
	});

	py::bind_map<std::unordered_map<int, std::vector<int>>>(m, "MapIntToListInt", py::module_local(false))
		.def("__repr__", [](const std::unordered_map<int, std::vector<int>>& umap) {
		std::string repr = "{";
		for (const std::pair<const int, std::vector<int>>& p : umap) {
			int key = p.first;
			auto value = p.second;
			repr += std::to_string(key) + ": [";
			for (size_t i = 0; i < value.size(); ++i) {
				repr += std::to_string(value[i]);
				if (i != value.size() - 1) repr += ", ";
			}
			repr += "], ";
		}
		if (!umap.empty()) repr.pop_back();  // 去掉最后的逗号
		repr += "}";
		return repr;
	})
		//.def("__getitem__", [](std::unordered_map<int, std::vector<int>>& umap, int key) {
		//return umap.at(key);  // 使用 at() 方法来获取元素
	//})
	;


	py::class_<dis_and_path>(m, "dis_and_path")
		.def(py::init<>())
		.def_readwrite("cost", &dis_and_path::distances)
		.def_readwrite("paths", &dis_and_path::paths)
		.def("__repr__", [](const dis_and_path &a) {
		return "<dis_and_path cost=" + std::to_string(a.distances.size()) +
			" paths=" + std::to_string(a.paths.size()) + ">";
	});

	py::class_<CGraph>(m, "CGraph")
		.def(py::init<>())

		.def("get_graph_info", &CGraph::get_graph_info)

		.def("get_node_info", &CGraph::get_node_info,
			py::arg("id"))

		.def("get_link_info", &CGraph::get_link_info,
			py::arg("start"),
			py::arg("end"))

		// 加点
		.def("add_node", &CGraph::add_node,
			py::arg("id"),
			py::arg("attribute_dict") = py::dict(),
			py::arg("is_planet") = false)

		.def("add_nodes", &CGraph::add_nodes,
			py::arg("nodes"))

		// 删点
		.def("remove_node", &CGraph::remove_node,
			py::arg("id"))

		.def("remove_nodes", &CGraph::remove_nodes,
			py::arg("nodes"))

		// 加边
		.def("add_edge", &CGraph::add_edge,
			py::arg("start_node"), 
			py::arg("end_node"),
			py::arg("attribute_dict") = py::dict(),
			py::arg("planet") = 0)

		.def("add_edges", &CGraph::add_edges,
			py::arg("edges"))

		// 删边
		.def("remove_edge", &CGraph::remove_edge,
			py::arg("start"),
			py::arg("end"))

		.def("remove_edges", &CGraph::remove_edges,
			py::arg("edges"))
		;

	py::class_<GraphAlgorithms>(m, "GraphAlgorithms")
		.def(py::init<>())
		.def("get_graph_info", &CGraph::get_graph_info)

		.def("get_node_info", &CGraph::get_node_info,
			py::arg("id"))

		.def("get_link_info", &CGraph::get_link_info,
			py::arg("start_node"),
			py::arg("end_node"))

		// 加点
		.def("add_node", &CGraph::add_node,
			py::arg("id"),
			py::arg("attribute_dict") = py::dict(),
			py::arg("is_planet") = false)

		.def("add_nodes", &CGraph::add_nodes,
			py::arg("nodes"))

		// 删点
		.def("remove_node", &CGraph::remove_node,
			py::arg("id"))

		.def("remove_nodes", &CGraph::remove_nodes,
			py::arg("nodes"))

		// 加边
		.def("add_edge", &CGraph::add_edge,
			py::arg("start_node"),
			py::arg("end_node"),
			py::arg("attribute_dict") = py::dict(),
			py::arg("planet") = 0)

		.def("add_edges", &CGraph::add_edges,
			py::arg("edges"))

		// 删边
		.def("remove_edge", &CGraph::remove_edge,
			py::arg("start"),
			py::arg("end"))

		.def("remove_edges", &CGraph::remove_edges,
			py::arg("edges"))

		// 多源最短路径
		.def("multi_source_cost", &GraphAlgorithms::multi_source_cost,
			py::arg("list_o"),
			py::arg("method") = "Dijkstra",
			py::arg("target") = -1,
			py::arg("cut_off") = numeric_limits<double>::infinity(),
			py::arg("weight_name") = "no_input")

		.def("multi_source_path", &GraphAlgorithms::multi_source_path,
			py::arg("list_o"),
			py::arg("method") = "Dijkstra",
			py::arg("target") = -1,
			py::arg("cut_off") = numeric_limits<double>::infinity(),
			py::arg("weight_name") = "no_input")

		.def("multi_source_all", &GraphAlgorithms::multi_source_all,
			py::arg("list_o"),
			py::arg("method") = "Dijkstra",
			py::arg("target") = -1,
			py::arg("cut_off") = numeric_limits<double>::infinity(),
			py::arg("weight_name") = "no_input")

		// 单源最短路径
		.def("single_source_cost", &GraphAlgorithms::single_source_cost,
			py::arg("start"),
			py::arg("method") = "Dijkstra",
			py::arg("target") = -1,
			py::arg("cut_off") = numeric_limits<double>::infinity(),
			py::arg("weight_name") = "no_input")

		.def("single_source_path", &GraphAlgorithms::single_source_path,
			py::arg("start"),
			py::arg("method") = "Dijkstra",
			py::arg("target") = -1,
			py::arg("cut_off") = numeric_limits<double>::infinity(),
			py::arg("weight_name") = "no_input")

		.def("single_source_all", &GraphAlgorithms::single_source_all,
			py::arg("start"),
			py::arg("method") = "Dijkstra",
			py::arg("target") = -1,
			py::arg("cut_off") = numeric_limits<double>::infinity(),
			py::arg("weight_name") = "no_input")

		// 多个单源最短路径
		.def("multi_single_source_cost", &GraphAlgorithms::multi_single_source_cost,
			py::arg("list_o"),
			py::arg("method") = "Dijkstra",
			py::arg("target") = -1,
			py::arg("cut_off") = numeric_limits<double>::infinity(),
			py::arg("weight_name") = "no_input",
			py::arg("num_thread") = 1)

		.def("multi_single_source_path", &GraphAlgorithms::multi_single_source_path,
			py::arg("list_o"),
			py::arg("method") = "Dijkstra",
			py::arg("target") = -1,
			py::arg("cut_off") = numeric_limits<double>::infinity(),
			py::arg("weight_name") = "no_input",
			py::arg("num_thread") = 1)

		.def("multi_single_source_all", &GraphAlgorithms::multi_single_source_all,
			py::arg("list_o"),
			py::arg("method") = "Dijkstra",
			py::arg("target") = -1,
			py::arg("cut_off") = numeric_limits<double>::infinity(),
			py::arg("weight_name") = "no_input",
			py::arg("num_thread") = 1)

		// 多个多源最短路径
		.def("multi_multi_source_cost", &GraphAlgorithms::multi_multi_source_cost,
			py::arg("list_o"),
			py::arg("method") = "Dijkstra",
			py::arg("target") = -1,
			py::arg("cut_off") = numeric_limits<double>::infinity(),
			py::arg("weight_name") = "no_input",
			py::arg("num_thread") = 1,
			py::return_value_policy::move)

		.def("multi_multi_source_path", &GraphAlgorithms::multi_multi_source_path,
			py::arg("list_o"),
			py::arg("method") = "Dijkstra",
			py::arg("target") = -1,
			py::arg("cut_off") = numeric_limits<double>::infinity(),
			py::arg("weight_name") = "no_input",
			py::arg("num_thread") = 1,
			py::return_value_policy::move)

		.def("multi_multi_source_all", &GraphAlgorithms::multi_multi_source_all,
			py::arg("list_o"),
			py::arg("method") = "Dijkstra",
			py::arg("target") = -1,
			py::arg("cut_off") = numeric_limits<double>::infinity(),
			py::arg("weight_name") = "no_input",
			py::arg("num_thread") = 1)

		// 
		.def("cost_matrix_to_numpy", &GraphAlgorithms::cost_matrix_to_numpy,
			py::arg("starts"),
			py::arg("ends"),
			py::arg("method") = "Dijkstra",
			py::arg("cut_off") = numeric_limits<double>::infinity(),
			py::arg("weight_name") = "no_input",
			py::arg("num_thread") = 1)

		.def("path_list_to_numpy", &GraphAlgorithms::path_list_to_numpy,
			py::arg("starts"),
			py::arg("ends"),
			py::arg("method") = "Dijkstra",
			py::arg("cut_off") = numeric_limits<double>::infinity(),
			py::arg("weight_name") = "no_input",
			py::arg("num_thread") = 1)

		.def("shortest_simple_paths", &GraphAlgorithms::shortest_simple_paths,
			py::arg("start"),
			py::arg("end"),
			py::arg("weight_name") = "no_input")

	;
}