# -- coding: utf-8 --
# @Time    : 2025/2/26 22:06
# @Author  : TangKai
# @Team    : ZheChengData


import time
import pandas as pd
import networkx as nx
import graphworkc as gw


def t_single_source_cost(link: pd.DataFrame = None):
    g = gw.CGraph()
    g.add_edges([(f, t, {'l': l}) for f, t, l in zip(link['from_node'],
                                                     link['to_node'], link['length'])])

    ng = nx.DiGraph()
    ng.add_edges_from([(f, t, {'l': l}) for f, t, l in zip(link['from_node'],
                                                           link['to_node'], link['length'])])

    # 时间对比
    a = time.time()
    gw_res = g.single_source_cost(start=355061, weight_name='l')  # -> dict
    b = time.time()
    print(rf'single_source: graphworkc: {b - a}')
    c = time.time()
    nx_res = nx.single_source_dijkstra_path_length(ng, 355061, weight='l')
    d = time.time()
    print(rf'single_source: networkx: {d - c}')

    print(f"时间比(c++/python): {(b-a)/(d-c)}")

    # 结果对比
    _ = 0
    for k in gw_res.keys():
        gap = gw_res[k] - nx_res[k]
        _ += gap
    print(_)


def t_single_source_path(link: pd.DataFrame = None):
    g = gw.CGraph()
    g.add_edges([(f, t, {'l': l}) for f, t, l in zip(link['from_node'],
                                                     link['to_node'], link['length'])])

    ng = nx.DiGraph()
    ng.add_edges_from([(f, t, {'l': l}) for f, t, l in zip(link['from_node'],
                                                           link['to_node'], link['length'])])

    # 时间对比
    a = time.time()
    gw_res = g.single_source_path(start=355061, weight_name='l')  # -> dict
    b = time.time()
    print(rf'single_source: graphworkc: {b - a}')
    c = time.time()
    nx_res = nx.single_source_dijkstra_path(ng, 355061, weight='l')
    d = time.time()
    print(rf'single_source: networkx: {d - c}')

    print(f"时间比(c++/python): {(b-a)/(d-c)}")

    # 结果对比
    total_difference_rate = 0  # 初始化差异率总和
    total_paths = len(gw_res)  # 路径总数，用于计算平均差异率

    for k in gw_res.keys():
        if k in nx_res:  # 确保在两个结果字典中都有该节点
            gw_path_length = len(gw_res[k])  # graphworkc的路径长度
            nx_path_length = len(nx_res[k])  # networkx的路径长度

            # 计算差异
            difference = abs(gw_path_length - nx_path_length)

            # 计算差异率：相对于networkx的路径长度
            if nx_path_length != 0:  # 防止除以0的情况
                difference_rate = (difference / nx_path_length) * 100
                total_difference_rate += difference_rate  # 累加差异率

    # 输出平均差异率
    average_difference_rate = total_difference_rate / total_paths if total_paths > 0 else 0
    print(f"平均差异率: {average_difference_rate}%")


def t_single_source_all(link: pd.DataFrame = None):
    g = gw.CGraph()
    g.add_edges([(f, t, {'l': l}) for f, t, l in zip(link['from_node'],
                                                     link['to_node'], link['length'])])

    ng = nx.DiGraph()
    ng.add_edges_from([(f, t, {'l': l}) for f, t, l in zip(link['from_node'],
                                                           link['to_node'], link['length'])])

    # 时间对比
    a = time.time()
    gw_res = g.single_source_all(start=355061, weight_name='l')  # -> dict
    b = time.time()
    print(rf'single_source: graphworkc: {b - a}')

    c = time.time()
    _, nx_res = nx.single_source_dijkstra(ng, 355061, weight='l')
    d = time.time()
    print(rf'single_source: networkx: {d - c}')

    print(f"时间比(c++/python): {(b-a)/(d-c)}")

    # 结果对比
    total_difference_rate = 0  # 初始化差异率总和
    total_paths = len(gw_res.paths)  # 路径总数，用于计算平均差异率

    for k in gw_res.paths.keys():
        if k in nx_res:  # 确保在两个结果字典中都有该节点
            gw_path_length = len(gw_res.paths[k])  # graphworkc的路径长度
            nx_path_length = len(nx_res[k])  # networkx的路径长度

            # 计算差异
            difference = abs(gw_path_length - nx_path_length)

            # 计算差异率：相对于networkx的路径长度
            if nx_path_length != 0:  # 防止除以0的情况
                difference_rate = (difference / nx_path_length) * 100
                total_difference_rate += difference_rate  # 累加差异率

    # 输出平均差异率
    average_difference_rate = total_difference_rate / total_paths if total_paths > 0 else 0
    print(f"平均差异率: {average_difference_rate}%")


