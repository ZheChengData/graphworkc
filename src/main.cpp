#include <iostream>
#include "CGraphBase.h"
#include "GraphAlgorithms.h"

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

		// 多源最短路径
		.def("multi_source_cost", &GraphAlgorithms::multi_source_cost,
			py::arg("start_nodes"),
			py::arg("method") = "Dijkstra",
			py::arg("target") = -1,
			py::arg("cut_off") = numeric_limits<double>::infinity(),
			py::arg("weight_name") = "")


		.def("multi_source_path", &GraphAlgorithms::multi_source_path,
			py::arg("start_nodes"),
			py::arg("method") = "Dijkstra",
			py::arg("target") = -1,
			py::arg("cut_off") = numeric_limits<double>::infinity(),
			py::arg("weight_name") = "")


		.def("multi_source_all", &GraphAlgorithms::multi_source_all,
			py::arg("start_nodes"),
			py::arg("method") = "Dijkstra",
			py::arg("target") = -1,
			py::arg("cut_off") = numeric_limits<double>::infinity(),
			py::arg("weight_name") = "")


		// 单源最短路径
		.def("single_source_cost", &GraphAlgorithms::single_source_cost,
			py::arg("start"),
			py::arg("method") = "Dijkstra",
			py::arg("target") = -1,
			py::arg("cut_off") = numeric_limits<double>::infinity(),
			py::arg("weight_name") = "")


		.def("single_source_path", &GraphAlgorithms::single_source_path,
			py::arg("start"),
			py::arg("method") = "Dijkstra",
			py::arg("target") = -1,
			py::arg("cut_off") = numeric_limits<double>::infinity(),
			py::arg("weight_name") = "")


		.def("single_source_all", &GraphAlgorithms::single_source_all,
			py::arg("start"),
			py::arg("method") = "Dijkstra",
			py::arg("target") = -1,
			py::arg("cut_off") = numeric_limits<double>::infinity(),
			py::arg("weight_name") = "")


		// 多个单源最短路径
		.def("multi_single_source_cost", &GraphAlgorithms::multi_single_source_cost,
			py::arg("start_nodes"),
			py::arg("method") = "Dijkstra",
			py::arg("target") = -1,
			py::arg("cut_off") = numeric_limits<double>::infinity(),
			py::arg("weight_name") = "",
			py::arg("num_thread") = 1)


		.def("multi_single_source_path", &GraphAlgorithms::multi_single_source_path,
			py::arg("start_nodes"),
			py::arg("method") = "Dijkstra",
			py::arg("target") = -1,
			py::arg("cut_off") = numeric_limits<double>::infinity(),
			py::arg("weight_name") = "",
			py::arg("num_thread") = 1)


		.def("multi_single_source_all", &GraphAlgorithms::multi_single_source_all,
			py::arg("start_nodes"),
			py::arg("method") = "Dijkstra",
			py::arg("target") = -1,
			py::arg("cut_off") = numeric_limits<double>::infinity(),
			py::arg("weight_name") = "",
			py::arg("num_thread") = 1)


		// 多个多源最短路径
		.def("multi_multi_source_cost", &GraphAlgorithms::multi_multi_source_cost,
			py::arg("start_nodes"),
			py::arg("method") = "Dijkstra",
			py::arg("target") = -1,
			py::arg("cut_off") = numeric_limits<double>::infinity(),
			py::arg("weight_name") = "",
			py::arg("num_thread") = 1,
			py::return_value_policy::move)


		.def("multi_multi_source_path", &GraphAlgorithms::multi_multi_source_path,
			py::arg("start_nodes"),
			py::arg("method") = "Dijkstra",
			py::arg("target") = -1,
			py::arg("cut_off") = numeric_limits<double>::infinity(),
			py::arg("weight_name") = "",
			py::arg("num_thread") = 1,
			py::return_value_policy::move)


		.def("multi_multi_source_all", &GraphAlgorithms::multi_multi_source_all,
			py::arg("start_nodes"),
			py::arg("method") = "Dijkstra",
			py::arg("target") = -1,
			py::arg("cut_off") = numeric_limits<double>::infinity(),
			py::arg("weight_name") = "",
			py::arg("num_thread") = 1)


		// 花费矩阵
		.def("cost_matrix", &GraphAlgorithms::cost_matrix,
			py::arg("starts"),
			py::arg("ends"),
			py::arg("method") = "Dijkstra",
			py::arg("cut_off") = numeric_limits<double>::infinity(),
			py::arg("weight_name") = "",
			py::arg("num_thread") = 1)


		// 路径字典:
		.def("path_dict", &GraphAlgorithms::path_dict,
			py::arg("starts"),
			py::arg("ends"),
			py::arg("method") = "Dijkstra",
			py::arg("cut_off") = numeric_limits<double>::infinity(),
			py::arg("weight_name") = "",
			py::arg("num_thread") = 1)


		// 路径字典: 一一对应
		.def("path_dict_pairwise", &GraphAlgorithms::path_dict_pairwise,
			py::arg("starts"),
			py::arg("ends"),
			py::arg("method") = "Dijkstra",
			py::arg("weight_name") = "",
			py::arg("num_thread") = 1)


		// K条最短路径
		.def("k_shortest_paths", &GraphAlgorithms::k_shortest_paths,
			py::arg("source"),
			py::arg("target"),
			py::arg("num"),
			py::arg("weight_name") = "")


		// 单个OD对最短花费和路径
		.def("shortest_path_cost", &GraphAlgorithms::shortest_path_cost,
			py::arg("source"),
			py::arg("target"),
			py::arg("weight_name") = "")


		.def("shortest_path_path", &GraphAlgorithms::shortest_path_path,
			py::arg("source"),
			py::arg("target"),
			py::arg("weight_name") = "")


		.def("shortest_path_all", &GraphAlgorithms::shortest_path_all,
			py::arg("source"),
			py::arg("target"),
			py::arg("weight_name") = "")


		.def("process", [](GraphAlgorithms& self, py::object df) { // 注意这里传入self引用
			// 通过成员函数处理数据
			auto net = self.convert_dataframe(df); // 调用成员函数
			auto new_net = self.process_neg_dir(net);
			auto seq_groups = self.group_by_seq(new_net);

			// 获取排序后的唯一seq
			std::vector<int> unique_sorted_values;
			for (const auto& pair : seq_groups) {
				unique_sorted_values.push_back(pair.first);
			}
			std::sort(unique_sorted_values.begin(), unique_sorted_values.end());

			// 处理相邻对
			return self.process_pairs(seq_groups, unique_sorted_values);
		})
	;
}