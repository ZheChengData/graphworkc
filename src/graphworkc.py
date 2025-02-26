import graphwork


class CGraph:
    def __init__(self):
        # 创建 C++ 图算法对象
        self.graph = graphwork.GraphAlgorithms()

    def get_graph_info(self):
        """**类方法 - get_graph_info:** <br>
        - 获取图信息

        Example:
            Getting graph information <br>
            graph.get_graph_info()
        """
        self.graph.get_graph_info()

    def get_node_info(self, id: int = None):
        """**类方法 - get_node_info:** <br>
        - 获取节点信息

        Args:
            id (int): 节点的标识符。必须是一个整数，表示图中节点的唯一标识符。

        Raises:
            ValueError: 如果“id”不是整数。
            AttributeError: 如果图形对象没有`get_node_info`方法。

        Example:
            Getting information about node 1 <br>
            graph.get_node_info(1)
        """
        # 参数检查：确保 id 是有效的节点（假设它们是整数）
        if not isinstance(id, int):
            raise ValueError(f"Invalid value for 'start': {start}. It must be an integer.")

        self.graph.get_node_info(id)

    def get_link_info(self, start: int = None, end: int = None):
        """**类方法 - get_link_info:** <br>
        - 获取边信息

        Args:
            start (int): 边的起始节点。必须是整数。
            end (int): 边的结束节点。必须是整数。

        Raises:
            ValueError: 如果“start”或“end”不是整数。
            AttributeError: 如果图对象没有`get_link_info`方法。

        Example:
            Getting information about the link between node 1 and node 2<br>
            graph.get_link_info(1, 2)
        """
        # 参数检查：确保 start 和 end 是有效的节点（假设它们是整数）
        if not isinstance(start, int):
            raise ValueError(f"Invalid value for 'start': {start}. It must be an integer.")

        if not isinstance(end, int):
            raise ValueError(f"Invalid value for 'end': {end}. It must be an integer.")

        self.graph.get_link_info(start, end)

    def add_edge(self, start=int, end=int, attribute_dict=dict):
        """**类方法 - add_edge:**<br>
         - 加一条边

        Args:
            start (int): 边的起始节点。必须是整数。
            end (int): 边的结束节点。必须是整数或浮点数。
            attribute_dict (dict): 包含边缘属性的字典，例如权重或其他属性。不能为空。

        Raises:
            ValueError: 如果“start”或“end”不是整数或浮点数。
            ValueError: 如果“attribute_dict_”不是字典。
            ValueError: 如果“attribute_dict_”是空字典。

        Example:
            Adding an edge from node 1 to node 2 with attributes<br>
            edge_attributes = {"weight": 10, "color": "blue"}<br>
            graph.add_edge(1, 2, edge_attributes)
        """
        # 参数检查：确保 start 和 v_ 是有效的节点（假设它们是整数）
        if not isinstance(start, int):
            raise ValueError(f"Invalid value for 'start': {start}. It must be an integer.")

        if not isinstance(end, int):
            raise ValueError(f"Invalid value for 'end': {end}. It must be an integer.")

        # 参数检查：确保 attribute_dict_ 是一个字典
        if not isinstance(attribute_dict, dict):
            raise ValueError(f"Invalid value for 'attribute_dict': {attribute_dict}. It must be a dictionary.")

        # 参数检查：确保字典为空时不要传入
        if not attribute_dict:
            raise ValueError("The 'attribute_dict' cannot be an empty dictionary.")

        # 假设 self.graph 是一个已定义的图对象
        self.graph.add_edge(start, end, attribute_dict)

    def add_edges(self, edges):
        """**类方法 - add_edges:**<br>
         - 加多条边

         Args:
             edges (list of tuple): 要添加的边列表。每条边都应该是一个元组。
             containing three elements:
                 - start (int): 边的起始节点。
                 - end (int): 边的结束节点。
                 - attribute_dict_ (dict): 一个包含边缘属性的字典，例如权重或其他属性。

         Raises:
             ValueError: 如果“边”不是列表。
             ValueError: 如果'edges'中的任何元素不是元组。
             ValueError: 如果任何元组都没有3个元素。
             ValueError: 如果“start”或“end”不是整数。
             ValueError: 如果“attribute_dict_”不是字典。

         Example:
             edges = [<br>
                 (1, 2, {"weight": 5}),<br>
                 (2, 3, {"weight": 10}),<br>
                 (3, 4, {"weight": 15})<br>
             ]<br>
             graph_instance.add_edges(edges)
         """
        # 确保 edges 是一个列表
        if not isinstance(edges, list):
            raise ValueError(f"Expected 'edges' to be a list, but got {type(edges)}.")

        # 遍历列表中的每个元素，确保每个元素是一个元组，并且有三个元素
        for edge in edges:
            if not isinstance(edge, tuple):
                raise ValueError(f"Each element in 'edges' should be a tuple, but got {type(edge)}.")

            if len(edge) != 3:
                raise ValueError(f"Each tuple in 'edges' should have exactly 3 elements, but got {len(edge)}.")

            # 检查 start 和 end 是否是有效的节点（例如整数或字符串）
            start, end, attribute_dict_ = edge
            if not isinstance(start, int):
                raise ValueError(f"Expected 'start' to be an integer, but got {type(start)}.")
            if not isinstance(end, int):
                raise ValueError(f"Expected 'end' to be an integer, but got {type(end)}.")

            # 检查 attribute_dict_ 是否是一个字典
            if not isinstance(attribute_dict_, dict):
                raise ValueError(f"Expected 'attribute_dict_' to be a dictionary, but got {type(attribute_dict_)}.")

        # 如果所有检查通过，调用 graph.add_edges
        self.graph.add_edges(edges)

    def remove_edge(self, start=int, end=int):
        """**类方法 - remove_edge:**<br>
         - 移除一条边

        Args:
            start (int): 边的起始节点。必须是整数。
            end (int): 边的结束节点。必须是整数。

        Raises:
            ValueError: 如果“start”或“end”不是整数。

        Example:
            Removing an edge between node 1 and node 2<br>
            graph.remove_edge(1, 2)
        """
        # 参数检查：确保 start 和 v_ 是有效的节点（假设它们是整数）
        if not isinstance(start, int):
            raise ValueError(f"Invalid value for 'start': {start}. It must be an integer.")

        if not isinstance(end, int):
            raise ValueError(f"Invalid value for 'end': {end}. It must be an integer.")

        self.graph.remove_edge(start, end)

    def remove_edges(self, edges):
        """**类方法 - remove_edges：**<br>
         - 移除多条边

        Args:
            edges (list of tuples): 要删除的边列表，其中每条边表示为两个整数的元组
                                    （开始、结束）表示边的节点。

        Raises:
            ValueError: 如果“edges”不是列表。
            ValueError: 如果'edges'中的任何元素不是元组或不包含正好有两个元素。

        Example:
            Removing multiple edges<br>
            edges_to_remove = [(1, 2), (3, 4), (5, 6)]<br>
            graph_instance.remove_edges(edges_to_remove)
        """
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
                          start_nodes=list,
                          method="Dijkstra",
                          target=-1,
                          cutoff=float('inf'),
                          weight_name="none"):
        """**类方法 - multi_source_cost：**<br>
         - 多源最短路径计算：返回花费

        Args:
            start_nodes (list): 多源最短路径计算的起始节点列表。列表中的每个元素代表一个起点节点。
            method (str): 用于计算最短路径的算法。它可以是以下之一：
                -“Dijkstra”：标准Dijkstra算法。
                -“A*”：A*搜索算法（需要启发式函数）。
                -“Bellman Ford”：Bellman Ford算法（处理负权重）。
                默认为“Dijkstra”。
            target (int): 最短路径计算的目标节点。如果设置为-1，则表示没有特定目标。
            cutoff (float): 最短路径的最大距离限制。任何超过此值的路径都将被忽略。
            weight_name (str): 图中用于最短路径计算的边权重的名称。默认为“无”。

        Returns:
            result: 返回多源最短路径计算的花费结果，一维列表

        Raises:
            ValueError: 如果违反了以下任何一项：<br>
                -“start_nodes”不是列表。<br>
                -“method”不是有效的算法之一：“Dijkstra”、“A*”或“Bellman Ford”。<br>
                -“target”不是整数。<br>
                -“cutoff”不是非负数。<br>
                -“weight_name”不是字符串。

        Example:
            result = multi_source_cost(<br>
                start_nodes=[0, 1],<br>
                method="A*",<br>
                target=5,<br>
                cutoff=100.0,<br>
                weight_name="distance")
        """
        # 检查 list_o 是否是一个列表
        if not isinstance(start_nodes, list):
            raise ValueError(f"Invalid value for 'list_o': {start_nodes}. It must be a list.")

        # 检查 method 是否是有效的字符串
        valid_methods = ["Dijkstra", "A*", "Bellman-Ford"]  # 你可以根据实际情况修改
        if method not in valid_methods:
            raise ValueError(f"Invalid value for 'method': {method}. It must be one of {valid_methods}.")

        # 检查 target 是否是一个非负整数
        if not isinstance(target, int):
            raise ValueError(f"Invalid value for 'target': {target}. It must be a integer.")

        # 检查 cutoff 是否是一个非负浮动数
        if not isinstance(cutoff, (int, float)) or cutoff < 0:
            raise ValueError(f"Invalid value for 'cutoff': {cutoff}. It must be a non-negative number.")

        # 检查 weight_name 是否是一个字符串
        if not isinstance(weight_name, str):
            raise ValueError(f"Invalid value for 'weight_name': {weight_name}. It must be a string.")

        result = self.graph.multi_source_cost(start_nodes, method, target, cutoff, weight_name)
        return result

    def multi_source_path(self,
                          start_nodes=list,
                          method="Dijkstra",
                          target=-1,
                          cutoff=float('inf'),
                          weight_name="none"):
        """**类方法 - multi_source_path：**<br>
         - 多源最短路径计算：返回路径

        Args:
            start_nodes (list): 多源最短路径计算的起始节点列表。列表中的每个元素代表一个起点节点。
            method (str): 用于计算最短路径的算法。它可以是以下之一：
                -“Dijkstra”：标准Dijkstra算法。
                -“A*”：A*搜索算法（需要启发式函数）。
                -“Bellman Ford”：Bellman Ford算法（处理负权重）。
                默认为“Dijkstra”。
            target (int): 最短路径计算的目标节点。如果设置为-1，则表示没有特定目标。
            cutoff (float): 最短路径的最大距离限制。任何超过此值的路径都将被忽略。
            weight_name (str): 图中用于最短路径计算的边权重的名称。默认为“无”。

        Returns:
            result: 返回多源最短路径计算的路径结果，一维字典

        Raises:
            ValueError: 如果违反了以下任何一项：<br>
                -“start_nodes”不是列表。<br>
                -“method”不是有效的算法之一：“Dijkstra”、“A*”或“Bellman Ford”。<br>
                -“target”不是整数。<br>
                -“cutoff”不是非负数。<br>
                -“weight_name”不是字符串。

        Example:
            result = multi_source_path(<br>
                start_nodes=[0, 1],<br>
                method="A*",<br>
                target=5,<br>
                cutoff=100.0,<br>
                weight_name="distance")
        """
        # 检查 list_o 是否是一个列表
        if not isinstance(start_nodes, list):
            raise ValueError(f"Invalid value for 'list_o': {start_nodes}. It must be a list.")

        # 检查 method 是否是有效的字符串
        valid_methods = ["Dijkstra", "A*", "Bellman-Ford"]  # 你可以根据实际情况修改
        if method not in valid_methods:
            raise ValueError(f"Invalid value for 'method': {method}. It must be one of {valid_methods}.")

        # 检查 target 是否是一个非负整数
        if not isinstance(target, int):
            raise ValueError(f"Invalid value for 'target': {target}. It must be a non-negative integer.")

        # 检查 cutoff 是否是一个非负浮动数
        if not isinstance(cutoff, (int, float)) or cutoff < 0:
            raise ValueError(f"Invalid value for 'cutoff': {cutoff}. It must be a number.")

        # 检查 weight_name 是否是一个字符串
        if not isinstance(weight_name, str):
            raise ValueError(f"Invalid value for 'weight_name': {weight_name}. It must be a string.")

        result = self.graph.multi_source_path(start_nodes, method, target, cutoff, weight_name)
        return result

    def multi_source_all(self,
                         start_nodes=list,
                         method="Dijkstra",
                         target=-1,
                         cutoff=float('inf'),
                         weight_name="none"):
        """**类方法 - multi_source_all：**<br>
         - 多源最短路径计算：返回路径和花费

        Args:
            start_nodes (list): 多源最短路径计算的起始节点列表。列表中的每个元素代表一个起点节点。
            method (str): 用于计算最短路径的算法。它可以是以下之一：
                -“Dijkstra”：标准Dijkstra算法。
                -“A*”：A*搜索算法（需要启发式函数）。
                -“Bellman Ford”：Bellman Ford算法（处理负权重）。
                默认为“Dijkstra”。
            target (int): 最短路径计算的目标节点。如果设置为-1，则表示没有特定目标。
            cutoff (float): 最短路径的最大距离限制。任何超过此值的路径都将被忽略。
            weight_name (str): 图中用于最短路径计算的边权重的名称。默认为“无”。

        Returns:
            result: 返回多源最短路径计算的花费和路径结果，一个result结构体，内有{cost,paths}两个属性

        Raises:
            ValueError: 如果违反了以下任何一项：<br>
                -“start_nodes”不是列表。<br>
                -“method”不是有效的算法之一：“Dijkstra”、“A*”或“Bellman Ford”。<br>
                -“target”不是整数。<br>
                -“cutoff”不是非负数。<br>
                -“weight_name”不是字符串。

        Example:
            result = multi_source_all(<br>
                start_nodes=[0, 1],<br>
                method="A*",<br>
                target=5,<br>
                cutoff=100.0,<br>
                weight_name="distance")
        """
        # 检查 list_o 是否是一个列表
        if not isinstance(start_nodes, list):
            raise ValueError(f"Invalid value for 'list_o': {start_nodes}. It must be a list.")

        # 检查 method 是否是有效的字符串
        valid_methods = ["Dijkstra", "A*", "Bellman-Ford"]  # 你可以根据实际情况修改
        if method not in valid_methods:
            raise ValueError(f"Invalid value for 'method': {method}. It must be one of {valid_methods}.")

        # 检查 target 是否是一个非负整数
        if not isinstance(target, int):
            raise ValueError(f"Invalid value for 'target': {target}. It must be a integer.")

        # 检查 cutoff 是否是一个非负浮动数
        if not isinstance(cutoff, (int, float)) or cutoff < 0:
            raise ValueError(f"Invalid value for 'cutoff': {cutoff}. It must be a non-negative number.")

        # 检查 weight_name 是否是一个字符串
        if not isinstance(weight_name, str):
            raise ValueError(f"Invalid value for 'weight_name': {weight_name}. It must be a string.")

        result = self.graph.multi_source_all(start_nodes, method, target, cutoff, weight_name)
        return result

    def single_source_cost(self,
                          start=int,
                          method="Dijkstra",
                          target=-1,
                          cutoff=float('inf'),
                          weight_name="none"):
        """**类方法 - single_source_cost：**<br>
         - 单源最短路径计算，返回花费

        Args:
            start (int): 多源最短路径计算的起始节点。
            method (str): 用于计算最短路径的算法。它可以是以下之一：
                -“Dijkstra”：标准Dijkstra算法。
                -“A*”：A*搜索算法（需要启发式函数）。
                -“Bellman Ford”：Bellman Ford算法（处理负权重）。
                默认为“Dijkstra”。
            target (int): 最短路径计算的目标节点。如果设置为-1，则表示没有特定目标。
            cutoff (float): 最短路径的最大距离限制。任何超过此值的路径都将被忽略。
            weight_name (str): 图中用于最短路径计算的边权重的名称。默认为“无”。

        Returns:
            result: 返回单源最短路径计算的花费结果，一维列表

        Raises:
            ValueError: 如果违反了以下任何一项：<br>
                -“start”不是整数。<br>
                -“method”不是有效的算法之一：“Dijkstra”、“A*”或“Bellman Ford”。<br>
                -“target”不是整数。<br>
                -“cutoff”不是非负数。<br>
                -“weight_name”不是字符串。

        Example:
            result = single_source_cost(<br>
                start=1,<br>
                method="Bellman-Ford",<br>
                target=5,<br>
                cutoff=100.0,<br>
                weight_name="distance"
            )
        """
        # 检查 start 是否是一个整数
        if not isinstance(start, int):
            raise ValueError(f"Invalid value for 'start': {start}. It must be a int.")

        # 检查 method 是否是有效的字符串
        valid_methods = ["Dijkstra", "A*", "Bellman-Ford"]  # 你可以根据实际情况修改
        if method not in valid_methods:
            raise ValueError(f"Invalid value for 'method': {method}. It must be one of {valid_methods}.")

        # 检查 target 是否是一个非负整数
        if not isinstance(target, int):
            raise ValueError(f"Invalid value for 'target': {target}. It must be a integer.")

        # 检查 cutoff 是否是一个非负浮动数
        if not isinstance(cutoff, (int, float)) or cutoff < 0:
            raise ValueError(f"Invalid value for 'cutoff': {cutoff}. It must be a non-negative number.")

        # 检查 weight_name 是否是一个字符串
        if not isinstance(weight_name, str):
            raise ValueError(f"Invalid value for 'weight_name': {weight_name}. It must be a string.")

        result = self.graph.single_source_cost(start, method, target, cutoff, weight_name)
        return result

    def single_source_path(self,
                          start=int,
                          method="Dijkstra",
                          target=-1,
                          cutoff=float('inf'),
                          weight_name="none"):
        """**类方法 - single_source_path：**<br>
         - 单源最短路径计算，返回花费

        Args:
            start (int): 多源最短路径计算的起始节点。
            method (str): 用于计算最短路径的算法。它可以是以下之一：
                -“Dijkstra”：标准Dijkstra算法。
                -“A*”：A*搜索算法（需要启发式函数）。
                -“Bellman Ford”：Bellman Ford算法（处理负权重）。
                默认为“Dijkstra”。
            target (int): 最短路径计算的目标节点。如果设置为-1，则表示没有特定目标。
            cutoff (float): 最短路径的最大距离限制。任何超过此值的路径都将被忽略。
            weight_name (str): 图中用于最短路径计算的边权重的名称。默认为“无”。

        Returns:
            result: 返回单源最短路径计算的路径结果，一个result结构体，内有{cost,paths}两个属性

        Raises:
            ValueError: 如果违反了以下任何一项：<br>
                -“start”不是整数。<br>
                -“method”不是有效的算法之一：“Dijkstra”、“A*”或“Bellman Ford”。<br>
                -“target”不是整数。<br>
                -“cutoff”不是非负数。<br>
                -“weight_name”不是字符串。

        Example:
            result = single_source_path(<br>
                start=1,<br>
                method="Bellman-Ford",<br>
                target=5,<br>
                cutoff=100.0,<br>
                weight_name="distance"
            )
        """
        # 检查 list_o 是否是一个列表
        if not isinstance(start, int):
            raise ValueError(f"Invalid value for 'start': {start}. It must be a int.")

        # 检查 method 是否是有效的字符串
        valid_methods = ["Dijkstra", "A*", "Bellman-Ford"]  # 你可以根据实际情况修改
        if method not in valid_methods:
            raise ValueError(f"Invalid value for 'method': {method}. It must be one of {valid_methods}.")

        # 检查 target 是否是一个非负整数
        if not isinstance(target, int):
            raise ValueError(f"Invalid value for 'target': {target}. It must be a non-negative integer.")

        # 检查 cutoff 是否是一个非负浮动数
        if not isinstance(cutoff, (int, float)) or cutoff < 0:
            raise ValueError(f"Invalid value for 'cutoff': {cutoff}. It must be a number.")

        # 检查 weight_name 是否是一个字符串
        if not isinstance(weight_name, str):
            raise ValueError(f"Invalid value for 'weight_name': {weight_name}. It must be a string.")

        result = self.graph.single_source_path(start, method, target, cutoff, weight_name)
        return result

    def single_source_all(self,
                         start=int,
                         method="Dijkstra",
                         target=-1,
                         cutoff=float('inf'),
                         weight_name="none"):
        """**类方法 - single_source_all：**<br>
         - 单源最短路径计算，返回花费和路径

        Args:
            start (int): 单源最短路径计算的起始节点。
            method (str): 用于计算最短路径的算法。它可以是以下之一：
                -“Dijkstra”：标准Dijkstra算法。
                -“A*”：A*搜索算法（需要启发式函数）。
                -“Bellman Ford”：Bellman Ford算法（处理负权重）。
                默认为“Dijkstra”。
            target (int): 最短路径计算的目标节点。如果设置为-1，则表示没有特定目标。
            cutoff (float): 最短路径的最大距离限制。任何超过此值的路径都将被忽略。
            weight_name (str): 图中用于最短路径计算的边权重的名称。默认为“无”。

        Returns:
            result: 返回单源最短路径计算的路径结果，一维字典

        Raises:
            ValueError: 如果违反了以下任何一项：<br>
                -“start”不是整数。<br>
                -“method”不是有效的算法之一：“Dijkstra”、“A*”或“Bellman Ford”。<br>
                -“target”不是整数。<br>
                -“cutoff”不是非负数。<br>
                -“weight_name”不是字符串。

        Example:
            result = single_source_all(<br>
                start=1,<br>
                method="Bellman-Ford",<br>
                target=5,<br>
                cutoff=100.0,<br>
                weight_name="distance"
            )
        """
        # 检查 list_o 是否是一个列表
        if not isinstance(start, int):
            raise ValueError(f"Invalid value for 'start': {start}. It must be a int.")

        # 检查 method 是否是有效的字符串
        valid_methods = ["Dijkstra", "A*", "Bellman-Ford"]  # 你可以根据实际情况修改
        if method not in valid_methods:
            raise ValueError(f"Invalid value for 'method': {method}. It must be one of {valid_methods}.")

        # 检查 target 是否是一个非负整数
        if not isinstance(target, int):
            raise ValueError(f"Invalid value for 'target': {target}. It must be a integer.")

        # 检查 cutoff 是否是一个非负浮动数
        if not isinstance(cutoff, (int, float)) or cutoff < 0:
            raise ValueError(f"Invalid value for 'cutoff': {cutoff}. It must be a non-negative number.")

        # 检查 weight_name 是否是一个字符串
        if not isinstance(weight_name, str):
            raise ValueError(f"Invalid value for 'weight_name': {weight_name}. It must be a string.")

        result = self.graph.single_source_all(start, method, target, cutoff, weight_name)
        return result

    def multi_single_source_cost(self,
                         start_nodes=list,
                         method="Dijkstra",
                         target=-1,
                         cutoff=float('inf'),
                         weight_name="none",
                         num_thread=1):
        """**类方法 - multi_single_source_cost：**<br>
         - 多个单源最短路径， 返回花费列表

        Args:
            start_nodes (list): 路径搜索的起始节点列表。每个项目列表中的值必须是整数。
            method (str): 用于寻路的算法。有效选项为“Dijkstra”，“A*”和“Bellman-Ford”。默认为“Dijkstra”。
            target (int): 要到达的目标节点。如果-1，则查找所有节点的路径。必须是非负整数。
            cutoff (float or int): 要搜索的最大距离。如果其总权重超过该值,路径被丢弃。默认值为无穷大。
            weight_name (str): 用作寻路权重的边属性的名称算法。默认值为“无”。
            num_thread (int): 用于并行计算的线程数。默认值为1。必须是整数。

        Returns:
            result: 返回多个单源最短路径计算的路径结果，二维列表：元素是每个源节点的单源最短计算花费

        Raises:
            ValueError: 如果任何输入参数的类型或值无效：<br>
                -“start_nodes”必须是整数列表。<br>
                -“method”必须是“Dijkstra”、“A*”或“Bellman Ford”之一。<br>
                -“target”必须是非负整数。<br>
                -“cutoff”必须是非负数。<br>
                -“weight_name”必须是字符串。<br>
                -“num_thread”必须是整数。

        Example:
            result = multi_single_source_cost(<br>
                start_nodes=[1,2,3],<br>
                method="Dijkstra",<br>
                target=10,<br>
                cutoff=100.0,<br>
                weight_name="distance",<br>
                num_thread=10
            )
        """
        # 检查 list_o 是否是一个列表
        if not isinstance(start_nodes, list):
            raise ValueError(f"Invalid value for 'list_o': {start_nodes}. It must be a list.")

        # 检查 method 是否是有效的字符串
        valid_methods = ["Dijkstra", "A*", "Bellman-Ford"]  # 你可以根据实际情况修改
        if method not in valid_methods:
            raise ValueError(f"Invalid value for 'method': {method}. It must be one of {valid_methods}.")

        # 检查 target 是否是一个整数
        if not isinstance(target, int):
            raise ValueError(f"Invalid value for 'target': {target}. It must be a integer.")

        # 检查 cutoff 是否是一个非负浮动数
        if not isinstance(cutoff, (int, float)) or cutoff < 0:
            raise ValueError(f"Invalid value for 'cutoff': {cutoff}. It must be a non-negative number.")

        # 检查 weight_name 是否是一个字符串
        if not isinstance(weight_name, str):
            raise ValueError(f"Invalid value for 'weight_name': {weight_name}. It must be a string.")

        # 检查 num_thread 是否是一个整数
        if not isinstance(num_thread, int):
            raise ValueError(f"Invalid value for 'num_thread': {num_thread}. It must be a integer.")

        result = self.graph.multi_single_source_cost(start_nodes, method, target, cutoff, weight_name, num_thread)
        return result

    def multi_single_source_path(self,
                         start_nodes=list,
                         method="Dijkstra",
                         target=-1,
                         cutoff=float('inf'),
                         weight_name="none",
                         num_thread=1):
        """**类方法 - multi_single_source_path：**<br>
         - 多个单源最短路径， 返回路径列表

        Args:
            start_nodes (list): 路径搜索的起始节点列表。每个项目列表中的值必须是整数。
            method (str): 用于寻路的算法。有效选项为“Dijkstra”，“A*”和“Bellman-Ford”。默认为“Dijkstra”。
            target (int): 要到达的目标节点。如果-1，则查找所有节点的路径。必须是非负整数。
            cutoff (float or int): 要搜索的最大距离。如果其总权重超过该值,路径被丢弃。默认值为无穷大。
            weight_name (str): 用作寻路权重的边属性的名称算法。默认值为“无”。
            num_thread (int): 用于并行计算的线程数。默认值为1。必须是整数。

        Returns:
            result: 返回多个单源最短路径计算的路径结果，二维列表：元素是每个源节点的单源最短路径

        Raises:
            ValueError: 如果任何输入参数的类型或值无效：<br>
                -“start_nodes”必须是整数列表。<br>
                -“method”必须是“Dijkstra”、“A*”或“Bellman Ford”之一。<br>
                -“target”必须是非负整数。<br>
                -“cutoff”必须是非负数。<br>
                -“weight_name”必须是字符串。<br>
                -“num_thread”必须是整数。

        Example:
            result = multi_single_source_path(<br>
                start_nodes=[1,2,3],<br>
                method="Dijkstra",<br>
                target=10,<br>
                cutoff=100.0,<br>
                weight_name="distance",<br>
                num_thread=10
            )
        """
        # 检查 list_o 是否是一个列表
        if not isinstance(start_nodes, list):
            raise ValueError(f"Invalid value for 'list_o': {start_nodes}. It must be a list.")

        # 检查 method 是否是有效的字符串
        valid_methods = ["Dijkstra", "A*", "Bellman-Ford"]  # 你可以根据实际情况修改
        if method not in valid_methods:
            raise ValueError(f"Invalid value for 'method': {method}. It must be one of {valid_methods}.")

        # 检查 target 是否是一个整数
        if not isinstance(target, int):
            raise ValueError(f"Invalid value for 'target': {target}. It must be a integer.")

        # 检查 cutoff 是否是一个非负浮动数
        if not isinstance(cutoff, (int, float)) or cutoff < 0:
            raise ValueError(f"Invalid value for 'cutoff': {cutoff}. It must be a non-negative number.")

        # 检查 weight_name 是否是一个字符串
        if not isinstance(weight_name, str):
            raise ValueError(f"Invalid value for 'weight_name': {weight_name}. It must be a string.")

        # 检查 num_thread 是否是一个整数
        if not isinstance(num_thread, int):
            raise ValueError(f"Invalid value for 'num_thread': {num_thread}. It must be a integer.")

        result = self.graph.multi_single_source_path(start_nodes, method, target, cutoff, weight_name, num_thread)
        return result

    def multi_single_source_all(self,
                         start_nodes=list,
                         method="Dijkstra",
                         target=-1,
                         cutoff=float('inf'),
                         weight_name="none",
                         num_thread=1):
        """**类方法 - multi_single_source_all：**<br>
         - 多个单源最短路径， 返回路径列表和花费

        Args:
            start_nodes (list): 路径搜索的起始节点列表。每个项目列表中的值必须是整数。
            method (str): 用于寻路的算法。有效选项为“Dijkstra”，“A*”和“Bellman-Ford”。默认为“Dijkstra”。
            target (int): 要到达的目标节点。如果-1，则查找所有节点的路径。必须是非负整数。
            cutoff (float or int): 要搜索的最大距离。如果其总权重超过该值,路径被丢弃。默认值为无穷大。
            weight_name (str): 用作寻路权重的边属性的名称算法。默认值为“无”。
            num_thread (int): 用于并行计算的线程数。默认值为1。必须是整数。

        Returns:
            result: 返回多个单源最短路径计算的路径结果，二维列表：元素是每个源节点的结构体result,此结构体包含两个属性{cost,paths}

        Raises:
            ValueError: 如果任何输入参数的类型或值无效：<br>
                -“start_nodes”必须是整数列表。<br>
                -“method”必须是“Dijkstra”、“A*”或“Bellman Ford”之一。<br>
                -“target”必须是非负整数。<br>
                -“cutoff”必须是非负数。<br>
                -“weight_name”必须是字符串。<br>
                -“num_thread”必须是整数。

        Example:
            result = multi_single_source_all(<br>
                start_nodes=[1,2,3],<br>
                method="Dijkstra",<br>
                target=10,<br>
                cutoff=100.0,<br>
                weight_name="distance",<br>
                num_thread=10
            )
        """
        # 检查 list_o 是否是一个列表
        if not isinstance(start_nodes, list):
            raise ValueError(f"Invalid value for 'list_o': {start_nodes}. It must be a list.")

        # 检查 method 是否是有效的字符串
        valid_methods = ["Dijkstra", "A*", "Bellman-Ford"]  # 你可以根据实际情况修改
        if method not in valid_methods:
            raise ValueError(f"Invalid value for 'method': {method}. It must be one of {valid_methods}.")

        # 检查 target 是否是一个整数
        if not isinstance(target, int):
            raise ValueError(f"Invalid value for 'target': {target}. It must be a integer.")

        # 检查 cutoff 是否是一个非负浮动数
        if not isinstance(cutoff, (int, float)) or cutoff < 0:
            raise ValueError(f"Invalid value for 'cutoff': {cutoff}. It must be a non-negative number.")

        # 检查 weight_name 是否是一个字符串
        if not isinstance(weight_name, str):
            raise ValueError(f"Invalid value for 'weight_name': {weight_name}. It must be a string.")

        # 检查 num_thread 是否是一个整数
        if not isinstance(num_thread, int):
            raise ValueError(f"Invalid value for 'num_thread': {num_thread}. It must be a integer.")

        result = self.graph.multi_single_source_all(start_nodes, method, target, cutoff, weight_name, num_thread)
        return result

    def multi_multi_source_cost(self,
                         start_nodes=list,
                         method="Dijkstra",
                         target=-1,
                         cutoff=float('inf'),
                         weight_name="none",
                         num_thread=1):
        """**类方法 - multi_multi_source_cost：**<br>
         - 多个多源最短路径， 返回最短路径花费

        Args:
            start_nodes (list): 路径搜索的起始节点二维列表。每个列表中的列表是一个多源节点列表。
            method (str): 用于寻路的算法。有效选项为“Dijkstra”，“A*”和“Bellman-Ford”。默认为“Dijkstra”。
            target (int): 要到达的目标节点。如果-1，则查找所有节点的路径。必须是非负整数。
            cutoff (float or int): 要搜索的最大距离。如果其总权重超过该值,路径被丢弃。默认值为无穷大。
            weight_name (str): 用作寻路权重的边属性的名称算法。默认值为“无”。
            num_thread (int): 用于并行计算的线程数。默认值为1。必须是整数。

        Returns:
            result: 返回多个多源最短路径计算的路径结果，二维列表：元素是每个多源最短路的花费列表

        Raises:
            ValueError: 如果任何输入参数的类型或值无效：<br>
                -“start_nodes”必须是二维整数列表。<br>
                -“method”必须是“Dijkstra”、“A*”或“Bellman Ford”之一。<br>
                -“target”必须是非负整数。<br>
                -“cutoff”必须是非负数。<br>
                -“weight_name”必须是字符串。<br>
                -“num_thread”必须是整数。

        Example:
            result = multi_multi_source_cost(<br>
                start_nodes=[[1,2],[3,4],[5,6]],<br>
                method="Dijkstra",<br>
                target=10,<br>
                cutoff=100.0,<br>
                weight_name="distance",<br>
                num_thread=10
            )
        """
        # 检查 list_o 是否是一个列表
        if not isinstance(start_nodes, list):
            raise ValueError(f"Invalid value for 'list_o': {start_nodes}. It must be a list.")

        # 检查 start_nodes 是否是二维列表
        if not all(isinstance(node, list) for node in start_nodes):
            raise ValueError(f"Invalid value for 'start_nodes': {start_nodes}. It must be a list of lists.")

        # 检查 method 是否是有效的字符串
        valid_methods = ["Dijkstra", "A*", "Bellman-Ford"]  # 你可以根据实际情况修改
        if method not in valid_methods:
            raise ValueError(f"Invalid value for 'method': {method}. It must be one of {valid_methods}.")

        # 检查 target 是否是一个整数
        if not isinstance(target, int):
            raise ValueError(f"Invalid value for 'target': {target}. It must be a integer.")

        # 检查 cutoff 是否是一个非负浮动数
        if not isinstance(cutoff, (int, float)) or cutoff < 0:
            raise ValueError(f"Invalid value for 'cutoff': {cutoff}. It must be a non-negative number.")

        # 检查 weight_name 是否是一个字符串
        if not isinstance(weight_name, str):
            raise ValueError(f"Invalid value for 'weight_name': {weight_name}. It must be a string.")

        # 检查 num_thread 是否是一个整数
        if not isinstance(num_thread, int):
            raise ValueError(f"Invalid value for 'num_thread': {num_thread}. It must be a integer.")

        result = self.graph.multi_multi_source_cost(start_nodes, method, target, cutoff, weight_name, num_thread)
        return result

    def multi_multi_source_path(self,
                         start_nodes=list,
                         method="Dijkstra",
                         target=-1,
                         cutoff=float('inf'),
                         weight_name="none",
                         num_thread=1):
        """**类方法 - multi_multi_source_path：**<br>
         - 多个多源最短路径， 返回最短计算路径

        Args:
            start_nodes (list): 路径搜索的起始节点二维列表。每个列表中的列表是一个多源节点列表。
            method (str): 用于寻路的算法。有效选项为“Dijkstra”，“A*”和“Bellman-Ford”。默认为“Dijkstra”。
            target (int): 要到达的目标节点。如果-1，则查找所有节点的路径。必须是非负整数。
            cutoff (float or int): 要搜索的最大距离。如果其总权重超过该值,路径被丢弃。默认值为无穷大。
            weight_name (str): 用作寻路权重的边属性的名称算法。默认值为“无”。
            num_thread (int): 用于并行计算的线程数。默认值为1。必须是整数。

        Returns:
            result: 返回多个多源最短路径计算的路径结果，二维列表：元素是每个多源最短路的花费列表

        Raises:
            ValueError: 如果任何输入参数的类型或值无效：<br>
                -“start_nodes”必须是二维整数列表。<br>
                -“method”必须是“Dijkstra”、“A*”或“Bellman Ford”之一。<br>
                -“target”必须是非负整数。<br>
                -“cutoff”必须是非负数。<br>
                -“weight_name”必须是字符串。<br>
                -“num_thread”必须是整数。

        Example:
            result = multi_multi_source_path(<br>
                start_nodes=[[1,2],[3,4],[5,6]],<br>
                method="Dijkstra",<br>
                target=10,<br>
                cutoff=100.0,<br>
                weight_name="distance",<br>
                num_thread=10
            )
        """
        # 检查 list_o 是否是一个列表
        if not isinstance(start_nodes, list):
            raise ValueError(f"Invalid value for 'list_o': {start_nodes}. It must be a list.")

        # 检查 start_nodes 是否是二维列表
        if not all(isinstance(node, list) for node in start_nodes):
            raise ValueError(f"Invalid value for 'start_nodes': {start_nodes}. It must be a list of lists.")

        # 检查 method 是否是有效的字符串
        valid_methods = ["Dijkstra", "A*", "Bellman-Ford"]  # 你可以根据实际情况修改
        if method not in valid_methods:
            raise ValueError(f"Invalid value for 'method': {method}. It must be one of {valid_methods}.")

        # 检查 target 是否是一个整数
        if not isinstance(target, int):
            raise ValueError(f"Invalid value for 'target': {target}. It must be a integer.")

        # 检查 cutoff 是否是一个非负浮动数
        if not isinstance(cutoff, (int, float)) or cutoff < 0:
            raise ValueError(f"Invalid value for 'cutoff': {cutoff}. It must be a non-negative number.")

        # 检查 weight_name 是否是一个字符串
        if not isinstance(weight_name, str):
            raise ValueError(f"Invalid value for 'weight_name': {weight_name}. It must be a string.")

        # 检查 num_thread 是否是一个整数
        if not isinstance(num_thread, int):
            raise ValueError(f"Invalid value for 'num_thread': {num_thread}. It must be a integer.")

        result = self.graph.multi_multi_source_path(start_nodes, method, target, cutoff, weight_name, num_thread)
        return result

    def multi_multi_source_all(self,
                         start_nodes=list,
                         method="Dijkstra",
                         target=-1,
                         cutoff=float('inf'),
                         weight_name="none",
                         num_thread=1):
        """**类方法 - multi_multi_source_all：**<br>
         - 多个多源最短路径， 返回所有最短计算路径和花费

        Args:
            start_nodes (list): 路径搜索的起始节点二维列表。每个列表中的列表是一个多源节点列表。
            method (str): 用于寻路的算法。有效选项为“Dijkstra”，“A*”和“Bellman-Ford”。默认为“Dijkstra”。
            target (int): 要到达的目标节点。如果-1，则查找所有节点的路径。必须是非负整数。
            cutoff (float or int): 要搜索的最大距离。如果其总权重超过该值,路径被丢弃。默认值为无穷大。
            weight_name (str): 用作寻路权重的边属性的名称算法。默认值为“无”。
            num_thread (int): 用于并行计算的线程数。默认值为1。必须是整数。

        Returns:
            result: 返回多个多源最短路径计算的路径结果，二维列表：元素是每个多源路径计算返回的结构体result,此结构体包含两个属性{cost,paths}

        Raises:
            ValueError: 如果任何输入参数的类型或值无效：<br>
                -“start_nodes”必须是二维整数列表。<br>
                -“method”必须是“Dijkstra”、“A*”或“Bellman Ford”之一。<br>
                -“target”必须是非负整数。<br>
                -“cutoff”必须是非负数。<br>
                -“weight_name”必须是字符串。<br>
                -“num_thread”必须是整数。

        Example:
            result = multi_multi_source_all(<br>
                start_nodes=[[1,2],[3,4],[5,6]],<br>
                method="Dijkstra",<br>
                target=10,<br>
                cutoff=100.0,<br>
                weight_name="distance",<br>
                num_thread=10
            )
        """
        # 检查 list_o 是否是一个列表
        if not isinstance(start_nodes, list):
            raise ValueError(f"Invalid value for 'list_o': {start_nodes}. It must be a list.")

        # 检查 start_nodes 是否是二维列表
        if not all(isinstance(node, list) for node in start_nodes):
            raise ValueError(f"Invalid value for 'start_nodes': {start_nodes}. It must be a list of lists.")

        # 检查 method 是否是有效的字符串
        valid_methods = ["Dijkstra", "A*", "Bellman-Ford"]  # 你可以根据实际情况修改
        if method not in valid_methods:
            raise ValueError(f"Invalid value for 'method': {method}. It must be one of {valid_methods}.")

        # 检查 target 是否是一个整数
        if not isinstance(target, int):
            raise ValueError(f"Invalid value for 'target': {target}. It must be a integer.")

        # 检查 cutoff 是否是一个非负浮动数
        if not isinstance(cutoff, (int, float)) or cutoff < 0:
            raise ValueError(f"Invalid value for 'cutoff': {cutoff}. It must be a non-negative number.")

        # 检查 weight_name 是否是一个字符串
        if not isinstance(weight_name, str):
            raise ValueError(f"Invalid value for 'weight_name': {weight_name}. It must be a string.")

        # 检查 num_thread 是否是一个整数
        if not isinstance(num_thread, int):
            raise ValueError(f"Invalid value for 'num_thread': {num_thread}. It must be a integer.")

        result = self.graph.multi_multi_source_all(start_nodes, method, target, cutoff, weight_name, num_thread)
        return result

    def cost_matrix_to_numpy(self,
                             start_nodes=list,
                             end_nodes=list,
                             method="Dijkstra",
                             cutoff=float('inf'),
                             weight_name="none",
                             num_thread=1):
        """**类方法 - cost_matrix_to_numpy：**
         - 输入起点列表和终点列表，计算获得一个起点到终点的花费矩阵

        Args:
            start_nodes (list of ints): 路径搜索的起始节点列表。每个元素都应该可以是表示单个起始节点的整数。
            end_nodes (list of ints): 路径搜索的结束节点列表。每个元素都应该。
            method (str): 用于寻路的算法。有效选项为“Dijkstra”，“A*”和“贝尔曼·福特”。默认为“Dijkstra”。
            cutoff (float or int): 要搜索的最大距离。路径被丢弃如果其总重量超过该值。默认值为无穷大。
            weight_name (str): 用作寻路权重的边属性的名称算法。默认值为“无”。
            num_thread (int): 用于并行计算的线程数。默认值为1。必须是整数。

        Raises:
            ValueError: 如果任何输入参数的类型或值无效：<br>
                -“start_nodes”必须是整数列表。<br>
                -“end_nodes”必须是整数列表。<br>
                -“method”必须是“Dijkstra”、“A*”或“Bellman Ford”之一。<br>
                -“cutoff”必须是非负数。<br>
                -“weight_name”必须是字符串。<br>
                -“num_thread”必须是整数。

        Returns:
            numpy.ndarray: 一个numpy数组，其中每个元素表示最短路径成本<br>
                基于指定的算法在开始节点和结束节点之间进行通信。<br>
                数组的维度将是len（start_nodes）x len（end_nodes”）。
        """
        # 检查 start_nodes 是否是一个列表
        if not isinstance(start_nodes, list):
            raise ValueError(f"Invalid value for 'start_nodes': {start_nodes}. It must be a list.")

        # 检查 end_nodes 是否是一个列表
        if not isinstance(end_nodes, list):
            raise ValueError(f"Invalid value for 'end_nodes': {end_nodes}. It must be a list.")

        # 检查 method 是否是有效的字符串
        valid_methods = ["Dijkstra", "A*", "Bellman-Ford"]  # 你可以根据实际情况修改
        if method not in valid_methods:
            raise ValueError(f"Invalid value for 'method': {method}. It must be one of {valid_methods}.")

        # 检查 cutoff 是否是一个非负浮动数
        if not isinstance(cutoff, (int, float)) or cutoff < 0:
            raise ValueError(f"Invalid value for 'cutoff': {cutoff}. It must be a non-negative number.")

        # 检查 weight_name 是否是一个字符串
        if not isinstance(weight_name, str):
            raise ValueError(f"Invalid value for 'weight_name': {weight_name}. It must be a string.")

        # 检查 num_thread 是否是一个整数
        if not isinstance(num_thread, int):
            raise ValueError(f"Invalid value for 'num_thread': {num_thread}. It must be a integer.")

        result = self.graph.cost_matrix_to_numpy(start_nodes, end_nodes, method, cutoff, weight_name, num_thread)
        return result

    def path_list_to_numpy(self,
                           start_nodes=list,
                           end_nodes=list,
                           method="Dijkstra",
                           cutoff=float('inf'),
                           weight_name="none",
                           num_thread=1):
        """**类方法 - path_list_to_numpy：**
         - - 输入起点列表和终点列表，计算获得一个起点到终点的路径列表

        Args:
            start_nodes (list of ints): 路径搜索的起始节点列表。每个元素都应该可以是表示单个起始节点的整数。
            end_nodes (list of ints): 路径搜索的结束节点列表。每个元素都应该。
            method (str): 用于寻路的算法。有效选项为“Dijkstra”，“A*”和“贝尔曼·福特”。默认为“Dijkstra”。
            cutoff (float or int): 要搜索的最大距离。路径被丢弃如果其总重量超过该值。默认值为无穷大。
            weight_name (str): 用作寻路权重的边属性的名称算法。默认值为“无”。
            num_thread (int): 用于并行计算的线程数。默认值为1。必须是整数。

        Raises:
            ValueError: 如果任何输入参数的类型或值无效：<br>
                -“start_nodes”必须是整数列表。<br>
                -“end_nodes”必须是整数列表。<br>
                -“method”必须是“Dijkstra”、“A*”或“Bellman Ford”之一。<br>
                -“cutoff”必须是非负数。<br>
                -“weight_name”必须是字符串。<br>
                -“num_thread”必须是整数。

        Returns:
            numpy.ndarray: NumPy数组，其中每个元素表示最短路径（作为节点列表）例如：<br>
                [1,3,[2,3],<br>
                1,4,[2,3,4]]
        """
        # 检查 start_nodes 是否是一个列表
        if not isinstance(start_nodes, list):
            raise ValueError(f"Invalid value for 'start_nodes': {start_nodes}. It must be a list.")

        # 检查 end_nodes 是否是一个列表
        if not isinstance(end_nodes, list):
            raise ValueError(f"Invalid value for 'end_nodes': {end_nodes}. It must be a list.")

        # 检查 method 是否是有效的字符串
        valid_methods = ["Dijkstra", "A*", "Bellman-Ford"]  # 你可以根据实际情况修改
        if method not in valid_methods:
            raise ValueError(f"Invalid value for 'method': {method}. It must be one of {valid_methods}.")

        # 检查 cutoff 是否是一个非负浮动数
        if not isinstance(cutoff, (int, float)) or cutoff < 0:
            raise ValueError(f"Invalid value for 'cutoff': {cutoff}. It must be a non-negative number.")

        # 检查 weight_name 是否是一个字符串
        if not isinstance(weight_name, str):
            raise ValueError(f"Invalid value for 'weight_name': {weight_name}. It must be a string.")

        # 检查 num_thread 是否是一个整数
        if not isinstance(num_thread, int):
            raise ValueError(f"Invalid value for 'num_thread': {num_thread}. It must be a integer.")

        result = self.graph.path_list_to_numpy(start_nodes, end_nodes, method, cutoff, weight_name, num_thread)
        return result