def t_multi_source_cost(link: pd.DataFrame = None):
    g = gw.CGraph()
    g.add_edges([(f, t, {'l': l}) for f, t, l in zip(link['from_node'],
                                                     link['to_node'], link['length'])])

    ng = nx.DiGraph()
    ng.add_edges_from([(f, t, {'l': l}) for f, t, l in zip(link['from_node'],
                                                           link['to_node'], link['length'])])

    # 时间对比
    a = time.time()
    gw_res = g.multi_source_cost(start_nodes=[355061], weight_name='l')  # -> dict
    b = time.time()
    print(rf'multi_source: graphworkc: {b - a}')

    c = time.time()
    nx_res = nx.multi_source_dijkstra_path_length(ng, [355061], weight='l')
    d = time.time()
    print(rf'multi_source: networkx: {d - c}')

    print(f"时间比(c++/python): {(b-a)/(d-c)}")

    # 结果对比
    _ = 0
    for k in gw_res.keys():
        gap = gw_res[k] - nx_res[k]
        _ += gap
    print(_)


def t_multi_source_path(link: pd.DataFrame = None):
    g = gw.CGraph()
    g.add_edges([(f, t, {'l': l}) for f, t, l in zip(link['from_node'],
                                                     link['to_node'], link['length'])])

    ng = nx.DiGraph()
    ng.add_edges_from([(f, t, {'l': l}) for f, t, l in zip(link['from_node'],
                                                           link['to_node'], link['length'])])

    # 时间对比
    a = time.time()
    gw_res = g.multi_source_path(start_nodes=[355061], weight_name='l')  # -> dict
    b = time.time()
    print(rf'multi_source: graphworkc: {b - a}')

    c = time.time()
    nx_res = nx. multi_source_dijkstra_path(ng, [355061], weight='l')
    d = time.time()
    print(rf'multi_source: networkx: {d - c}')

    print(f"时间比(c++/python): {(b-a)/(d-c)}")

    # 结果对比
    total_difference_rate = 0  # 初始化差异率总和
    total_paths = len(gw_res)  # 路径总数，用于计算平均差异率

    for k in gw_res.keys():
        if k in nx_res:  # 确保在两个结果字典中都有该节点
            gw_path_length = len(gw_res[k])  # graphworkc的路径长度
            nx_path_length = len(nx_res[k])  # networkx的路径长度

            # 计算差异
            difference = abs(gw_path_length - nx_path_length)

            # 计算差异率：相对于networkx的路径长度
            if nx_path_length != 0:  # 防止除以0的情况
                difference_rate = (difference / nx_path_length) * 100
                total_difference_rate += difference_rate  # 累加差异率

    # 输出平均差异率
    average_difference_rate = total_difference_rate / total_paths if total_paths > 0 else 0
    print(f"平均差异率: {average_difference_rate}%")


def t_multi_source_all(link: pd.DataFrame = None):
    g = gw.CGraph()
    g.add_edges([(f, t, {'l': l}) for f, t, l in zip(link['from_node'],
                                                     link['to_node'], link['length'])])

    ng = nx.DiGraph()
    ng.add_edges_from([(f, t, {'l': l}) for f, t, l in zip(link['from_node'],
                                                           link['to_node'], link['length'])])

    # 时间对比
    a = time.time()
    gw_res = g.multi_source_all(start_nodes=[355061], weight_name='l')  # -> dict
    b = time.time()
    print(rf'multi_source: graphworkc: {b - a}')

    c = time.time()
    _, nx_res = nx.multi_source_dijkstra(ng, [355061], weight='l')
    d = time.time()
    print(rf'multi_source: networkx: {d - c}')

    print(f"时间比(c++/python): {(b-a)/(d-c)}")

    # 结果对比
    total_difference_rate = 0  # 初始化差异率总和
    total_paths = len(gw_res.paths)  # 路径总数，用于计算平均差异率

    for k in gw_res.paths.keys():
        if k in nx_res:  # 确保在两个结果字典中都有该节点
            gw_path_length = len(gw_res.paths[k])  # graphworkc的路径长度
            nx_path_length = len(nx_res[k])  # networkx的路径长度

            # 计算差异
            difference = abs(gw_path_length - nx_path_length)

            # 计算差异率：相对于networkx的路径长度
            if nx_path_length != 0:  # 防止除以0的情况
                difference_rate = (difference / nx_path_length) * 100
                total_difference_rate += difference_rate  # 累加差异率

    # 输出平均差异率
    average_difference_rate = total_difference_rate / total_paths if total_paths > 0 else 0
    print(f"平均差异率: {average_difference_rate}%")


