import time
import pandas as pd
import networkx as nx
import geopandas as gpd
import graphworkc as gw
import nx_parallel as nxp

# enabling networkx's config for nx-parallel
nx.config.backends.parallel.active = True
# setting `n_jobs` (by default, `n_jobs=None`)
nx.config.backends.parallel.n_jobs = 10

# 检查并行配置
is_parallel_active = nx.config.backends.parallel.active
num_jobs = nx.config.backends.parallel.n_jobs

print(f"Parallel active: {is_parallel_active}")
print(f"Number of parallel jobs: {num_jobs}")


def t_all_single_source_cost(link: pd.DataFrame = None, num: int = 50):
    n = link
    n_list = list(pd.concat([n['from_node'], n['to_node']]).unique())
    print(len(list(pd.concat([n['from_node'], n['to_node']]).unique())))

    g = gw.CGraph()
    g.add_edges([(f, t, {'l': l}) for f, t, l in zip(link['from_node'],
                                                     link['to_node'], link['length'])])

    ng = nx.DiGraph()
    ng.add_edges_from([(f, t, {'l': l}) for f, t, l in zip(link['from_node'],
                                                           link['to_node'], link['length'])])

    # 时间对比
    a = time.time()
    gw_res = g.multi_single_source_cost(start_nodes=n_list, weight_name='l', num_thread=10)  # -> list[dict]
    b = time.time()

    print(rf'multi_single_source: graphworkc: {b - a}')

    c = time.time()
    all_nx_res = dict(nxp.all_pairs_dijkstra_path_length(ng, weight='l'))
    d = time.time()
    print(rf'multi_single_source: networkx: {d - c}')

    print(f"时间比(c++/python): {(b - a) / (d - c)}")

    _ = 0
    for i, n in enumerate(n_list):
        for k in all_nx_res[n].keys():
            # 检查是否 k 在 gw_res[n] 中存在
            if k in gw_res[i]:
                gap = gw_res[i][k] - all_nx_res[n][k]
                _ += gap
    print(_)


def t_all_single_source_path(link: pd.DataFrame = None, num: int = 50):
    # n = link.sample(num)
    n = link
    n_list = list(pd.concat([n['from_node'], n['to_node']]).unique())
    print(len(list(pd.concat([n['from_node'], n['to_node']]).unique())))

    g = gw.CGraph()
    g.add_edges([(f, t, {'l': l}) for f, t, l in zip(link['from_node'],
                                                     link['to_node'], link['length'])])
    g.get_graph_info()

    ng = nx.DiGraph()
    ng.add_edges_from([(f, t, {'l': l}) for f, t, l in zip(link['from_node'],
                                                           link['to_node'], link['length'])])

    # 时间对比
    a = time.time()
    gw_res = g.multi_single_source_path(start_nodes=n_list, weight_name='l', num_thread=10)  # -> list[dict]
    b = time.time()

    print(rf'multi_single_source: graphworkc: {b - a}')

    c = time.time()
    all_nx_res = dict(nxp.all_pairs_dijkstra_path(ng, weight='l'))
    d = time.time()
    print(rf'multi_single_source: networkx: {d - c}')

    print(f"时间比(c++/python): {(b - a) / (d - c)}")

    # 结果对比
    total_paths = 0
    mismatched_paths = 0

    for i, n in enumerate(n_list):
        # print(f"Comparing results for start node {n}:")

        for k in all_nx_res[n].keys():
            gw_path = gw_res[i][k]  # 获取路径，如果没有返回None
            nx_path = all_nx_res[n].get(k, None)  # 获取路径，如果没有返回None

            total_paths += 1  # 总路径数

            if gw_path is None or nx_path is None:
                mismatched_paths += 1  # 这表示路径缺失
                print(f"Node {k} is missing from either gw_res or nx_res.")
                print(f"gw_res for {n}: {gw_res[i]}")
                print(f"nx_res for {n}: {all_nx_res[n]}")
                print()
            else:
                # 比较路径，如果路径不一致，则计入不匹配路径
                if set([n] + gw_path) != set(nx_path):
                    mismatched_paths += 1  # 不匹配的路径
                    print(f"Mismatch for node {k} from start node {n}:")
                    print(f"gw_res path: {gw_path}")
                    print(f"nx_res path: {nx_path}")
                    print()

    # 计算差异率
    if total_paths > 0:
        difference_rate = (mismatched_paths / total_paths) * 100
        print(f"总差异率: {difference_rate:.2f}%")
    else:
        print("没有路径进行比较，无法计算差异率。")


