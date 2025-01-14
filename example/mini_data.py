# 需使用3.11版本

import graphworkc
import pandas as pd
import time

# 初始化C交网类
network = graphworkc.CNetwork()

# 假设python端现有如下路网
transnet = [(1, 2, {'weight':      1}),
            (2, 3, {'weight':     10}),
            (3, 4, {'weight':    100}),
            (2, 1, {'weight':   1000}),
            (3, 2, {'weight':  10000}),
            (4, 3, {'weight': 100000}),
            (1, 20, {'weight': 2}),]

transnet_2 = [(10, 20, {'weight':      1}),
              (20, 30, {'weight':     10}),
              (30, 40, {'weight':    100}),
              (20, 10, {'weight':   1000}),
              (30, 20, {'weight':  10000}),
              (40, 30, {'weight': 100000})]

remove_list = [(10, 20),
               (20, 30),
               (30, 40),
               (20, 10),
               (30, 20),
               (40, 30)]

method = "Dijkstra"
list_node = [1, 2, 3, 4]

# 加一条边
network.AddEdgeFromTuple((4, 5, {'weight': 20}))
# 加多条边
network.AddEdgesFromList(transnet)
network.AddEdgesFromList(transnet_2)
# 移除一条边
network.RemoveEdge(4, 5)
# 移除多条边
network.RemoveEdges(remove_list)
# 单源最短路径
network.SingleSourcePath(1, method)
# 多源最短路径
network.MultiSourcePath(list_node)

# 拿取源节点为1的最短路径计算结果
result = network.m_path_result[1]
result_cost = network.m_path_result[1].dict_cost
result_path = network.m_path_result[1].dict_path

# 输出结果
network.CostMartixToCsv(list_node, list_node, "last_cost.csv")
network.PathToCsv(list_node, list_node, "last_paths.csv")
