#include <iostream>
#include "CGraphBase.h"
#include "GraphAlgorithms.h"
#include "GraphTrackit.h" 
#include "GraphAlgorithmInterface.h" 

using namespace std;
namespace py = pybind11;

// 声明容器为不透明类型，禁止默认拷贝行为
PYBIND11_MAKE_OPAQUE(unordered_map<int, vector<int>>);
//PYBIND11_MAKE_OPAQUE(vector<vector<int>>);

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

	py::bind_vector<vector<vector<int>>>(
		m, "ListListInt",
		py::module_local(false)
		);

	py::bind_map<unordered_map<int, double>>(
		m, "MapIntToDouble",
		py::module_local(false)
		)
		.def("__repr__", [](const unordered_map<int, double>& umap) {
		std::string repr = "{";
		for (const std::pair<const int, double>& p : umap) {
			int key = p.first;
			double value = p.second;
			repr += to_string(key) + ": " + to_string(value) + ", ";
		}
		if (!umap.empty()) repr.pop_back();  // 去掉最后的逗号
		repr += "}";
		return repr;
	});

	py::bind_map<unordered_map<int, vector<int>>>(m, "MapIntToListInt", py::module_local(false))
		.def("__repr__", [](const unordered_map<int, vector<int>>& umap) {
		string repr = "{";
		for (const pair<const int, vector<int>>& p : umap) {
			int key = p.first;
			auto value = p.second;
			repr += to_string(key) + ": [";
			for (size_t i = 0; i < value.size(); ++i) {
				repr += to_string(value[i]);
				if (i != value.size() - 1) repr += ", ";
			}
			repr += "], ";
		}
		if (!umap.empty()) repr.pop_back();  
		repr += "}";
		return repr;
	});


	py::class_<dis_and_path>(m, "dis_and_path")
		.def(py::init<>())
		.def_readwrite("cost", &dis_and_path::cost)
		.def_readwrite("paths", &dis_and_path::paths)
		.def("__repr__", [](const dis_and_path &a) {
		return "<dis_and_path cost=" + to_string(a.cost.size()) +
			" paths=" + to_string(a.paths.size()) + ">";
	});


	py::class_<dis_and_path_1>(m, "dis_and_path_1")
		.def(py::init<>())
		.def_readwrite("cost", &dis_and_path_1::cost)
		.def_readwrite("paths", &dis_and_path_1::paths)
		.def("__repr__", [](const dis_and_path_1 &a) {
		return "<dis_and_path cost=" + to_string(a.cost.size()) +
			" paths=" + to_string(a.paths.size()) + ">";
	});

	py::class_<CGraph>(m, "CGraph")
		.def(py::init<>())

		// 获取图信息
		.def("get_graph_info", &CGraph::get_graph_info)


		// 获取节点信息
		.def("get_node_info", &CGraph::get_node_info,
			py::arg("id"))


		// 获取边信息
		.def("get_link_info", &CGraph::get_link_info,
			py::arg("start_node"),
			py::arg("end_node"))


		// 加边
		.def("add_edge", &CGraph::add_edge,
			py::arg("start_node"), 
			py::arg("end_node"),
			py::arg("attribute_dict") = py::dict())


		.def("add_edges", &CGraph::add_edges,
			py::arg("edges"))


		// 删边
		.def("remove_edge", &CGraph::remove_edge,
			py::arg("start"),
			py::arg("end"))

		.def("remove_edges", &CGraph::remove_edges,
			py::arg("edges"))


		// 设置形心点
		.def("set_centroid", py::overload_cast<int>(&CGraph::set_centroid),
			py::arg("node"))
		.def("set_centroids", py::overload_cast<const vector<int>&>(&CGraph::set_centroid),
			py::arg("nodes"))
		;

	py::class_<GraphAlgorithms, CGraph>(m, "GraphAlgorithms")
		.def(py::init<>())

		;

	py::class_<GraphAlgorithmInterface, GraphAlgorithms>(m, "GraphAlgorithmInterface")
		.def(py::init<>())

		// 多源最短路径
		.def("multi_source_cost", &GraphAlgorithmInterface::multi_source_cost,
			py::arg("start_nodes"),
			py::arg("method") = "Dijkstra",
			py::arg("target") = -1,
			py::arg("cut_off") = numeric_limits<double>::infinity(),
			py::arg("weight_name") = "")


		.def("multi_source_path", &GraphAlgorithmInterface::multi_source_path,
			py::arg("start_nodes"),
			py::arg("method") = "Dijkstra",
			py::arg("target") = -1,
			py::arg("cut_off") = numeric_limits<double>::infinity(),
			py::arg("weight_name") = "")


		.def("multi_source_all", &GraphAlgorithmInterface::multi_source_all,
			py::arg("start_nodes"),
			py::arg("method") = "Dijkstra",
			py::arg("target") = -1,
			py::arg("cut_off") = numeric_limits<double>::infinity(),
			py::arg("weight_name") = "")


		// 单源最短路径
		.def("single_source_cost", &GraphAlgorithmInterface::single_source_cost,
			py::arg("start"),
			py::arg("method") = "Dijkstra",
			py::arg("target") = -1,
			py::arg("cut_off") = numeric_limits<double>::infinity(),
			py::arg("weight_name") = "")


		.def("single_source_path", &GraphAlgorithmInterface::single_source_path,
			py::arg("start"),
			py::arg("method") = "Dijkstra",
			py::arg("target") = -1,
			py::arg("cut_off") = numeric_limits<double>::infinity(),
			py::arg("weight_name") = "")


		.def("single_source_all", &GraphAlgorithmInterface::single_source_all,
			py::arg("start"),
			py::arg("method") = "Dijkstra",
			py::arg("target") = -1,
			py::arg("cut_off") = numeric_limits<double>::infinity(),
			py::arg("weight_name") = "")


		// 多个单源最短路径
		.def("multi_single_source_cost", &GraphAlgorithmInterface::multi_single_source_cost,
			py::arg("start_nodes"),
			py::arg("method") = "Dijkstra",
			py::arg("target") = -1,
			py::arg("cut_off") = numeric_limits<double>::infinity(),
			py::arg("weight_name") = "",
			py::arg("num_thread") = 1)


		.def("multi_single_source_path", &GraphAlgorithmInterface::multi_single_source_path,
			py::arg("start_nodes"),
			py::arg("method") = "Dijkstra",
			py::arg("target") = -1,
			py::arg("cut_off") = numeric_limits<double>::infinity(),
			py::arg("weight_name") = "",
			py::arg("num_thread") = 1)


		.def("multi_single_source_all", &GraphAlgorithmInterface::multi_single_source_all,
			py::arg("start_nodes"),
			py::arg("method") = "Dijkstra",
			py::arg("target") = -1,
			py::arg("cut_off") = numeric_limits<double>::infinity(),
			py::arg("weight_name") = "",
			py::arg("num_thread") = 1)


		// 多个多源最短路径
		.def("multi_multi_source_cost", &GraphAlgorithmInterface::multi_multi_source_cost,
			py::arg("start_nodes"),
			py::arg("method") = "Dijkstra",
			py::arg("target") = -1,
			py::arg("cut_off") = numeric_limits<double>::infinity(),
			py::arg("weight_name") = "",
			py::arg("num_thread") = 1,
			py::return_value_policy::move)


		.def("multi_multi_source_path", &GraphAlgorithmInterface::multi_multi_source_path,
			py::arg("start_nodes"),
			py::arg("method") = "Dijkstra",
			py::arg("target") = -1,
			py::arg("cut_off") = numeric_limits<double>::infinity(),
			py::arg("weight_name") = "",
			py::arg("num_thread") = 1,
			py::return_value_policy::move)


		.def("multi_multi_source_all", &GraphAlgorithmInterface::multi_multi_source_all,
			py::arg("start_nodes"),
			py::arg("method") = "Dijkstra",
			py::arg("target") = -1,
			py::arg("cut_off") = numeric_limits<double>::infinity(),
			py::arg("weight_name") = "",
			py::arg("num_thread") = 1)


		// 花费矩阵
		.def("cost_matrix", &GraphAlgorithmInterface::cost_matrix,
			py::arg("starts"),
			py::arg("ends"),
			py::arg("method") = "Dijkstra",
			py::arg("cut_off") = numeric_limits<double>::infinity(),
			py::arg("weight_name") = "",
			py::arg("num_thread") = 1)


		// 路径字典:
		.def("path_dict", &GraphAlgorithmInterface::path_dict,
			py::arg("starts"),
			py::arg("ends"),
			py::arg("method") = "Dijkstra",
			py::arg("cut_off") = numeric_limits<double>::infinity(),
			py::arg("weight_name") = "",
			py::arg("num_thread") = 1)


		// 路径字典: 一一对应
		.def("path_dict_pairwise", &GraphAlgorithmInterface::path_dict_pairwise,
			py::arg("starts"),
			py::arg("ends"),
			py::arg("method") = "Dijkstra",
			py::arg("weight_name") = "",
			py::arg("num_thread") = 1)


		// K条最短路径
		.def("k_shortest_paths", &GraphAlgorithmInterface::k_shortest_paths,
			py::arg("source"),
			py::arg("target"),
			py::arg("num"),
			py::arg("weight_name") = "")


		// 单个OD对最短花费和路径
		.def("shortest_path_cost", &GraphAlgorithmInterface::shortest_path_cost,
			py::arg("source"),
			py::arg("target"),
			py::arg("weight_name") = "")


		.def("shortest_path_path", &GraphAlgorithmInterface::shortest_path_path,
			py::arg("source"),
			py::arg("target"),
			py::arg("weight_name") = "")


		.def("shortest_path_all", &GraphAlgorithmInterface::shortest_path_all,
			py::arg("source"),
			py::arg("target"),
			py::arg("weight_name") = "")
		;

	py::class_<GraphTrackit, GraphAlgorithmInterface>(m, "GraphTrackit")
		.def(py::init<>())


		.def("calc_global_cache", &GraphTrackit::calc_global_cache,
			py::arg("o_list"),
			py::arg("cut_off") = numeric_limits<double>::infinity(),
			py::arg("thread_num") = 1,
			py::arg("weight_name") = "")


		.def("del_temp_cache", &GraphTrackit::del_temp_cache)


		.def("has_path", &GraphTrackit::has_path,
			py::arg("o"),
			py::arg("d"),
			py::arg("use_cache") = true,
			py::arg("cut_off") = numeric_limits<double>::infinity(),
			py::arg("weight_name") = "")


		.def("gotrackit_calc", &GraphTrackit::gotrackit_calc,
			py::arg("seq_k_candidate_info"),
			py::arg("gps_adj_dis_map"),
			py::arg("use_global_cache"),
			py::arg("not_conn_cost"),
			py::arg("num_thread") = 1,
			py::arg("cut_off") = numeric_limits<double>::infinity(),
			py::arg("weight_name") = "")
		;
	
}