def t_multi_single_source_cost(link: pd.DataFrame = None, num: int = 50):
    n = link.sample(num)
    n_list = list(n['from_node'].unique())

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
    all_nx_res = dict()
    for n in n_list:
        nx_res = nx.single_source_dijkstra_path_length(ng, n, weight='l',)
        all_nx_res[n] = nx_res
    d = time.time()
    print(rf'multi_single_source: networkx: {d - c}')

    print(f"时间比(c++/python): {(b - a) / (d - c)}")

    _ = 0
    for i, n in enumerate(n_list):
        for k in all_nx_res[n_list[i]].keys():
            gap = gw_res[i][k] - all_nx_res[n_list[i]][k]
            _ += gap
    print(_)


def t_multi_single_source_path(link: pd.DataFrame = None, num: int = 50):
    n = link.sample(num)
    n_list = list(n['from_node'].unique())

    g = gw.CGraph()
    g.add_edges([(f, t, {'l': l}) for f, t, l in zip(link['from_node'],
                                                     link['to_node'], link['length'])])

    ng = nx.DiGraph()
    ng.add_edges_from([(f, t, {'l': l}) for f, t, l in zip(link['from_node'],
                                                           link['to_node'], link['length'])])

    # 时间对比
    a = time.time()
    gw_res = g.multi_single_source_path(start_nodes=n_list, weight_name='l', num_thread=10)  # -> list[dict]
    b = time.time()

    print(rf'multi_single_source: graphworkc: {b - a}')

    c = time.time()
    all_nx_res = dict()
    for n in n_list:
        nx_res = nx.single_source_dijkstra_path(ng, n, weight='l',)
        all_nx_res[n] = nx_res
    d = time.time()
    print(rf'multi_single_source: networkx: {d - c}')

    print(f"时间比(c++/python): {(b - a) / (d - c)}")

    # 结果对比
    total_paths = 0
    mismatched_paths = 0

    for i, n in enumerate(n_list):
        print(f"Comparing results for start node {n}:")

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


def t_multi_single_source_all(link: pd.DataFrame = None, num: int = 50):
    n = link.sample(num)
    n_list = list(n['from_node'].unique())

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
    all_nx_res = dict()
    for n in n_list:
        _, nx_res = nx.single_source_dijkstra(ng, n, weight='l',)
        all_nx_res[n] = nx_res
    d = time.time()
    print(rf'multi_single_source: networkx: {d - c}')

    print(f"时间比(c++/python): {(b - a) / (d - c)}")

    # 结果对比
    total_paths = 0
    mismatched_paths = 0

    for i, n in enumerate(n_list):
        print(f"Comparing results for start node {n}:")
        gw_res1 = gw_res[i].paths
        for k in all_nx_res[n].keys():
            gw_path = gw_res1[k]  # 获取路径，如果没有返回None
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


def t_multi_multi_source_cost(link: pd.DataFrame = None, num: int = 50):
    n = link.sample(num)
    n_list = list(n['from_node'].unique())
    n_list_list = []
    for i in n_list:
        n_list_list.append([i])

    g = gw.CGraph()
    g.add_edges([(f, t, {'l': l}) for f, t, l in zip(link['from_node'],
                                                     link['to_node'], link['length'])])

    ng = nx.DiGraph()
    ng.add_edges_from([(f, t, {'l': l}) for f, t, l in zip(link['from_node'],
                                                           link['to_node'], link['length'])])

    # 时间对比
    a = time.time()
    gw_res = g.multi_multi_source_cost(start_nodes=n_list_list, weight_name='l', num_thread=10)  # -> list[dict]
    b = time.time()

    print(rf'multi_multi_source: graphworkc: {b - a}')

    c = time.time()
    all_nx_res = []
    for i in range(len(n_list_list)):
        nx_res = nx.multi_source_dijkstra_path_length(ng, n_list_list[i], weight='l',)
        all_nx_res.append(nx_res)
    d = time.time()
    print(rf'multi_multi_source: networkx: {d - c}')

    print(f"时间比(c++/python): {(b - a) / (d - c)}")

    _ = 0
    for i, n in enumerate(n_list):
        for k in all_nx_res[i].keys():
            gap = gw_res[i][k] - all_nx_res[i][k]
            _ += gap
    print(_)


