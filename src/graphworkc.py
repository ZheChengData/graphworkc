import graphwork
from typing import Optional, Type
import numpy as np


class CGraph:
    def __init__(self):
        # 创建 C++ 图算法对象
        self.graph = graphwork.GraphAlgorithms()

    def get_graph_info(self) -> dict:
        """**类方法 - get_graph_info:** <br>
        - 获取图信息

        Returns:
              dict: 返回当前图的相关属性
        """
        result = self.graph.get_graph_info()
        return result

    def get_node_info(self,
                      id: int) -> dict:
        """**类方法 - get_node_info:** <br>
        - 获取节点信息

        Args:
            id (int): 节点的标识符。必须是一个整数，表示图中节点的唯一标识符。

        Raises:
            ValueError: 如果“id”不是整数。

        Returns:
              dict: 返回当前节点的相关属性
        """
        # 确保 id 是有效的节点（假设它们是整数）
        if not isinstance(id, int):
            raise ValueError(f"Invalid value for 'start': {start}. It must be an integer.")

        result = self.graph.get_node_info(id)
        return result

    def get_link_info(self,
                      start: int,
                      end: int) -> dict:
        """**类方法 - get_link_info:** <br>
        - 获取边信息

        Args:
            start (int): 边的起始节点。必须是整数。
            end (int): 边的结束节点。必须是整数。

        Raises:
            ValueError: 如果“start”或“end”不是整数。

        Returns:
              dict: 返回当前边的相关属性
        """
        # 确保 start 和 end 是有效的节点（假设它们是整数）
        if not isinstance(start, int):
            raise ValueError(f"Invalid value for 'start': {start}. It must be an integer.")

        if not isinstance(end, int):
            raise ValueError(f"Invalid value for 'end': {end}. It must be an integer.")

        result = self.graph.get_link_info(start, end)
        return result

    def set_centroid(self,
                     node: int) -> None:
        """**类方法 - set_centroid:** <br>
        - 将一个节点修改为形心点

        Args:
            node (int): 要修改的节点。必须是整数。

        Raises:
            ValueError: 如果“node”不是整数。
        """
        # 确保 node 是有效的节点（假设它们是整数）
        if not isinstance(node, int):
            raise ValueError(f"Invalid value for 'start': {node}. It must be an integer.")

        self.graph.set_centroid(node)

    def set_centroids(self,
                     nodes: list[int]) -> None:
        """**类方法 - set_centroids:** <br>
        - 将多个节点修改为形心点

        Args:
            nodes (list[int]): 要修改的节点。必须是整数。

        Raises:
            ValueError: 如果“nodes”不是列表。
            ValueError: 如果“node”不是整数。
        """
        # 确保 nodes 是一个列表
        if not isinstance(nodes, list):
            raise ValueError(f"Expected 'nodes' to be a list, but got {type(nodes)}.")
        # 确保 node 是有效的节点（假设它们是整数）
        for node in nodes:
            if not isinstance(node, int):
                raise ValueError(f"Invalid value for 'node': {node}. It must be an integer.")

        self.graph.set_centroids(nodes)

    def add_edge(self,
                 start: int,
                 end: int,
                 attribute_dict: dict = None) -> None:
        """**类方法 - add_edge:**<br>
         - 加一条边

        Args:
            start (int): 边的起始节点。必须是整数。
            end (int): 边的结束节点。必须是整数或浮点数。
            attribute_dict (dict): 包含边缘属性的字典，例如权重或其他属性。

        Raises:
            ValueError: 如果“start”或“end”不是整数或浮点数。
            ValueError: 如果“attribute_dict_”不是字典。
        """
        if 1:
            # 参数检查：确保 start 和 v_ 是有效的节点（假设它们是整数）
            if not isinstance(start, int):
                raise ValueError(f"Invalid value for 'start': {start}. It must be an integer.")

            if not isinstance(end, int):
                raise ValueError(f"Invalid value for 'end': {end}. It must be an integer.")

            # 初始化空字典
            if attribute_dict is None:
                attribute_dict = {}

            # 参数类型检查
            if not isinstance(attribute_dict, dict):
                raise ValueError(f"attribute_dict必须是字典类型，当前类型：{type(attribute_dict)}")

        # 假设 self.graph 是一个已定义的图对象
        self.graph.add_edge(start, end, attribute_dict)

    def add_edges(self,
                  edges: list[tuple]) -> None:
        """**类方法 - add_edges:**<br>
         - 加多条边

         Args:
             edges (list of tuple): 要添加的边列表。每条边都应该是一个元组。
             containing three elements:
                 - start (int): 边的起始节点。
                 - end (int): 边的结束节点。
                 - attribute_dict_ (dict): 一个包含边缘属性的字典，例如权重或其他属性。（可选）

         Raises:
             ValueError: 如果“边”不是列表。
             ValueError: 如果edges中的任何元素不是元组。
             ValueError: 如果任何元组都没有2-3个元素。
             ValueError: 如果“start”或“end”不是整数。
             ValueError: 如果“attribute_dict_”不是字典。
         """
        if 1:
            # 确保 edges 是一个列表
            if not isinstance(edges, list):
                raise ValueError(f"Expected 'edges' to be a list, but got {type(edges)}.")

            # 遍历列表中的每个元素，确保每个元素是一个元组，并且有三个元素
            for edge in edges:
                if not isinstance(edge, tuple):
                    raise ValueError(f"Each element in 'edges' should be a tuple, but got {type(edge)}.")

                if len(edge) < 2 or len(edge) > 3:
                    raise ValueError(f"Each tuple in 'edges' should have exactly 2-4 elements, but got {len(edge)}.")

                # 检查 start 和 end 是否是有效的节点（例如整数或字符串）
                start = edge[0]
                end = edge[1]
                attribute_dict_ = {}
                if len(edge) == 3:
                    attribute_dict_ = edge[2]

                # 检查 start end 是否为整数
                if not isinstance(start, int):
                    raise ValueError(f"Expected 'start' to be an integer, but got {type(start)}.")
                if not isinstance(end, int):
                    raise ValueError(f"Expected 'end' to be an integer, but got {type(end)}.")
                # 检查 attribute_dict_ 是否是一个字典
                if not isinstance(attribute_dict_, dict):
                    raise ValueError(f"Expected 'attribute_dict_' to be a dictionary, but got {type(attribute_dict_)}.")

        # 如果所有检查通过，调用 graph.add_edges
        self.graph.add_edges(edges)

    def remove_edge(self,
                    start: int,
                    end: int) -> None:
        """**类方法 - remove_edge:**<br>
         - 移除一条边

        Args:
            start (int): 边的起始节点。必须是整数。
            end (int): 边的结束节点。必须是整数。

        Raises:
            ValueError: 如果“start”或“end”不是整数。
        """
        # 参数检查：确保 start 和 v_ 是有效的节点（假设它们是整数）
        if not isinstance(start, int):
            raise ValueError(f"Invalid value for 'start': {start}. It must be an integer.")

        if not isinstance(end, int):
            raise ValueError(f"Invalid value for 'end': {end}. It must be an integer.")

        self.graph.remove_edge(start, end)

    def remove_edges(self,
                     edges: list[tuple[int, int]]) -> None:
        """**类方法 - remove_edges：**<br>
         - 移除多条边

        Args:
            edges (list of tuples): 要删除的边列表，其中每条边表示为两个整数的元组
                                    （开始、结束）表示边的节点。

        Raises:
            ValueError: 如果edges不是列表。
            ValueError: 如果edges中的任何元素不是元组或不包含正好有两个元素。
        """
        if 1:
            # 确保 edges 是一个列表
            if not isinstance(edges, list):
                raise ValueError(f"Expected 'edges' to be a list, but got {type(edges)}.")

            # 遍历列表中的每个元素，确保每个元素是一个元组，并且有三个元素
            for edge in edges:
                if not isinstance(edge, tuple):
                    raise ValueError(f"Each element in 'edges' should be a tuple, but got {type(edge)}.")

                if len(edge) != 2:
                    raise ValueError(f"Each tuple in 'edges' should have exactly 2 elements, but got {len(edge)}.")

        self.graph.remove_edges(edges)

    def multi_source_cost(self,
                          start_nodes: list[int],
                          method: str = "Dijkstra",
                          target: int = -1,
                          cut_off: float = float('inf'),
                          weight_name: str = None) -> dict[int, float]:
        """**类方法 - multi_source_cost：**<br>
           - 多源最短路径计算：返回最少花费

          Args:
              start_nodes (list): 多源最短路径计算的起始节点列表。列表中的每个元素代表一个起点节点。
              method (str): 用于计算最短路径的算法。它可以是以下之一：
                  -“Dijkstra”：标准Dijkstra算法。
                  默认为“Dijkstra”。
              target (int): 最短路径计算的目标节点。如果设置为-1，则表示没有特定目标。
              cut_off (float): 最短路径的最大距离限制。任何超过此值的路径都将被忽略。
              weight_name (str): 图中用于最短路径计算的边权重的名称。默认为None。

          Returns:
              dict[int, float]: 多源最短路径计算的花费结果，键为目标节点，值为对应的花费

          Raises:
              ValueError: 如果违反了以下任何一项：<br>
                  -“start_nodes”不是列表。<br>
                  -“method”不是有效的算法之一：“Dijkstra”。<br>
                  -“target”不是整数。<br>
                  -“cut_off”不是非负数。<br>
                  -“weight_name”不是字符串。
          """
        if 1:
            # 检查 start_nodes 是否是一个列表
            if not isinstance(start_nodes, list):
                raise ValueError(f"Invalid value for 'start_nodes': {start_nodes}. It must be a list.")

            # 检查 method 是否是有效的字符串
            valid_methods = ["Dijkstra"]  # 你可以根据实际情况修改
            if method not in valid_methods:
                raise ValueError(f"Invalid value for 'method': {method}. It must be one of {valid_methods}.")

            # 检查 target 是否是一个整数
            if not isinstance(target, int):
                raise ValueError(f"Invalid value for 'target': {target}. It must be an integer.")

            # 检查 cut_off 是否是一个非负数
            if not isinstance(cut_off, (int, float)) or cut_off < 0:
                raise ValueError(f"Invalid value for 'cut_off': {cut_off}. It must be a non-negative number.")

            # 检查 weight_name 是否是一个字符串或 None
            if weight_name is not None and not isinstance(weight_name, str):
                raise ValueError(f"Invalid value for 'weight_name': {weight_name}. It must be either a string or None.")

        if weight_name is None:
            weight_name = ""
        result = self.graph.multi_source_cost(start_nodes, method, target, cut_off, weight_name)

        return result

    def multi_source_path(self,
                          start_nodes: list[int],
                          method: str = "Dijkstra",
                          target: int = -1,
                          cut_off: float = float('inf'),
                          weight_name: str = None) -> dict[int, list[int]]:
        """**类方法 - multi_source_path：**<br>
         - 多源最短路径计算：返回序列路径

        Args:
            start_nodes (list[int]): 多源最短路径计算的起始节点列表。列表中的每个元素代表一个起点节点。
            method (str): 用于计算最短路径的算法。它可以是以下之一：
                -“Dijkstra”：标准Dijkstra算法。
                默认为“Dijkstra”。
            target (int): 最短路径计算的目标节点。如果设置为-1，则表示没有特定目标。
            cut_off (float): 最短路径的最大距离限制。任何超过此值的路径都将被忽略。
            weight_name (str): 图中用于最短路径计算的边权重的名称。默认为None。

        Returns:
            dict[int, list[int]]: 返回多源最短路径计算的路径结果， 键为目标节点，值为对应的序列路径

        Raises:
            ValueError: 如果违反了以下任何一项：<br>
                -“start_nodes”不是列表。<br>
                -“method”不是有效的算法之一：“Dijkstra”。<br>
                -“target”不是整数。<br>
                -“cut_off”不是非负数。<br>
                -“weight_name”不是字符串。
        """
        if 1:
            # 检查 start_nodes 是否是一个列表
            if not isinstance(start_nodes, list):
                raise ValueError(f"Invalid value for 'start_nodes': {start_nodes}. It must be a list.")

            # 检查 method 是否是有效的字符串
            valid_methods = ["Dijkstra"]  # 你可以根据实际情况修改
            if method not in valid_methods:
                raise ValueError(f"Invalid value for 'method': {method}. It must be one of {valid_methods}.")

            # 检查 target 是否是一个整数
            if not isinstance(target, int):
                raise ValueError(f"Invalid value for 'target': {target}. It must be an integer.")

            # 检查 cut_off 是否是一个非负数
            if not isinstance(cut_off, (int, float)) or cut_off < 0:
                raise ValueError(f"Invalid value for 'cut_off': {cut_off}. It must be a non-negative number.")

            # 检查 weight_name 是否是一个字符串或 None
            if weight_name is not None and not isinstance(weight_name, str):
                raise ValueError(f"Invalid value for 'weight_name': {weight_name}. It must be either a string or None.")

        if weight_name is None:
            weight_name = ""
        result = self.graph.multi_source_path(start_nodes, method, target, cut_off, weight_name)

        return result

    def multi_source_all(self,
                         start_nodes: list[int],
                         method: str = "Dijkstra",
                         target: int = -1,
                         cut_off: float = float('inf'),
                         weight_name: str = None) -> dict:
        """**类方法 - multi_source_all：**<br>
         - 多源最短路径计算：返回序列路径和最少花费

        Args:
            start_nodes (list[int]): 多源最短路径计算的起始节点列表。列表中的每个元素代表一个起点节点。
            method (str): 用于计算最短路径的算法。它可以是以下之一：
                -“Dijkstra”：标准Dijkstra算法。
                默认为“Dijkstra”。
            target (int): 最短路径计算的目标节点。如果设置为-1，则表示没有特定目标。
            cut_off (float): 最短路径的最大距离限制。任何超过此值的路径都将被忽略。
            weight_name (str): 图中用于最短路径计算的边权重的名称。默认为None。

        Returns:
            dis_and_path: 自定义结构体，内有{cost,paths}两个属性，返回多源最短路径计算的花费和路径结果，<br>
                         假设返回结果为： res，则 <br>
                          res.cost为 dict[int, float]: 多源最短路径计算的花费结果，键为目标节点，值为对应的花费 <br>
                          res.paths为 dict[int, list[int]]: 多源最短路径计算的路径结果， 键为目标节点，值为对应的序列路径

        Raises:
            ValueError: 如果违反了以下任何一项：<br>
                -“start_nodes”不是列表。<br>
                -“method”不是有效的算法之一：“Dijkstra”。<br>
                -“target”不是整数。<br>
                -“cut_off”不是非负数。<br>
                -“weight_name”不是字符串。
        """
        if 1:
            # 检查 start_nodes 是否是一个列表
            if not isinstance(start_nodes, list):
                raise ValueError(f"Invalid value for 'start_nodes': {start_nodes}. It must be a list.")

            # 检查 method 是否是有效的字符串
            valid_methods = ["Dijkstra"]  # 你可以根据实际情况修改
            if method not in valid_methods:
                raise ValueError(f"Invalid value for 'method': {method}. It must be one of {valid_methods}.")

            # 检查 target 是否是一个整数
            if not isinstance(target, int):
                raise ValueError(f"Invalid value for 'target': {target}. It must be an integer.")

            # 检查 cut_off 是否是一个非负数
            if not isinstance(cut_off, (int, float)) or cut_off < 0:
                raise ValueError(f"Invalid value for 'cut_off': {cut_off}. It must be a non-negative number.")

            # 检查 weight_name 是否是一个字符串或 None
            if weight_name is not None and not isinstance(weight_name, str):
                raise ValueError(f"Invalid value for 'weight_name': {weight_name}. It must be either a string or None.")
        if weight_name is None:
            weight_name = ""
        result = self.graph.multi_source_all(start_nodes, method, target, cut_off, weight_name)

        return result

    def single_source_cost(self,
                          start: int,
                          method: str = "Dijkstra",
                          target: int = -1,
                          cut_off: float = float('inf'),
                          weight_name: str = None) -> dict[int, float]:
        """**类方法 - single_source_cost：**<br>
         - 单源最短路径计算，返回最少花费

        Args:
            start (int): 多源最短路径计算的起始节点。
            method (str): 用于计算最短路径的算法。它可以是以下之一：
                -“Dijkstra”：标准Dijkstra算法。
                默认为“Dijkstra”。
            target (int): 最短路径计算的目标节点。如果设置为-1，则表示没有特定目标。
            cut_off (float): 最短路径的最大距离限制。任何超过此值的路径都将被忽略。
            weight_name (str): 图中用于最短路径计算的边权重的名称。默认为None。

        Returns:
            dict[int, float]: 单源最短路径计算的花费结果，键为目标节点，值为对应的花费

        Raises:
            ValueError: 如果违反了以下任何一项：<br>
                -“start”不是整数。<br>
                -“method”不是有效的算法之一：“Dijkstra”。<br>
                -“target”不是整数。<br>
                -“cut_off”不是非负数。<br>
                -“weight_name”不是字符串。
        """
        if 1:
            # 检查 start_nodes 是否是一个列表
            if not isinstance(start, int):
                raise ValueError(f"Invalid value for 'start': {start}. It must be a list.")

            # 检查 method 是否是有效的字符串
            valid_methods = ["Dijkstra"]  # 你可以根据实际情况修改
            if method not in valid_methods:
                raise ValueError(f"Invalid value for 'method': {method}. It must be one of {valid_methods}.")

            # 检查 target 是否是一个整数
            if not isinstance(target, int):
                raise ValueError(f"Invalid value for 'target': {target}. It must be an integer.")

            # 检查 cut_off 是否是一个非负数
            if not isinstance(cut_off, (int, float)) or cut_off < 0:
                raise ValueError(f"Invalid value for 'cut_off': {cut_off}. It must be a non-negative number.")

            # 检查 weight_name 是否是一个字符串或 None
            if weight_name is not None and not isinstance(weight_name, str):
                raise ValueError(f"Invalid value for 'weight_name': {weight_name}. It must be either a string or None.")

        if weight_name is None:
            weight_name = ""
        result = self.graph.single_source_cost(start, method, target, cut_off, weight_name)

        return result

    def single_source_path(self,
                           start: int,
                           method: str = "Dijkstra",
                           target: int = -1,
                           cut_off: float = float('inf'),
                           weight_name: str = None) -> dict[int, list[int]]:
        """**类方法 - single_source_path：**<br>
         - 单源最短路径计算，返回花费

        Args:
            start (int): 多源最短路径计算的起始节点。
            method (str): 用于计算最短路径的算法。它可以是以下之一：
                -“Dijkstra”：标准Dijkstra算法。
                默认为“Dijkstra”。
            target (int): 最短路径计算的目标节点。如果设置为-1，则表示没有特定目标。
            cut_off (float): 最短路径的最大距离限制。任何超过此值的路径都将被忽略。
            weight_name (str): 图中用于最短路径计算的边权重的名称。默认为 None。

        Returns:
            dict[int, list[int]]: 返回单源最短路径计算的路径结果， 键为目标节点，值为对应的序列路径

        Raises:
            ValueError: 如果违反了以下任何一项：<br>
                -“start”不是整数。<br>
                -“method”不是有效的算法之一：“Dijkstra”。<br>
                -“target”不是整数。<br>
                -“cut_off”不是非负数。<br>
                -“weight_name”不是字符串。
        """
        if 1:
            # 检查 list_o 是否是一个列表
            if not isinstance(start, int):
                raise ValueError(f"Invalid value for 'start': {start}. It must be a int.")

            # 检查 method 是否是有效的字符串
            valid_methods = ["Dijkstra"]  # 你可以根据实际情况修改
            if method not in valid_methods:
                raise ValueError(f"Invalid value for 'method': {method}. It must be one of {valid_methods}.")

            # 检查 target 是否是一个整数
            if not isinstance(target, int):
                raise ValueError(f"Invalid value for 'target': {target}. It must be an integer.")

            # 检查 cut_off 是否是一个非负数
            if not isinstance(cut_off, (int, float)) or cut_off < 0:
                raise ValueError(f"Invalid value for 'cut_off': {cut_off}. It must be a non-negative number.")

            # 检查 weight_name 是否是一个字符串或 None
            if weight_name is not None and not isinstance(weight_name, str):
                raise ValueError(f"Invalid value for 'weight_name': {weight_name}. It must be either a string or None.")

        if weight_name is None:
            weight_name = ""
        result = self.graph.single_source_path(start, method, target, cut_off, weight_name)

        return result

    def single_source_all(self,
                          start: int,
                          method: str = "Dijkstra",
                          target: int = -1,
                          cut_off: float = float('inf'),
                          weight_name: str = None) -> dict:
        """**类方法 - single_source_all：**<br>
         - 单源最短路径计算，返回花费和路径

        Args:
            start (int): 单源最短路径计算的起始节点。
            method (str): 用于计算最短路径的算法。它可以是以下之一：
                -“Dijkstra”：标准Dijkstra算法。
                默认为“Dijkstra”。
            target (int): 最短路径计算的目标节点。如果设置为-1，则表示没有特定目标。
            cut_off (float): 最短路径的最大距离限制。任何超过此值的路径都将被忽略。
            weight_name (str): 图中用于最短路径计算的边权重的名称。默认为“无”。

        Returns:
            dis_and_path: 自定义结构体，内有{cost,paths}两个属性，返回单源最短路径计算的花费和路径结果，<br>
                         假设返回结果为： res，则 <br>
                          res.cost为 dict[int, float]: 单源最短路径计算的花费结果，键为目标节点，值为对应的花费 <br>
                          res.paths为 dict[int, list[int]]: 单源最短路径计算的路径结果，键为目标节点，值为对应的序列路径

        Raises:
            ValueError: 如果违反了以下任何一项：<br>
                -“start”不是整数。<br>
                -“method”不是有效的算法之一：“Dijkstra”。<br>
                -“target”不是整数。<br>
                -“cut_off”不是非负数。<br>
                -“weight_name”不是字符串。
        """
        if 1:
            # 检查 list_o 是否是一个列表
            if not isinstance(start, int):
                raise ValueError(f"Invalid value for 'start': {start}. It must be a int.")

            # 检查 method 是否是有效的字符串
            valid_methods = ["Dijkstra"]  # 你可以根据实际情况修改
            if method not in valid_methods:
                raise ValueError(f"Invalid value for 'method': {method}. It must be one of {valid_methods}.")

            # 检查 target 是否是一个整数
            if not isinstance(target, int):
                raise ValueError(f"Invalid value for 'target': {target}. It must be an integer.")

            # 检查 cut_off 是否是一个非负数
            if not isinstance(cut_off, (int, float)) or cut_off < 0:
                raise ValueError(f"Invalid value for 'cut_off': {cut_off}. It must be a non-negative number.")

            # 检查 weight_name 是否是一个字符串或 None
            if weight_name is not None and not isinstance(weight_name, str):
                raise ValueError(f"Invalid value for 'weight_name': {weight_name}. It must be either a string or None.")

        # 如果 weight_name 是 None, 不传递该参数
        if weight_name is None:
            weight_name = ""
        result = self.graph.single_source_all(start, method, target, cut_off, weight_name)

        return result

    def multi_single_source_cost(self,
                         start_nodes: list[int],
                         method: str = "Dijkstra",
                         target: int = -1,
                         cut_off: float = float('inf'),
                         weight_name: str = None,
                         num_thread: int = 1) -> list[dict[int, float]]:
        """**类方法 - multi_single_source_cost：**<br>
         - 多个单源最短路径， 返回花费列表

        Args:
            start_nodes (list): 路径搜索的起始节点列表。每个项目列表中的值必须是整数。
            method (str): 用于寻路的算法。有效选项为“Dijkstra”。默认为“Dijkstra”。
            target (int): 要到达的目标节点。如果-1，则查找所有节点的路径。必须是非负整数。
            cut_off (float): 要搜索的最大距离。如果其总权重超过该值,路径被丢弃。默认值为无穷大。
            weight_name (str): 用作寻路权重的边属性的名称算法。默认值为“无”。
            num_thread (int): 用于并行计算的线程数。默认值为1。必须是整数。

        Returns:
            list[dict[int, float]]: 返回多个单源最短路径计算的路径结果<br>
                                列表内的元素是每个源节点的单源最短计算花费字典dict[int, float]

        Raises:
            ValueError: 如果任何输入参数的类型或值无效：<br>
                -“start_nodes”必须是整数列表。<br>
                -“method”必须是“Dijkstra”。<br>
                -“target”必须是非负整数。<br>
                -“cut_off”必须是非负数。<br>
                -“weight_name”必须是字符串。<br>
                -“num_thread”必须是整数。
        """
        if 1:
            # 检查 list_o 是否是一个列表
            if not isinstance(start_nodes, list):
                raise ValueError(f"Invalid value for 'start_nodes': {start_nodes}. It must be a list.")

            # 检查 method 是否是有效的字符串
            valid_methods = ["Dijkstra"]  # 你可以根据实际情况修改
            if method not in valid_methods:
                raise ValueError(f"Invalid value for 'method': {method}. It must be one of {valid_methods}.")

            # 检查 target 是否是一个整数
            if not isinstance(target, int):
                raise ValueError(f"Invalid value for 'target': {target}. It must be a integer.")

            # 检查 cut_off 是否是一个非负浮动数
            if not isinstance(cut_off, (int, float)) or cut_off < 0:
                raise ValueError(f"Invalid value for 'cut_off': {cut_off}. It must be a non-negative number.")

            # 检查 weight_name 是否是一个字符串或 None
            if weight_name is not None and not isinstance(weight_name, str):
                raise ValueError(f"Invalid value for 'weight_name': {weight_name}. It must be either a string or None.")

            # 检查 num_thread 是否是一个整数
            if not isinstance(num_thread, int):
                raise ValueError(f"Invalid value for 'num_thread': {num_thread}. It must be a integer.")

        # 如果 weight_name 是 None, 不传递该参数
        if weight_name is None:
            weight_name = ""
        result = self.graph.multi_single_source_cost(start_nodes, method, target, cut_off, weight_name, num_thread)

        return result

    def multi_single_source_path(self,
                         start_nodes: list[int],
                         method: str = "Dijkstra",
                         target: int = -1,
                         cut_off: float = float('inf'),
                         weight_name: str = None,
                         num_thread: int = 1) -> list[dict[int, list[int]]]:
        """**类方法 - multi_single_source_path：**<br>
         - 多个单源最短路径， 返回路径列表

        Args:
            start_nodes (list): 路径搜索的起始节点列表。每个项目列表中的值必须是整数。
            method (str): 用于寻路的算法。有效选项为“Dijkstra”。默认为“Dijkstra”。
            target (int): 要到达的目标节点。如果-1，则查找所有节点的路径。必须是非负整数。
            cut_off (float): 要搜索的最大距离。如果其总权重超过该值,路径被丢弃。默认值为无穷大。
            weight_name (str): 用作寻路权重的边属性的名称算法。默认值为“无”。
            num_thread (int): 用于并行计算的线程数。默认值为1。必须是整数。

        Returns:
            list[dict[int, list[int]]]: 返回多个单源最短路径计算的路径结果<br>
                                        列表内的每个元素是每个源节点的单源最短路径dict[int, list[int]]

        Raises:
            ValueError: 如果任何输入参数的类型或值无效：<br>
                -“start_nodes”必须是整数列表。<br>
                -“method”必须是“Dijkstra”。<br>
                -“target”必须是非负整数。<br>
                -“cut_off”必须是非负数。<br>
                -“weight_name”必须是字符串。<br>
                -“num_thread”必须是整数。
        """
        if 1:
            # 检查 list_o 是否是一个列表
            if not isinstance(start_nodes, list):
                raise ValueError(f"Invalid value for 'start_nodes': {start_nodes}. It must be a list.")

            # 检查 method 是否是有效的字符串
            valid_methods = ["Dijkstra"]  # 你可以根据实际情况修改
            if method not in valid_methods:
                raise ValueError(f"Invalid value for 'method': {method}. It must be one of {valid_methods}.")

            # 检查 target 是否是一个整数
            if not isinstance(target, int):
                raise ValueError(f"Invalid value for 'target': {target}. It must be a integer.")

            # 检查 cut_off 是否是一个非负浮动数
            if not isinstance(cut_off, (int, float)) or cut_off < 0:
                raise ValueError(f"Invalid value for 'cut_off': {cut_off}. It must be a non-negative number.")

            # 检查 weight_name 是否是一个字符串或 None
            if weight_name is not None and not isinstance(weight_name, str):
                raise ValueError(f"Invalid value for 'weight_name': {weight_name}. It must be either a string or None.")

            # 检查 num_thread 是否是一个整数
            if not isinstance(num_thread, int):
                raise ValueError(f"Invalid value for 'num_thread': {num_thread}. It must be a integer.")

        if weight_name is None:
            weight_name = ""
        result = self.graph.multi_single_source_path(start_nodes, method, target, cut_off, weight_name, num_thread)

        return result

    def multi_single_source_all(self,
                         start_nodes: list[int],
                         method: str = "Dijkstra",
                         target: int = -1,
                         cut_off: float = float('inf'),
                         weight_name: str = None,
                         num_thread: int = 1) -> dict:
        """**类方法 - multi_single_source_all：**<br>
         - 多个单源最短路径， 返回路径列表和花费

        Args:
            start_nodes (list): 路径搜索的起始节点列表。每个项目列表中的值必须是整数。
            method (str): 用于寻路的算法。有效选项为“Dijkstra”。默认为“Dijkstra”。
            target (int): 要到达的目标节点。如果-1，则查找所有节点的路径。必须是非负整数。
            cut_off (float or int): 要搜索的最大距离。如果其总权重超过该值,路径被丢弃。默认值为无穷大。
            weight_name (str): 用作寻路权重的边属性的名称算法。默认值为“无”。
            num_thread (int): 用于并行计算的线程数。默认值为1。必须是整数。

        Returns:
            dict: 返回多个单源最短路径计算的路径结果，二维列表：元素是每个源节点的结构体dis_and_path,此结构体包含两个属性{cost,paths} <br>
                    假设返回结果为： res_list，则 <br>
                          结果的第k个元素：res_list[k]: <br>
                          res_list[k].cost为 dict[int, float]: 单源最短路径计算的花费结果，键为目标节点，值为对应的花费 <br>
                          res_list[k].paths为 dict[int, list[int]]: 单源最短路径计算的路径结果，键为目标节点，值为对应的序列路径

        Raises:
            ValueError: 如果任何输入参数的类型或值无效：<br>
                -“start_nodes”必须是整数列表。<br>
                -“method”必须是“Dijkstra”。<br>
                -“target”必须是非负整数。<br>
                -“cut_off”必须是非负数。<br>
                -“weight_name”必须是字符串。<br>
                -“num_thread”必须是整数。
        """
        if 1:
            # 检查 list_o 是否是一个列表
            if not isinstance(start_nodes, list):
                raise ValueError(f"Invalid value for 'list_o': {start_nodes}. It must be a list.")

            # 检查 method 是否是有效的字符串
            valid_methods = ["Dijkstra"]  # 你可以根据实际情况修改
            if method not in valid_methods:
                raise ValueError(f"Invalid value for 'method': {method}. It must be one of {valid_methods}.")

            # 检查 target 是否是一个整数
            if not isinstance(target, int):
                raise ValueError(f"Invalid value for 'target': {target}. It must be a integer.")

            # 检查 cut_off 是否是一个非负浮动数
            if not isinstance(cut_off, (int, float)) or cut_off < 0:
                raise ValueError(f"Invalid value for 'cut_off': {cut_off}. It must be a non-negative number.")

            # 检查 weight_name 是否是一个字符串或 None
            if weight_name is not None and not isinstance(weight_name, str):
                raise ValueError(f"Invalid value for 'weight_name': {weight_name}. It must be either a string or None.")

            # 检查 num_thread 是否是一个整数
            if not isinstance(num_thread, int):
                raise ValueError(f"Invalid value for 'num_thread': {num_thread}. It must be a integer.")

        if weight_name is None:
            weight_name = ""
        result = self.graph.multi_single_source_all(start_nodes, method, target, cut_off, weight_name, num_thread)

        return result

    def multi_multi_source_cost(self,
                         start_nodes: list[list[int]],
                         method: str = "Dijkstra",
                         target: int = -1,
                         cut_off: float = float('inf'),
                         weight_name: str = None,
                         num_thread: int = 1) -> list[dict[int, float]]:
        """**类方法 - multi_multi_source_cost：**<br>
         - 多个多源最短路径， 返回最短路径花费

        Args:
            start_nodes (list): 路径搜索的起始节点二维列表。每个列表中的列表是一个多源节点列表。
            method (str): 用于寻路的算法。有效选项为“Dijkstra”。默认为“Dijkstra”。
            target (int): 要到达的目标节点。如果-1，则查找所有节点的路径。必须是非负整数。
            cut_off (float or int): 要搜索的最大距离。如果其总权重超过该值,路径被丢弃。默认值为无穷大。
            weight_name (str): 用作寻路权重的边属性的名称算法。默认值为“无”。
            num_thread (int): 用于并行计算的线程数。默认值为1。必须是整数。

        Returns:
            list[dict[int, float]]: 返回多个多源最短路径计算的路径结果<br>
                                    列表内的元素是每个多源最短路的花费字典dict[int, float]

        Raises:
            ValueError: 如果任何输入参数的类型或值无效：<br>
                -“start_nodes”必须是二维整数列表。<br>
                -“method”必须是“Dijkstra”。<br>
                -“target”必须是非负整数。<br>
                -“cut_off”必须是非负数。<br>
                -“weight_name”必须是字符串。<br>
                -“num_thread”必须是整数。
        """
        if 1:
            # 检查 list_o 是否是一个列表
            if not isinstance(start_nodes, list):
                raise ValueError(f"Invalid value for 'list_o': {start_nodes}. It must be a list.")

            # 检查 start_nodes 是否是二维列表
            if not all(isinstance(node, list) for node in start_nodes):
                raise ValueError(f"Invalid value for 'start_nodes': {start_nodes}. It must be a list of lists.")

            # 检查 method 是否是有效的字符串
            valid_methods = ["Dijkstra"]  # 你可以根据实际情况修改
            if method not in valid_methods:
                raise ValueError(f"Invalid value for 'method': {method}. It must be one of {valid_methods}.")

            # 检查 target 是否是一个整数
            if not isinstance(target, int):
                raise ValueError(f"Invalid value for 'target': {target}. It must be a integer.")

            # 检查 cut_off 是否是一个非负浮动数
            if not isinstance(cut_off, (int, float)) or cut_off < 0:
                raise ValueError(f"Invalid value for 'cut_off': {cut_off}. It must be a non-negative number.")

            # 检查 weight_name 是否是一个字符串或 None
            if weight_name is not None and not isinstance(weight_name, str):
                raise ValueError(f"Invalid value for 'weight_name': {weight_name}. It must be either a string or None.")

            # 检查 num_thread 是否是一个整数
            if not isinstance(num_thread, int):
                raise ValueError(f"Invalid value for 'num_thread': {num_thread}. It must be a integer.")

        if weight_name is None:
            weight_name = ""
        result = self.graph.multi_multi_source_cost(start_nodes, method, target, cut_off, weight_name, num_thread)

        return result

    def multi_multi_source_path(self,
                         start_nodes: list[list[int]],
                         method: str = "Dijkstra",
                         target: int = -1,
                         cut_off: float = float('inf'),
                         weight_name: str = None,
                         num_thread: int = 1) -> list[dict[int, list[int]]]:
        """**类方法 - multi_multi_source_path：**<br>
         - 多个多源最短路径， 返回最短计算路径

        Args:
            start_nodes (list): 路径搜索的起始节点二维列表。每个列表中的列表是一个多源节点列表。
            method (str): 用于寻路的算法。有效选项为“Dijkstra”。默认为“Dijkstra”。
            target (int): 要到达的目标节点。如果-1，则查找所有节点的路径。必须是非负整数。
            cut_off (float or int): 要搜索的最大距离。如果其总权重超过该值,路径被丢弃。默认值为无穷大。
            weight_name (str): 用作寻路权重的边属性的名称算法。默认值为“无”。
            num_thread (int): 用于并行计算的线程数。默认值为1。必须是整数。

        Returns:
            list[dict[int, list[int]]]: 返回多个多源最短路径计算的路径结果<br>
                    列表内的每个元素是每个多源最短路的路径字典 dict[int, list[int]]

        Raises:
            ValueError: 如果任何输入参数的类型或值无效：<br>
                -“start_nodes”必须是二维整数列表。<br>
                -“method”必须是“Dijkstra”。<br>
                -“target”必须是非负整数。<br>
                -“cut_off”必须是非负数。<br>
                -“weight_name”必须是字符串。<br>
                -“num_thread”必须是整数。
        """
        if 1:
            # 检查 list_o 是否是一个列表
            if not isinstance(start_nodes, list):
                raise ValueError(f"Invalid value for 'start_nodes': {start_nodes}. It must be a list.")

            # 检查 start_nodes 是否是二维列表
            if not all(isinstance(node, list) for node in start_nodes):
                raise ValueError(f"Invalid value for 'start_nodes': {start_nodes}. It must be a list of lists.")

            # 检查 method 是否是有效的字符串
            valid_methods = ["Dijkstra"]  # 你可以根据实际情况修改
            if method not in valid_methods:
                raise ValueError(f"Invalid value for 'method': {method}. It must be one of {valid_methods}.")

            # 检查 target 是否是一个整数
            if not isinstance(target, int):
                raise ValueError(f"Invalid value for 'target': {target}. It must be a integer.")

            # 检查 cut_off 是否是一个非负浮动数
            if not isinstance(cut_off, (int, float)) or cut_off < 0:
                raise ValueError(f"Invalid value for 'cut_off': {cut_off}. It must be a non-negative number.")

            # 检查 weight_name 是否是一个字符串或 None
            if weight_name is not None and not isinstance(weight_name, str):
                raise ValueError(f"Invalid value for 'weight_name': {weight_name}. It must be either a string or None.")

            # 检查 num_thread 是否是一个整数
            if not isinstance(num_thread, int):
                raise ValueError(f"Invalid value for 'num_thread': {num_thread}. It must be a integer.")

        if weight_name is None:
            weight_name = ""
        result = self.graph.multi_multi_source_path(start_nodes, method, target, cut_off, weight_name, num_thread)

        return result

    def multi_multi_source_all(self,
                         start_nodes: list[list[int]],
                         method: str = "Dijkstra",
                         target: int = -1,
                         cut_off: float = float('inf'),
                         weight_name: str = None,
                         num_thread: int = 1) -> dict:
        """**类方法 - multi_multi_source_all：**<br>
         - 多个多源最短路径， 返回所有最短计算路径和花费

        Args:
            start_nodes (list): 路径搜索的起始节点二维列表。每个列表中的列表是一个多源节点列表。
            method (str): 用于寻路的算法。有效选项为“Dijkstra”。默认为“Dijkstra”。
            target (int): 要到达的目标节点。如果-1，则查找所有节点的路径。必须是非负整数。
            cut_off (float or int): 要搜索的最大距离。如果其总权重超过该值,路径被丢弃。默认值为无穷大。
            weight_name (str): 用作寻路权重的边属性的名称算法。默认值为“无”。
            num_thread (int): 用于并行计算的线程数。默认值为1。必须是整数。

        Returns:
            result: dict: 返回多个单源最短路径计算的路径结果，二维列表：元素是每个源节点的结构体dis_and_path,此结构体包含两个属性{cost,paths} <br>
                    假设返回结果为： res_list，则 <br>
                          结果的第k个元素：res_list[k]: <br>
                          res_list[k].cost为 dict[int, float]: 多源最短路径计算的花费结果，键为目标节点，值为对应的花费 <br>
                          res_list[k].paths为 dict[int, list[int]]: 多源最短路径计算的路径结果，键为目标节点，值为对应的序列路径


        Raises:
            ValueError: 如果任何输入参数的类型或值无效：<br>
                -“start_nodes”必须是二维整数列表。<br>
                -“method”必须是“Dijkstra”。<br>
                -“target”必须是非负整数。<br>
                -“cut_off”必须是非负数。<br>
                -“weight_name”必须是字符串。<br>
                -“num_thread”必须是整数。
        """
        if 1:
            # 检查 list_o 是否是一个列表
            if not isinstance(start_nodes, list):
                raise ValueError(f"Invalid value for 'start_nodes': {start_nodes}. It must be a list.")

            # 检查 start_nodes 是否是二维列表
            if not all(isinstance(node, list) for node in start_nodes):
                raise ValueError(f"Invalid value for 'start_nodes': {start_nodes}. It must be a list of lists.")

            # 检查 method 是否是有效的字符串
            valid_methods = ["Dijkstra"]  # 你可以根据实际情况修改
            if method not in valid_methods:
                raise ValueError(f"Invalid value for 'method': {method}. It must be one of {valid_methods}.")

            # 检查 target 是否是一个整数
            if not isinstance(target, int):
                raise ValueError(f"Invalid value for 'target': {target}. It must be a integer.")

            # 检查 cut_off 是否是一个非负浮动数
            if not isinstance(cut_off, (int, float)) or cut_off < 0:
                raise ValueError(f"Invalid value for 'cut_off': {cut_off}. It must be a non-negative number.")

            # 检查 weight_name 是否是一个字符串或 None
            if weight_name is not None and not isinstance(weight_name, str):
                raise ValueError(f"Invalid value for 'weight_name': {weight_name}. It must be either a string or None.")

            # 检查 num_thread 是否是一个整数
            if not isinstance(num_thread, int):
                raise ValueError(f"Invalid value for 'num_thread': {num_thread}. It must be a integer.")

        if weight_name is None:
            weight_name = ""
        result = self.graph.multi_multi_source_all(start_nodes, method, target, cut_off, weight_name, num_thread)

        return result

    def cost_matrix(self,
                 start_nodes: list[int],
                 end_nodes: list[int],
                 method: str = "Dijkstra",
                 cut_off: float = float('inf'),
                 weight_name: str = None,
                 num_thread: int = 1) -> np.ndarray:
        """**类方法 - cost_matrix：**
         - 输入起点列表和终点列表，计算获得一个起点到终点的花费矩阵

        Args:
            start_nodes (list of ints): 路径搜索的起始节点列表。每个元素都应该可以是表示单个起始节点的整数。
            end_nodes (list of ints): 路径搜索的结束节点列表。每个元素都应该。
            method (str): 用于寻路的算法。有效选项为“Dijkstra”。默认为“Dijkstra”。
            cut_off (float or int): 要搜索的最大距离。路径被丢弃如果其总重量超过该值。默认值为无穷大。
            weight_name (str): 用作寻路权重的边属性的名称算法。默认值为“无”。
            num_thread (int): 用于并行计算的线程数。默认值为1。必须是整数。

        Raises:
            ValueError: 如果任何输入参数的类型或值无效：<br>
                -“start_nodes”必须是整数列表。<br>
                -“end_nodes”必须是整数列表。<br>
                -“method”必须是“Dijkstra”。<br>
                -“cut_off”必须是非负数。<br>
                -“weight_name”必须是字符串。<br>
                -“num_thread”必须是整数。

        Returns:
            numpy.ndarray: 一个numpy数组，其中每个元素表示最短路径成本<br>
                数组的维度将是len（start_nodes）x len（end_nodes”）<br>
                假设返回结果是res： 则<br>
                res[m][n]代表了从m到n的花费
        """
        if 1:
            # 检查 start_nodes 是否是一个列表
            if not isinstance(start_nodes, list):
                raise ValueError(f"Invalid value for 'start_nodes': {start_nodes}. It must be a list.")

            # 检查 end_nodes 是否是一个列表
            if not isinstance(end_nodes, list):
                raise ValueError(f"Invalid value for 'end_nodes': {end_nodes}. It must be a list.")

            # 检查 method 是否是有效的字符串
            valid_methods = ["Dijkstra"]  # 你可以根据实际情况修改
            if method not in valid_methods:
                raise ValueError(f"Invalid value for 'method': {method}. It must be one of {valid_methods}.")

            # 检查 cut_off 是否是一个非负浮动数
            if not isinstance(cut_off, (int, float)) or cut_off < 0:
                raise ValueError(f"Invalid value for 'cut_off': {cut_off}. It must be a non-negative number.")

            # 检查 weight_name 是否是一个字符串或 None
            if weight_name is not None and not isinstance(weight_name, str):
                raise ValueError(f"Invalid value for 'weight_name': {weight_name}. It must be either a string or None.")

            # 检查 num_thread 是否是一个整数
            if not isinstance(num_thread, int):
                raise ValueError(f"Invalid value for 'num_thread': {num_thread}. It must be a integer.")

        if weight_name is None:
            weight_name = ""
        result = self.graph.cost_matrix(start_nodes, end_nodes, method, cut_off, weight_name, num_thread)

        return result

    def path_dict(self,
                  start_nodes: list[int],
                  end_nodes: list[int],
                  method: str = "Dijkstra",
                  cut_off: float = float('inf'),
                  weight_name: str = None,
                  num_thread: int = 1) -> dict[tuple[int, int], list[int]]:
        """**类方法 - path_dict：**
         - - 输入起点列表和终点列表，计算获得一个起点到终点的路径列表

        Args:
            start_nodes (list of ints): 路径搜索的起始节点列表。每个元素都应该可以是表示单个起始节点的整数。
            end_nodes (list of ints): 路径搜索的结束节点列表。每个元素都应该。
            method (str): 用于寻路的算法。有效选项为“Dijkstra”。默认为“Dijkstra”。
            cut_off (float or int): 要搜索的最大距离。路径被丢弃如果其总重量超过该值。默认值为无穷大。
            weight_name (str): 用作寻路权重的边属性的名称算法。默认值为“无”。
            num_thread (int): 用于并行计算的线程数。默认值为1。必须是整数。

        Raises:
            ValueError: 如果任何输入参数的类型或值无效：<br>
                -“start_nodes”必须是整数列表。<br>
                -“end_nodes”必须是整数列表。<br>
                -“method”必须是“Dijkstra”。<br>
                -“cut_off”必须是非负数。<br>
                -“weight_name”必须是字符串。<br>
                -“num_thread”必须是整数。

        Returns:
            dict[tuple[int, int], list[int]]: 字典，其中每个键表示 起点-终点 的路径（作为节点列表），每个值表示最短路径。 例如：<br>
                [(1,3):[2,3],<br>
                (1,4):[2,3,4]]
        """
        if 1:
            # 检查 start_nodes 是否是一个列表
            if not isinstance(start_nodes, list):
                raise ValueError(f"Invalid value for 'start_nodes': {start_nodes}. It must be a list.")

            # 检查 end_nodes 是否是一个列表
            if not isinstance(end_nodes, list):
                raise ValueError(f"Invalid value for 'end_nodes': {end_nodes}. It must be a list.")

            # 检查 method 是否是有效的字符串
            valid_methods = ["Dijkstra"]  # 你可以根据实际情况修改
            if method not in valid_methods:
                raise ValueError(f"Invalid value for 'method': {method}. It must be one of {valid_methods}.")

            # 检查 cut_off 是否是一个非负浮动数
            if not isinstance(cut_off, (int, float)) or cut_off < 0:
                raise ValueError(f"Invalid value for 'cut_off': {cut_off}. It must be a non-negative number.")

            # 检查 weight_name 是否是一个字符串或 None
            if weight_name is not None and not isinstance(weight_name, str):
                raise ValueError(f"Invalid value for 'weight_name': {weight_name}. It must be either a string or None.")

            # 检查 num_thread 是否是一个整数
            if not isinstance(num_thread, int):
                raise ValueError(f"Invalid value for 'num_thread': {num_thread}. It must be a integer.")

        if weight_name is None:
            weight_name = ""
        result = self.graph.path_dict(start_nodes, end_nodes, method, cut_off, weight_name, num_thread)

        return result

    def k_shortest_paths(self,
                       source: int,
                       target: int,
                       num: int,
                       weight_name: str = None) -> list[list[int]]:
        """**类方法 - k_shortest_paths：**
        - - 输入起点和目标点，和要获取的最短路数量，获得花费从小到大的最短路径序列

        Args:
            source (int): 路径搜索的起点。
            target (int): 路径搜索的终点。
            num (int): 要获取最短路径的数量。
            weight_name (str): 要使用的权重的字段名称。默认值为None。

        Raises:
            ValueError: 如果任何输入参数的类型或值无效：<br>
                -“source”必须是整数。<br>
                -“target”必须是整数。<br>
                -“num”必须是整数。<br>
                -“weight_name”必须是字符串。<br>

        Returns:
            list[list[int]]: 二维列表，按花费从小到大返回序列路径。 例如：<br>
                [[1,3,5],
                 [1,2,5],
                 [1,4,5]]
        """
        if 1:
            # 检查 source 是否是一个列表
            if not isinstance(source, int):
                raise ValueError(f"Invalid value for 'source': {source}. It must be a int.")

            # 检查 target 是否是一个列表
            if not isinstance(target, int):
                raise ValueError(f"Invalid value for 'target': {target}. It must be a int.")

            # 检查 num 是否是一个列表
            if not isinstance(num, int):
                raise ValueError(f"Invalid value for 'num': {num}. It must be a int.")

            # 检查 weight_name 是否是一个字符串或 None
            if weight_name is not None and not isinstance(weight_name, str):
                raise ValueError(f"Invalid value for 'weight_name': {weight_name}. It must be either a string or None.")

        if weight_name is None:
            weight_name = ""
        result = self.graph.k_shortest_paths(source, target, num, weight_name)

        return result

    def shortest_path_cost(self,
                       source: int,
                       target: int,
                       weight_name: str = None) -> float:
        """**类方法 - shortest_path_cost：**
        - - 输入起点和目标点，和要获取的最短路数量，获得花费从小到大的最短路径序列

        Args:
           source (int): 路径搜索的起点。
           target (int): 路径搜索的终点。
           weight_name (str): 要使用的权重的字段名称。默认值为None。

        Raises:
            ValueError: 如果任何输入参数的类型或值无效：<br>
               -“source”必须是整数。<br>
               -“target”必须是整数。<br>
               -“weight_name”必须是字符串。<br>

        Returns:
            double: 最短路径到达目标节点的花费。 例如：<br>
               2.0
        """
        if 1:
            # 检查 source 是否是一个列表
            if not isinstance(source, int):
                raise ValueError(f"Invalid value for 'source': {source}. It must be a int.")

            # 检查 target 是否是一个列表
            if not isinstance(target, int):
                raise ValueError(f"Invalid value for 'target': {target}. It must be a int.")

            # 检查 weight_name 是否是一个字符串或 None
            if weight_name is not None and not isinstance(weight_name, str):
                raise ValueError(f"Invalid value for 'weight_name': {weight_name}. It must be either a string or None.")

        if weight_name is None:
            weight_name = ""
        result = self.graph.shortest_path_cost(source, target, weight_name)

        return result

    def shortest_path_path(self,
                       source: int,
                       target: int,
                       weight_name: str = None) -> list[int]:
        """**类方法 - shortest_path_path：**
        - - 输入起点和目标点，和要获取的最短路数量，获得花费从小到大的最短路径序列

        Args:
           source (int): 路径搜索的起点。
           target (int): 路径搜索的终点。
           weight_name (str): 要使用的权重的字段名称。默认值为None。

        Raises:
            ValueError: 如果任何输入参数的类型或值无效：<br>
               -“source”必须是整数。<br>
               -“target”必须是整数。<br>
               -“weight_name”必须是字符串。

        Returns:
            list[int]: 最短路径到达目标节点的路径。 例如：<br>
               [1,4,5]
        """
        if 1:
            # 检查 source 是否是一个列表
            if not isinstance(source, int):
                raise ValueError(f"Invalid value for 'source': {source}. It must be a int.")

            # 检查 target 是否是一个列表
            if not isinstance(target, int):
                raise ValueError(f"Invalid value for 'target': {target}. It must be a int.")

            # 检查 weight_name 是否是一个字符串或 None
            if weight_name is not None and not isinstance(weight_name, str):
                raise ValueError(f"Invalid value for 'weight_name': {weight_name}. It must be either a string or None.")

        if weight_name is None:
            weight_name = ""
        result = self.graph.shortest_path_path(source, target, weight_name)

        return result

    def shortest_path_all(self,
                       source: int,
                       target: int,
                       weight_name: str = None) -> tuple[float, list[int]]:
        """**类方法 - shortest_path_all：**
        - - 输入起点和目标点，和要获取的最短路数量，获得花费从小到大的最短路径序列

        Args:
           source (int): 路径搜索的起点。
           target (int): 路径搜索的终点。
           weight_name (str): 要使用的权重的字段名称。默认值为None。

        Raises:
            ValueError: 如果任何输入参数的类型或值无效：<br>
               -“source”必须是整数。<br>
               -“target”必须是整数。<br>
               -“weight_name”必须是字符串。<br>

        Returns:
            tuple[float, list[int]]: 最短路径到达目标节点的花费和路径。 例如：<br>
               [2.0, [1, 4, 5]]
        """
        if 1:
            # 检查 source 是否是一个列表
            if not isinstance(source, int):
                raise ValueError(f"Invalid value for 'source': {source}. It must be a int.")

            # 检查 target 是否是一个列表
            if not isinstance(target, int):
                raise ValueError(f"Invalid value for 'target': {target}. It must be a int.")

            # 检查 weight_name 是否是一个字符串或 None
            if weight_name is not None and not isinstance(weight_name, str):
                raise ValueError(f"Invalid value for 'weight_name': {weight_name}. It must be either a string or None.")

        if weight_name is None:
            weight_name = ""
        result = self.graph.shortest_path_all(source, target, weight_name)

        return result

    def path_dict_pairwise(self,
                          start_nodes: list[int],
                          end_nodes: list[int],
                          method: str = "Dijkstra",
                          weight_name: str = None,
                          num_thread: int = 1) -> dict[tuple[int, int], list[int]]:
        """**类方法 - path_dict_pairwise：**
         - - 输入起点列表和终点列表，计算获得一一对应的起点到终点的路径

        Args:
            start_nodes (list of ints): 路径搜索的起始节点列表。每个元素都应该可以是表示单个起始节点的整数。
            end_nodes (list of ints): 路径搜索的结束节点列表。每个元素都应该。
            method (str): 用于寻路的算法。有效选项为“Dijkstra”。默认为“Dijkstra”。
            weight_name (str): 用作寻路权重的边属性的名称算法。默认值为“无”。
            num_thread (int): 用于并行计算的线程数。默认值为1。必须是整数。

        Raises:
            ValueError: 如果任何输入参数的类型或值无效：<br>
                -"start_nodes"和"end_nodes"的长度必须一致。<br>
                -“start_nodes”必须是整数列表。<br>
                -“end_nodes”必须是整数列表。<br>
                -“method”必须是“Dijkstra”。<br>
                -“weight_name”必须是字符串。<br>
                -“num_thread”必须是整数。

        Returns:
            dict[tuple[int, int], list[int]]: 字典，其中每个键表示 起点-终点 的路径（作为节点列表），每个值表示最短路径。 例如：<br>
                [(1,3):[2,3],<br>
                (1,4):[2,3,4]]
        """
        if 1:
            # 检查 start_nodes 是否是一个列表
            if not isinstance(start_nodes, list):
                raise ValueError(f"Invalid value for 'start_nodes': {start_nodes}. It must be a list.")

            # 检查 end_nodes 是否是一个列表
            if not isinstance(end_nodes, list):
                raise ValueError(f"Invalid value for 'end_nodes': {end_nodes}. It must be a list.")

            # 检查 start_nodes 和 end_nodes 长度一致
            if len(start_nodes) != len(end_nodes):
                raise ValueError(f"'start_nodes' and 'end_nodes' must have the same length. "
                                 f"Got {len(start_nodes)} and {len(end_nodes)}.")

            # 检查 method 是否是有效的字符串
            valid_methods = ["Dijkstra"]  # 你可以根据实际情况修改
            if method not in valid_methods:
                raise ValueError(f"Invalid value for 'method': {method}. It must be one of {valid_methods}.")

            # 检查 weight_name 是否是一个字符串或 None
            if weight_name is not None and not isinstance(weight_name, str):
                raise ValueError(f"Invalid value for 'weight_name': {weight_name}. It must be either a string or None.")

            # 检查 num_thread 是否是一个整数
            if not isinstance(num_thread, int):
                raise ValueError(f"Invalid value for 'num_thread': {num_thread}. It must be a integer.")

        if weight_name is None:
            weight_name = ""
        result = self.graph.path_dict_pairwise(start_nodes, end_nodes, method, weight_name, num_thread)

        return result

    def gotrackit_calc(self,
                       seq_k_candidate_info: dataframe,
                       gps_adj_dis_map: dict[int, float],
                       use_global_cache: bool = False,
                       not_conn_cost: float = None,
                       num_thread: int = 1,
                       cut_off: float = float('inf'),
                       weight_name:  str = None) -> dict[tuple[int, int], list[int]]:
        """**类方法 - gotrackit_calc：**
         - - gotrackit计算状态转移矩阵

        Args:
            seq_k_candidate_info (dataframe): 输入需要处理的df数据。
            gps_adj_dis_map (list[int]): gps字典。
            use_global_cache (bool)： 是否使用全局变量。
            not_conn_cost (float): 一个常数。
            num_thread (int): 用于并行计算的线程数。默认值为1。必须是整数。
            cut_off (float): 截断距离。
            weight_name (str): 用作寻路权重的边属性的名称算法。默认值为“无”。

        Raises:
            ValueError: 如果任何输入参数的类型或值无效：<br>
                -"start_nodes"和"end_nodes"的长度必须一致。<br>
                -“start_nodes”必须是整数列表。<br>
                -“end_nodes”必须是整数列表。<br>
                -“method”必须是“Dijkstra”。<br>
                -“weight_name”必须是字符串。<br>
                -“num_thread”必须是整数。

        Returns:
            dict[tuple[int, int], list[int]]: 字典，其中每个键表示 起点-终点 的路径（作为节点列表），每个值表示最短路径。 例如：<br>
                [(1,3):[2,3],<br>
                (1,4):[2,3,4]]
        """
        # 检查 seq_k_candidate_info 是否为 dataframe 类型
        if not isinstance(seq_k_candidate_info, dataframe):
            raise ValueError(f"Invalid value for 'seq_k_candidate_info': {seq_k_candidate_info}. It must be a dataframe.")

        # 检查 gps_adj_dis_map 是否为整数列表
        if not isinstance(gps_adj_dis_map, dict):
            raise ValueError(f"Invalid value for 'gps_adj_dis_map': {gps_adj_dis_map}. It must be a list of dict.")

        # 检查 use_global_cache 是否为布尔值
        if not isinstance(use_global_cache, bool):
            raise ValueError(f"Invalid value for 'use_global_cache': {use_global_cache}. It must be a boolean.")

        # 检查 not_conn_cost 是否为浮动数值或 None
        if not_conn_cost is not None and not isinstance(not_conn_cost, float):
            raise ValueError(f"Invalid value for 'not_conn_cost': {not_conn_cost}. It must be a float or None.")

        # 检查 num_thread 是否为整数
        if not isinstance(num_thread, int):
            raise ValueError(f"Invalid value for 'num_thread': {num_thread}. It must be an integer.")

        # 检查 cut_off 是否为浮动数值
        if not isinstance(cut_off, float):
            raise ValueError(f"Invalid value for 'cut_off': {cut_off}. It must be a float.")

        # 检查 weight_name 是否为字符串或 None
        if weight_name is not None and not isinstance(weight_name, str):
            raise ValueError(f"Invalid value for 'weight_name': {weight_name}. It must be either a string or None.")

        # 默认情况下，设置 weight_name 为 ""（如果是 None）
        if weight_name is None:
            weight_name = ""

        # 调用 graph.path_dict_pairwise 方法来计算结果
        result = self.graph.gotrackit_calc(seq_k_candidate_info, gps_adj_dis_map, use_global_cache,
                                               not_conn_cost, num_thread, cut_off, weight_name)

        return result