def t_all_single_source_all(link: pd.DataFrame = None, num: int = 50):
    n = link
    n_list = list(pd.concat([n['from_node'], n['to_node']]).unique())
    print(len(list(pd.concat([n['from_node'], n['to_node']]).unique())))

    g = gw.CGraph()
    g.add_edges([(f, t, {'l': l}) for f, t, l in zip(link['from_node'],
                                                     link['to_node'], link['length'])])

    ng = nx.DiGraph()
    ng.add_edges_from([(f, t, {'l': l}) for f, t, l in zip(link['from_node'],
                                                           link['to_node'], link['length'])])

    # 时间对比
    a = time.time()
    gw_res = g.multi_single_source_all(start_nodes=n_list, weight_name='l', num_thread=10)  # -> list[dict]
    b = time.time()

    print(rf'multi_single_source: graphworkc: {b - a}')

    c = time.time()
    all_nx_res = dict(nxp.all_pairs_dijkstra(ng, weight='l'))
    d = time.time()
    print(rf'multi_single_source: networkx: {d - c}')

    print(f"时间比(c++/python): {(b - a) / (d - c)}")

    # 结果对比
    total_paths = 0
    mismatched_paths = 0

    for i, n in enumerate(n_list):
        # print(f"Comparing results for start node {n}:")
        gw_res1 = gw_res[i].paths
        for k in all_nx_res[n][1].keys():
            gw_path = gw_res1[k]  # 获取路径，如果没有返回None
            nx_path = all_nx_res[n][1].get(k, None)  # 获取路径，如果没有返回None

            total_paths += 1  # 总路径数

            if gw_path is None or nx_path is None:
                mismatched_paths += 1  # 这表示路径缺失
                print(f"Node {k} is missing from either gw_res or nx_res.")
                print(f"gw_res for {n}: {gw_res[i]}")
                print(f"nx_res for {n}: {all_nx_res[n]}")
                print()
            else:
                # 比较路径，如果路径不一致，则计入不匹配路径
                if set([n] + gw_path) != set(nx_path):
                    mismatched_paths += 1  # 不匹配的路径
                    print(f"Mismatch for node {k} from start node {n}:")
                    print(f"gw_res path: {gw_path}")
                    print(f"nx_res path: {nx_path}")
                    print()

    # 计算差异率
    if total_paths > 0:
        difference_rate = (mismatched_paths / total_paths) * 100
        print(f"总差异率: {difference_rate:.2f}%")
    else:
        print("没有路径进行比较，无法计算差异率。")


if __name__ == '__main__':
    net = gpd.read_file("data/sub_link/sub_link.shp", encoding='gbk')
    neg_net = net[net['dir'] == 0].copy()
    neg_net[['from_node', 'to_node']] = neg_net[['to_node', 'from_node']]
    net = pd.concat([neg_net, net]).reset_index(drop=True)

    # t_all_single_source_cost(link=net)
    # t_all_single_source_path(link=net)
    t_all_single_source_all(link=net)


# GC = graphwork.CGraph()
# list_o = []
# for i in range(2000):
#     list_o.append(i)
#     GC.add_edge(i, i+1, {"weight": 1})
# GC.add_edge(2000, 0, {"weight": 1})
#
# G = nx.cycle_graph(2000)
# print(nx.config)
#
#
# # 统计 networkx 计算耗时
# start_time_nx_1 = time.time()
# result_p = dict(nxp.all_pairs_dijkstra_path(G))
# end_time_nx_1 = time.time()
# print(f"2000个节点的循环图 networkx-parallel 总耗时: {end_time_nx_1 - start_time_nx_1:.2f} 秒")
#
# # 统计 networkx 计算耗时
# start_time_gc_1 = time.time()
# result_c = GC.multi_single_source_path(list_o, num_thread=16)
# end_time_gc_1 = time.time()
# print(f"2000个节点的循环图 graphworkc 总耗时: {end_time_gc_1 - start_time_gc_1:.2f} 秒")