def t_multi_multi_source_path(link: pd.DataFrame = None, num: int = 50):
    n = link.sample(num)
    n_list = list(n['from_node'].unique())
    n_list_list = []
    for i in n_list:
        n_list_list.append([i])

    g = gw.CGraph()
    g.add_edges([(f, t, {'l': l}) for f, t, l in zip(link['from_node'],
                                                     link['to_node'], link['length'])])

    ng = nx.DiGraph()
    ng.add_edges_from([(f, t, {'l': l}) for f, t, l in zip(link['from_node'],
                                                           link['to_node'], link['length'])])

    # 时间对比
    a = time.time()
    gw_res = g.multi_multi_source_path(start_nodes=n_list_list, weight_name='l', num_thread=10)  # -> list[dict]
    b = time.time()
    print(rf'multi_multi_source: graphworkc: {b - a}')

    c = time.time()
    all_nx_res = []
    for i in range(len(n_list_list)):
        nx_res = nx.multi_source_dijkstra_path(ng, n_list_list[i], weight='l', )
        all_nx_res.append(nx_res)
    d = time.time()
    print(rf'multi_multi_source: networkx: {d - c}')

    print(f"时间比(c++/python): {(b - a) / (d - c)}")

    # 结果对比
    total_paths = 0
    mismatched_paths = 0

    for i, n in enumerate(n_list):
        print(f"Comparing results for start node {n}:")

        for k in all_nx_res[i].keys():
            gw_path = gw_res[i][k]  # 获取路径，如果没有返回None
            nx_path = all_nx_res[i].get(k, None)  # 获取路径，如果没有返回None

            total_paths += 1  # 总路径数

            if gw_path is None or nx_path is None:
                mismatched_paths += 1  # 这表示路径缺失
                print(f"Node {k} is missing from either gw_res or nx_res.")
                print(f"gw_res for {n}: {gw_res[i]}")
                print(f"nx_res for {n}: {all_nx_res[i]}")
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


def t_multi_multi_source_all(link: pd.DataFrame = None, num: int = 50):
    n = link.sample(num)
    n_list = list(n['from_node'].unique())
    n_list_list = []
    for i in n_list:
        n_list_list.append([i])

    g = gw.CGraph()
    g.add_edges([(f, t, {'l': l}) for f, t, l in zip(link['from_node'],
                                                     link['to_node'], link['length'])])

    ng = nx.DiGraph()
    ng.add_edges_from([(f, t, {'l': l}) for f, t, l in zip(link['from_node'],
                                                           link['to_node'], link['length'])])

    # 时间对比
    a = time.time()
    gw_res1 = g.multi_multi_source_all(start_nodes=n_list_list, weight_name='l', num_thread=10)  # -> list[dict]
    b = time.time()
    print(rf'multi_multi_source: graphworkc: {b - a}')

    c = time.time()
    all_nx_res = []
    for i in range(len(n_list_list)):
        _, nx_res = nx.multi_source_dijkstra(ng, n_list_list[i], weight='l', )
        all_nx_res.append(nx_res)
    d = time.time()
    print(rf'multi_multi_source: networkx: {d - c}')

    print(f"时间比(c++/python): {(b - a) / (d - c)}")

    # 结果对比
    total_paths = 0
    mismatched_paths = 0

    for i, n in enumerate(n_list):
        gw_res = gw_res1[i].paths
        print(f"Comparing results for start node {n}:")

        for k in all_nx_res[i].keys():
            gw_path = gw_res[k]  # 获取路径，如果没有返回None
            nx_path = all_nx_res[i].get(k, None)  # 获取路径，如果没有返回None

            total_paths += 1  # 总路径数

            if gw_path is None or nx_path is None:
                mismatched_paths += 1  # 这表示路径缺失
                print(f"Node {k} is missing from either gw_res or nx_res.")
                print(f"gw_res for {n}: {gw_res}")
                print(f"nx_res for {n}: {all_nx_res[i]}")
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


def t_cost_matrix(link: pd.DataFrame = None, num: int = 50):
    n = link.sample(num)
    n_list = list(n['from_node'].unique())
    print(len(n_list))
    g = gw.GraphAlgorithms()
    g.add_edges([(f, t, {'l': l}) for f, t, l in zip(link['from_node'],
                                                     link['to_node'], link['length'])])

    a = time.time()
    cost_mat = g.cost_matrix_to_numpy(starts=n_list,
                                      ends=n_list, weight_name='l',
                                      num_thread=16)  # -> list[dict]
    b = time.time()
    print(cost_mat)

    print(rf'cost calc: graphworkc: {b - a}')
    # print(cost_mat)
    print(cost_mat.shape)


if __name__ == '__main__':
    net = pd.read_csv("data/link.csv", encoding='gbk')
    neg_net = net[net['dir'] == 0].copy()
    neg_net[['from_node', 'to_node']] = neg_net[['to_node', 'from_node']]
    net = pd.concat([neg_net, net]).reset_index(drop=True)

    # t_single_source_cost(link=net)
    # t_single_source_path(link=net)
    # t_single_source_all(link=net)
    # t_multi_source_cost(link=net)
    # t_multi_source_path(link=net)
    # t_multi_source_all(link=net)
    t_multi_single_source_cost(link=net, num=20)
    t_multi_single_source_path(link=net, num=20)
    t_multi_single_source_all(link=net, num=20)
    # t_multi_multi_source_cost(link=net, num=5)
    # t_multi_multi_source_path(link=net, num=5)
    # t_multi_multi_source_all(link=net, num=5)
    # t_cost_matrix(link=net, num=2000)
