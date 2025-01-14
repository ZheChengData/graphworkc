import os
import time
import pandas as pd
import graphworkc

if __name__ == "__main__":
    od_df = pd.read_csv("od.csv")
    link_df = pd.read_csv("link.csv")
    # C++ 端数据读入
    transnet = graphworkc.CNetwork()
    # link列表
    if 1:
        list_o = []
        # 4. 添加边及其属性到图
        for _, row in link_df.iterrows():
            from_node = row["FROM_NODE"]
            to_node = row["TO_NODE"]
            ab_fft = row["AB_FFT"]  # 最短时间（A到B方向）
            ba_fft = row["BA_FFT"]  # 最短时间（B到A方向）
            direction = row["DIR"]  # 方向性，假设1表示单向，0表示双向
            # 根据方向性添加边
            if direction == 1:  # 单向 (A -> B)
                list_o.append((from_node, to_node, {'weight': ab_fft}))
            elif direction == 0:  # 双向 (A <-> B)
                list_o.append((from_node, to_node, {'weight': ab_fft}))
                list_o.append((to_node, from_node, {'weight': ba_fft}))
            elif direction == -1:
                list_o.append((to_node, from_node, {'weight': ba_fft}))

    transnet.AddEdgesFromList(list_o)
    method = "Dijkstra"
    list_node = []
    # 时间计算
    start_time = time.time()
    for o_node_id in od_df['o_node_id'].unique():
        list_node.append(o_node_id)
    transnet.MultiSourcePath(list_node, method)
    end_time = time.time()
    print("using time: ", 1000*(end_time - start_time), " ms")

    transnet.CostMartixToCsv(list_node, list_node, "last_cost.csv")
    transnet.PathToCsv(list_node, list_node, "last_paths.csv")
    print("end")
