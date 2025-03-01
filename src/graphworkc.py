import graphwork
from typing import Optional,Type


class CGraph:
    def __init__(self):
        # 创建 C++ 图算法对象
        self.graph = graphwork.GraphAlgorithms()

    def get_graph_info(self):
        """
           Retrieve information about the graph.

           This method retrieves general information about the graph, such as its
           structure, the number of nodes and edges, and any other relevant graph
           details. It assumes that the graph object has a method `get_graph_info`
           that returns such information.

           Raises:
               AttributeError: If the graph object does not have a `get_graph_info` method.

           Example:
               # Getting graph information
               graph_info = graph_instance.get_graph_info()
               print(graph_info)

           Notes:
               - This method relies on the `get_graph_info` method from the graph object
                 stored in `self.graph`.
               - The format and content of the information returned depend on the specific
                 implementation of the `get_graph_info` method in the graph object.
        """
        self.graph.get_graph_info()

    def get_node_info(self, id: int):
        """
           Retrieve information about a specific node in the graph.

           This method retrieves details about a node identified by its `id`. The
           information returned is dependent on the specific implementation of the
           `get_node_info` method in the graph object.

           Args:
               id (int): The identifier of the node. Must be an integer representing
                         the node's unique identifier in the graph.

           Raises:
               ValueError: If 'id' is not an integer.
               AttributeError: If the graph object does not have a `get_node_info` method.

           Example:
               # Getting information about node 1
               node_info = graph_instance.get_node_info(1)
               print(node_info)

           Notes:
               - This method relies on the `get_node_info` method from the graph object
                 stored in `self.graph` to retrieve the node information.
               - If the node with the given `id` does not exist in the graph, the
                 behavior depends on the implementation of `get_node_info`. It may
                 return `None`, raise an exception, or behave differently.
        """
        # 参数检查：确保 id 是有效的节点（假设它们是整数）
        if not isinstance(id, int):
            raise ValueError(f"Invalid value for 'start': {start}. It must be an integer.")

        self.graph.get_node_info(id)

    def get_link_info(self,
                      start: int,
                      end: int):
        """
            Retrieve information about a specific link (edge) in the graph.

            This method retrieves details about the link (edge) between two nodes,
            identified by the `start` and `end` nodes. The information returned depends
            on the specific implementation of the `get_link_info` method in the graph object.

            Args:
                start (int): The starting node of the edge. Must be an integer.
                end (int): The ending node of the edge. Must be an integer.

            Raises:
                ValueError: If 'start' or 'end' are not integers.
                AttributeError: If the graph object does not have a `get_link_info` method.

            Example:
                # Getting information about the link between node 1 and node 2
                link_info = graph_instance.get_link_info(1, 2)
                print(link_info)

            Notes:
                - This method relies on the `get_link_info` method from the graph object
                  stored in `self.graph` to retrieve the link information.
                - If the link (edge) between the specified nodes does not exist, the
                  behavior depends on the implementation of `get_link_info`. It may
                  return `None`, raise an exception, or behave differently.
        """
        # 参数检查：确保 start 和 end 是有效的节点（假设它们是整数）
        if not isinstance(start, int):
            raise ValueError(f"Invalid value for 'start': {start}. It must be an integer.")

        if not isinstance(end, int):
            raise ValueError(f"Invalid value for 'end': {end}. It must be an integer.")

        self.graph.get_link_info(start, end)

    def add_edge(self,
                 start: int,
                 end: int,
                 attribute_dict: dict = None,
                 planet: int = 0):
        """
        Add a single edge to the graph between two nodes with specified attributes.

        This method adds an edge between two nodes, where the start node and the
        end node are provided by the user. The edge also has attributes stored in
        a dictionary.

        Args:
            start (int): The starting node of the edge. Must be an integer or float.
            end (int): The ending node of the edge. Must be an integer or float.
            attribute_dict (dict): A dictionary containing the attributes of the edge,
                                    such as weights or other properties. Cannot be empty.
            planet (int):Mark whether the starting or ending point is a planetary point.
                0 represents that neither is a planetary point,
                1 represents that the starting point is a planetary point,
                2 represents that the ending point is a planetary point,
                3 represents that both are planetary points
        Raises:
            ValueError: If 'start' or 'end' are not integers or floats.
            ValueError: If 'attribute_dict_' is not a dictionary.

        Example:
            # Adding an edge from node 1 to node 2 with attributes
            edge_attributes = {"weight": 10, "color": "blue"}
            graph_instance.add_edge(1, 2, edge_attributes)

        Notes:
            - The method ensures that the input parameters are valid before adding the edge.
            - If any parameter is invalid, a detailed error message will be raised.
            - The edge is added to the graph, assuming `self.graph` is a valid graph object
              with an `add_edge` method.

        """
        if 1:
            # 参数检查：确保 start 和 v_ 是有效的节点（假设它们是整数）
            if not isinstance(start, int):
                raise ValueError(f"Invalid value for 'start': {start}. It must be an integer.")

            if not isinstance(end, int):
                raise ValueError(f"Invalid value for 'end': {end}. It must be an integer.")

            if not isinstance(planet, int):
                raise ValueError(f"Invalid value for 'planet': {planet}. It must be an integer.")

            # 初始化空字典
            if attribute_dict is None:
                attribute_dict = {}  # 👈 每个调用生成新字典
            if planet < 0 or planet > 3:
                raise ValueError(f"Invalid value for 'planet': {planet}. It must be an integer of 0-3.")

            # 参数类型检查
            if not isinstance(attribute_dict, dict):
                raise ValueError(f"attribute_dict必须是字典类型，当前类型：{type(attribute_dict)}")

        # 假设 self.graph 是一个已定义的图对象
        self.graph.add_edge(start, end, attribute_dict, planet)

    def add_edges(self,
                  edges: list[tuple[int, int,  Optional[dict[str, float]], Optional[int]]]):
        """
         Add multiple edges to the graph.

         Args:
             edges (list of tuple): A list of edges to be added. Each edge should be a tuple
             containing three elements:
                 - start (int): The starting node of the edge.
                 - end (int): The ending node of the edge.
                 - attribute_dict_ (dict): A dictionary containing attributes for the edge,
                   such as weights or other properties.
                 - planet (int):Mark whether the starting or ending point is a planetary point.
                    0 represents that neither is a planetary point,
                    1 represents that the starting point is a planetary point,
                    2 represents that the ending point is a planetary point,
                    3 represents that both are planetary points
         Raises:
             ValueError: If 'edges' is not a list.
             ValueError: If any element in 'edges' is not a tuple.
             ValueError: If any tuple does not have exactly 3 elements.
             ValueError: If 'start' or 'end' are not integers.
             ValueError: If 'attribute_dict_' is not a dictionary.

         Example:
             edges = [
                 (1, 2, {"weight": 5}, 0),
                 (2, 3, {"weight": 10}, 1),
                 (3, 4, {"weight": 15}, 1)
             ]
             graph_instance.add_edges(edges)

         Notes:
             This method will check the validity of the input data (type and structure) before
             adding the edges to the graph. If any validation fails, an appropriate ValueError
             will be raised to help identify the issue with the input.
         """
        if 1:
            # 确保 edges 是一个列表
            if not isinstance(edges, list):
                raise ValueError(f"Expected 'edges' to be a list, but got {type(edges)}.")

            # 遍历列表中的每个元素，确保每个元素是一个元组，并且有三个元素
            for edge in edges:
                if not isinstance(edge, tuple):
                    raise ValueError(f"Each element in 'edges' should be a tuple, but got {type(edge)}.")

                if len(edge) < 2 or len(edge) > 4:
                    raise ValueError(f"Each tuple in 'edges' should have exactly 2-4 elements, but got {len(edge)}.")

                # 检查 start 和 end 是否是有效的节点（例如整数或字符串）
                start = edge[0]
                end = edge[1]
                attribute_dict_ = {}
                is_planet_ = 0
                if len(edge) == 3:
                    attribute_dict_ = edge[2]
                if len(edge) == 4:
                    is_planet_ = edge[3]

                if is_planet_ < 0 or is_planet_ > 3:
                    raise ValueError(f"Expected 'is_planet_' to be an integer 0-3.")
                if not isinstance(start, int):
                    raise ValueError(f"Expected 'start' to be an integer, but got {type(start)}.")
                if not isinstance(end, int):
                    raise ValueError(f"Expected 'end' to be an integer, but got {type(end)}.")
                if not isinstance(is_planet_, int):
                    raise ValueError(f"Expected 'end' to be an integer, but got {type(end)}.")
                # 检查 attribute_dict_ 是否是一个字典
                if not isinstance(attribute_dict_, dict):
                    raise ValueError(f"Expected 'attribute_dict_' to be a dictionary, but got {type(attribute_dict_)}.")

        # 如果所有检查通过，调用 graph.add_edges
        self.graph.add_edges(edges)

    def remove_edge(self,
                    start: int,
                    end: int):
        """
        Remove an edge from the graph between the specified start and end nodes.

        This method removes an edge from the graph, which is defined by its starting
        and ending nodes. The edge is identified using these nodes, and the edge is
        removed if it exists in the graph.

        Args:
            start (int): The starting node of the edge. Must be an integer.
            end (int): The ending node of the edge. Must be an integer.

        Raises:
            ValueError: If 'start' or 'end' are not integers.

        Example:
            # Removing an edge between node 1 and node 2
            graph_instance.remove_edge(1, 2)

        Notes:
            - This method assumes that `self.graph` is a valid graph object with a
              `remove_edge` method that can remove an edge using the specified nodes.
            - If the edge does not exist in the graph, it will be removed without any
              error, but no change will occur to the graph.
        """
        # 参数检查：确保 start 和 v_ 是有效的节点（假设它们是整数）
        if not isinstance(start, int):
            raise ValueError(f"Invalid value for 'start': {start}. It must be an integer.")

        if not isinstance(end, int):
            raise ValueError(f"Invalid value for 'end': {end}. It must be an integer.")

        self.graph.remove_edge(start, end)

    def remove_edges(self,
                     edges: list[tuple[int, int]]):
        """
            Remove multiple edges from the graph.

            This method removes a list of edges from the graph. Each edge is specified
            as a tuple containing two elements, representing the start and end nodes.
            The edges are removed from the graph if they exist.

            Args:
                edges (list of tuples): A list of edges to remove, where each edge
                                         is represented as a tuple of two integers
                                         (start, end) indicating the nodes of the edge.

            Raises:
                ValueError: If 'edges' is not a list.
                ValueError: If any element in 'edges' is not a tuple or does not contain
                            exactly two elements.

            Example:
                # Removing multiple edges
                edges_to_remove = [(1, 2), (3, 4), (5, 6)]
                graph_instance.remove_edges(edges_to_remove)

            Notes:
                - This method assumes that `self.graph` is a valid graph object with a
                  `remove_edges` method that can remove multiple edges.
                - If any edge in the list does not exist in the graph, it will be ignored
                  and the rest of the edges will still be removed.
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
        """
        Computes the multi-source shortest paths from a set of starting nodes using a specified algorithm.

        Args:
            start_nodes (list): A list of starting nodes for the multi-source shortest path calculation.
                                Each element of the list represents a single starting node.
            method (str): The algorithm used for calculating shortest paths. It can be one of the following:
                          - "Dijkstra": Standard Dijkstra's algorithm.
                          Defaults to "Dijkstra".
            target (int): The target node for the shortest path calculation. If set to -1, it indicates no specific target.
                          Default is -1.
            cut_off (float): The maximum distance limit for the shortest path. Any path exceeding this value will be ignored.
                            Defaults to positive infinity (float('inf')).
            weight_name (str): The name of the edge weight used in the graph for the shortest path calculation.
                               Defaults to "none".

        Returns:
            dict[int: float]: A dictionary that stores the shortest path cost from the source node to each node.

        Raises:
            ValueError: If any of the following are violated:
                - 'start_nodes' is not a list.
                - 'method' is not one of the valid algorithms: "Dijkstra".
                - 'target' is not an integer.
                - 'cut_off' is not a non-negative number.
                - 'weight_name' is not a string.
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

        result = self.graph.multi_source_cost(start_nodes, method, target, cut_off, weight_name)

        return result

    def multi_source_path(self,
                          start_nodes: list[int],
                          method: str = "Dijkstra",
                          target: int = -1,
                          cut_off: float = float('inf'),
                          weight_name: str = None) -> dict[int, list[int]]:
        """
        Computes the multi-source shortest paths from a set of starting nodes to a target node using a specified algorithm.

        Args:
            start_nodes (list): A list of starting nodes for the multi-source shortest path calculation.
                                Each element represents a node from which paths are calculated.
            method (str): The algorithm used for calculating shortest paths. It can be one of the following:
                          - "Dijkstra": Standard Dijkstra's algorithm (works for graphs with non-negative weights).
            target (int): The target node to which the shortest paths are calculated. If set to -1, indicates no specific target.
                          Default is -1, which means no target.
            cut_off (float): The maximum distance limit for the shortest path calculation. Any path exceeding this value will be ignored.
                            Defaults to positive infinity (`float('inf')`).
            weight_name (str): The name of the edge weight used in the graph for the shortest path calculation.
                               Defaults to "none".

        Returns:
            dict[int: list[int]]: A dictionary that stores the shortest path sequence from the source node to each node.

        Raises:
            ValueError: If any of the following are violated:
                - 'start_nodes' is not a list.
                - 'method' is not one of the valid algorithms: "Dijkstra"
                - 'target' is not an integer.
                - 'cut_off' is not a non-negative number.
                - 'weight_name' is not a string.
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

        result = self.graph.multi_source_path(start_nodes, method, target, cut_off, weight_name)

        return result

    def multi_source_all(self,
                         start_nodes: list[int],
                         method: str = "Dijkstra",
                         target: int = -1,
                         cut_off: float = float('inf'),
                         weight_name: str = None) -> dict:
        """
        Computes the shortest paths from multiple source nodes to a target node using a specified
        pathfinding algorithm and returns the results.

        Args:
            start_nodes (list of ints): A list of starting nodes for the path search. Each element
                                         should be an integer representing a start node.
            method (str): The algorithm to use for pathfinding. Valid options are "Dijkstra".
                            Defaults to "Dijkstra".
            target (int): The target node to which all paths from the source nodes are calculated.
                          If set to -1, the target is ignored. Defaults to -1.
            cut_off (float or int): The maximum distance to search for. A path is discarded
                                   if its total weight exceeds this value. Default is infinity.
            weight_name (str): The name of the edge attribute to use as weights for the pathfinding
                               algorithm. Default is "none".

        Raises:
            ValueError: If any of the input parameters have invalid types or values:
                - 'start_nodes' must be a list of integers.
                - 'method' must be one of "Dijkstra".
                - 'target' must be an integer.
                - 'cut_off' must be a non-negative number.
                - 'weight_name' must be a string.

        Returns:
            dis_and_path[cost:dict[int:float],
                         path:dict[int: list[int]]]: A custom structure containing two attributes,
                          cost and paths, which respectively store the shortest path cost and shortest path sequence
                          from multiple source nodes to each node.
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

        result = self.graph.multi_source_all(start_nodes, method, target, cut_off, weight_name)

        return result

    def single_source_cost(self,
                          start: int,
                          method: str = "Dijkstra",
                          target: int = -1,
                          cut_off: float = float('inf'),
                          weight_name: str = None) -> dict[int, float]:
        """
        Computes the shortest paths from a set of starting nodes to all other nodes using a specified algorithm.

        Args:
            start (int): A starting node for the multi-source shortest path calculation.
                                Each element represents a starting node from which the paths are calculated.
            method (str): The algorithm used for calculating the shortest paths. It can be one of the following:
                          - "Dijkstra": Standard Dijkstra's algorithm (works for graphs with non-negative weights).
            target (int): The target node to which the shortest paths are calculated. If set to -1, indicates no specific target.
                          Default is -1, which means no target.
            cut_off (float): The maximum distance limit for the shortest path calculation. Any path exceeding this value will be ignored.
                            Defaults to positive infinity (`float('inf')`).
            weight_name (str): The name of the edge weight used in the graph for the shortest path calculation.
                               Defaults to "none".

        Returns:
            dict[int:float]: A dictionary that stores the shortest path cost from the source node to each node.

        Raises:
            ValueError: If any of the following are violated:
                - 'start_nodes' is not int.
                - 'method' is not one of the valid algorithms: "Dijkstra".
                - 'target' is not an integer.
                - 'cut_off' is not a non-negative number.
                - 'weight_name' is not a string.
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

        result = self.graph.single_source_cost(start, method, target, cut_off, weight_name)

        return result

    def single_source_path(self,
                           start: int,
                           method: str = "Dijkstra",
                           target: int = -1,
                           cut_off: float = float('inf'),
                           weight_name: str = None) -> dict[int, list[int]]:
        """
        Computes the shortest path(s) from a given start node to a target node using
        a specified algorithm.

        Args:
            start (int): The starting node for the path search. Must be an integer.
            method (str): The algorithm to use for pathfinding. Valid options are "Dijkstra".
                            Defaults to "Dijkstra".
            target (int): The target node to reach. If -1, finds paths to all nodes.
                          Must be a non-negative integer.
            cut_off (float or int): The maximum distance to search for. A path is discarded
                                   if its total weight exceeds this value. Default is infinity.
            weight_name (str): The name of the edge attribute to use as weights for the pathfinding
                               algorithm. Default is "none".

        Raises:
            ValueError: If any of the input parameters have invalid types or values:
                - 'start' must be an integer.
                - 'method' must be one of "Dijkstra".
                - 'target' must be a non-negative integer.
                - 'cut_off' must be a non-negative number.
                - 'weight_name' must be a string.

        Returns:
            dict[int: list[int]]: A dictionary that stores the shortest path sequence from the source node to each node.
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

        result = self.graph.single_source_path(start, method, target, cut_off, weight_name)

        return result

    def single_source_all(self,
                          start: int,
                          method: str = "Dijkstra",
                          target: int = -1,
                          cut_off: float = float('inf'),
                          weight_name: str = None) -> dict:
        """
        Computes the shortest path(s) from a given start node to all other nodes using
        a specified algorithm.

        Args:
            start (int): The starting node for the path search. Must be an integer.
            method (str): The algorithm to use for pathfinding. Valid options are "Dijkstra". Defaults to "Dijkstra".
            target (int): The target node to reach. If -1, finds paths to all nodes.
                          Must be a non-negative integer.
            cut_off (float or int): The maximum distance to search for. A path is discarded
                                   if its total weight exceeds this value. Default is infinity.
            weight_name (str): The name of the edge attribute to use as weights for the pathfinding
                               algorithm. Default is "none".

        Raises:
            ValueError: If any of the input parameters have invalid types or values:
                - 'start' must be an integer.
                - 'method' must be one of "Dijkstra".
                - 'target' must be a non-negative integer.
                - 'cut_off' must be a non-negative number.
                - 'weight_name' must be a string.

        Returns:
            dis_and_path[cost:dict[int:float],
                         path:dict[int: list[int]]]: A custom structure containing two attributes,
                          cost and paths, which respectively store the shortest path cost and shortest path sequence
                          from multiple source nodes to each node.
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
            result = self.graph.single_source_all(start, method, target, cut_off)
        else:
            result = self.graph.single_source_all(start, method, target, cut_off, weight_name)

        return result

    def multi_single_source_cost(self,
                         start_nodes: list[int],
                         method: str = "Dijkstra",
                         target: int = -1,
                         cut_off: float = float('inf'),
                         weight_name: str = None,
                         num_thread: int = 1) -> list[dict[int, float]]:
        """
        Computes the shortest path(s) from multiple start nodes to all other nodes
        using a specified algorithm, and returns the computed costs.

        Args:
            start_nodes (list): A list of starting nodes for the path search. Each item
                                 in the list must be an integer.
            method (str): The algorithm to use for pathfinding. Valid options are "Dijkstra". Defaults to "Dijkstra".
            target (int): The target node to reach. If -1, finds paths to all nodes.
                          Must be a non-negative integer.
            cut_off (float or int): The maximum distance to search for. A path is discarded
                                   if its total weight exceeds this value. Default is infinity.
            weight_name (str): The name of the edge attribute to use as weights for the pathfinding
                               algorithm. Default is "none".
            num_thread (int): The number of threads to use for parallel computation. Default is 1.
                              Must be an integer.

        Raises:
            ValueError: If any of the input parameters have invalid types or values:
                - 'start_nodes' must be a list of integers.
                - 'method' must be one of "Dijkstra".
                - 'target' must be a non-negative integer.
                - 'cut_off' must be a non-negative number.
                - 'weight_name' must be a string.
                - 'num_thread' must be an integer.

        Returns:
            list[dist[int: float]]: List, elements are dictionaries: each dictionary
                stores the shortest path cost from the source node to each node.
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
        result = self.graph.multi_single_source_cost(start_nodes, method, target, cut_off, weight_name, num_thread)

        return result

    def multi_single_source_path(self,
                         start_nodes: list[int],
                         method: str = "Dijkstra",
                         target: int = -1,
                         cut_off: float = float('inf'),
                         weight_name: str = None,
                         num_thread: int = 1) -> list[dict[int, list[int]]]:
        """
         Computes the shortest path(s) from multiple start nodes to all other nodes
         using a specified algorithm, and returns the computed paths.

         Args:
             start_nodes (list): A list of starting nodes for the path search. Each item
                                  in the list must be an integer.
             method (str): The algorithm to use for pathfinding. Valid options are "Dijkstra". Defaults to "Dijkstra".
             target (int): The target node to reach. If -1, finds paths to all nodes.
                           Must be a non-negative integer.
             cut_off (float or int): The maximum distance to search for. A path is discarded
                                    if its total weight exceeds this value. Default is infinity.
             weight_name (str): The name of the edge attribute to use as weights for the pathfinding
                                algorithm. Default is "none".
             num_thread (int): The number of threads to use for parallel computation. Default is 1.
                               Must be an integer.

         Raises:
             ValueError: If any of the input parameters have invalid types or values:
                 - 'start_nodes' must be a list of integers.
                 - 'method' must be one of "Dijkstra".
                 - 'target' must be a non-negative integer.
                 - 'cut_off' must be a non-negative number.
                 - 'weight_name' must be a string.
                 - 'num_thread' must be an integer.

         Returns:
             list[dist[int: list[int]]]: List, elements are dictionaries: each dictionary
                stores the shortest path sequence from the source node to each node.
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

        result = self.graph.multi_single_source_path(start_nodes, method, target, cut_off, weight_name, num_thread)
        return result

    def multi_single_source_all(self,
                         start_nodes: list[int],
                         method: str = "Dijkstra",
                         target: int = -1,
                         cut_off: float = float('inf'),
                         weight_name: str = None,
                         num_thread: int = 1) -> dict:
        """
           Computes the shortest paths from multiple start nodes to all other nodes
           using a specified algorithm, and returns the computed paths for all nodes.

           Args:
               start_nodes (list): A list of starting nodes for the path search. Each item
                                    in the list must be an integer.
               method (str): The algorithm to use for pathfinding. Valid options are "Dijkstra". Defaults to "Dijkstra".
               target (int): The target node to reach. If -1, finds paths to all nodes.
                             Must be a non-negative integer.
               cut_off (float or int): The maximum distance to search for. A path is discarded
                                      if its total weight exceeds this value. Default is infinity.
               weight_name (str): The name of the edge attribute to use as weights for the pathfinding
                                  algorithm. Default is "none".
               num_thread (int): The number of threads to use for parallel computation. Default is 1.
                                 Must be an integer.

           Raises:
               ValueError: If any of the input parameters have invalid types or values:
                   - 'start_nodes' must be a list of integers.
                   - 'method' must be one of "Dijkstra".
                   - 'target' must be a non-negative integer.
                   - 'cut_off' must be a non-negative number.
                   - 'weight_name' must be a string.
                   - 'num_thread' must be an integer.

           Returns:
               list[dis_and_path[cost: dist[int: float],
                    path: dist[int: list[int]]]]: List: The element is a custom structure: each structure contains
                     two attributes, cost and paths, which respectively store the shortest path cost and shortest path
                      sequence from multiple source nodes to each node.
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

        result = self.graph.multi_single_source_all(start_nodes, method, target, cut_off, weight_name, num_thread)
        return result

    def multi_multi_source_cost(self,
                         start_nodes: list[list[int]],
                         method: str = "Dijkstra",
                         target: int = -1,
                         cut_off: float = float('inf'),
                         weight_name: str = None,
                         num_thread: int = 1) -> list[dict[int, float]]:
        """
        Computes the cost of the shortest path(s) from multiple start node sets to all other nodes
        using a specified algorithm, and returns the computed costs for each start node set.

        Args:
            start_nodes (list of lists): A list where each item is a list of starting nodes for the
                                         path search. Each inner list should contain integers representing
                                         multiple start nodes.
            method (str): The algorithm to use for pathfinding. Valid options are "Dijkstra". Defaults to "Dijkstra".
            target (int): The target node to reach. If -1, finds paths to all nodes.
                          Must be a non-negative integer.
            cut_off (float or int): The maximum distance to search for. A path is discarded
                                   if its total weight exceeds this value. Default is infinity.
            weight_name (str): The name of the edge attribute to use as weights for the pathfinding
                               algorithm. Default is "none".
            num_thread (int): The number of threads to use for parallel computation. Default is 1.
                              Must be an integer.

        Raises:
            ValueError: If any of the input parameters have invalid types or values:
                - 'start_nodes' must be a list of lists of integers.
                - 'method' must be one of "Dijkstra".
                - 'target' must be a non-negative integer.
                - 'cut_off' must be a non-negative number.
                - 'weight_name' must be a string.
                - 'num_thread' must be an integer.

        Returns:
            list[dist[int: float]]: List, elements are dictionaries: each dictionary
                stores the shortest path cost from the source node to each node.
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

        result = self.graph.multi_multi_source_cost(start_nodes, method, target, cut_off, weight_name, num_thread)
        return result

    def multi_multi_source_path(self,
                         start_nodes: list[list[int]],
                         method: str = "Dijkstra",
                         target: int = -1,
                         cut_off: float = float('inf'),
                         weight_name: str = None,
                         num_thread: int = 1) -> list[dict[int, list[int]]]:
        """
        Computes the shortest paths from multiple sets of start nodes to all other nodes
        using a specified algorithm, and returns the computed paths for each start node set.

        Args:
            start_nodes (list of lists): A list where each item is a list of starting nodes for the
                                         path search. Each inner list should contain integers representing
                                         multiple start nodes.
            method (str): The algorithm to use for pathfinding. Valid options are "Dijkstra". Defaults to "Dijkstra".
            target (int): The target node to reach. If -1, finds paths to all nodes.
                          Must be a non-negative integer.
            cut_off (float or int): The maximum distance to search for. A path is discarded
                                   if its total weight exceeds this value. Default is infinity.
            weight_name (str): The name of the edge attribute to use as weights for the pathfinding
                               algorithm. Default is "none".
            num_thread (int): The number of threads to use for parallel computation. Default is 1.
                              Must be an integer.

        Raises:
            ValueError: If any of the input parameters have invalid types or values:
                - 'start_nodes' must be a list of lists of integers.
                - 'method' must be one of "Dijkstra".
                - 'target' must be a non-negative integer.
                - 'cut_off' must be a non-negative number.
                - 'weight_name' must be a string.
                - 'num_thread' must be an integer.

        Returns:
            list[dist[int: list[int]]]: List, elements are dictionaries: each dictionary
                stores the shortest path sequence from the source node to each node.
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

        result = self.graph.multi_multi_source_path(start_nodes, method, target, cut_off, weight_name, num_thread)
        return result

    def multi_multi_source_all(self,
                         start_nodes: list[list[int]],
                         method: str = "Dijkstra",
                         target: int = -1,
                         cut_off: float = float('inf'),
                         weight_name: str = None,
                         num_thread: int = 1) -> dict:
        """
        Computes the shortest paths from multiple sets of start nodes to all other nodes using a specified
        algorithm, and returns the computed paths for all start node sets.

        Args:
            start_nodes (list of lists): A list where each item is a list of starting nodes for the
                                         path search. Each inner list should contain integers representing
                                         multiple start nodes.
            method (str): The algorithm to use for pathfinding. Valid options are "Dijkstra". Defaults to "Dijkstra".
            target (int): The target node to reach. If -1, finds paths to all nodes.
                          Must be a non-negative integer.
            cut_off (float or int): The maximum distance to search for. A path is discarded
                                   if its total weight exceeds this value. Default is infinity.
            weight_name (str): The name of the edge attribute to use as weights for the pathfinding
                               algorithm. Default is "none".
            num_thread (int): The number of threads to use for parallel computation. Default is 1.
                              Must be an integer.

        Raises:
            ValueError: If any of the input parameters have invalid types or values:
                - 'start_nodes' must be a list of lists of integers.
                - 'method' must be one of "Dijkstra".
                - 'target' must be a non-negative integer.
                - 'cut_off' must be a non-negative number.
                - 'weight_name' must be a string.
                - 'num_thread' must be an integer.

        Returns:
            list[dis_and_path[cost: dist[int: float],
                    path: dist[int: list[int]]]]: List: The element is a custom structure: each structure contains
                     two attributes, cost and paths, which respectively store the shortest path cost and shortest path
                      sequence from multiple source nodes to each node.
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

        result = self.graph.multi_multi_source_all(start_nodes, method, target, cut_off, weight_name, num_thread)
        return result

    def cost_matrix_to_numpy(self,
                             start_nodes: list[list[int]],
                             end_nodes: list[list[int]],
                             method: str = "Dijkstra",
                             cut_off: float = float('inf'),
                             weight_name: str = None,
                             num_thread: int = 1):
        """
            Computes the cost matrix (shortest path costs) between multiple pairs of start and end nodes using
            a specified pathfinding algorithm, and returns the result as a NumPy array.

            Args:
                start_nodes (list of ints): A list of starting nodes for the path search. Each element should
                                             be an integer representing a single start node.
                end_nodes (list of ints): A list of ending nodes for the path search. Each element should
                                          be an integer representing a single end node.
                method (str): The algorithm to use for pathfinding. Valid options are "Dijkstra". Defaults to "Dijkstra".
                cut_off (float or int): The maximum distance to search for. A path is discarded
                                       if its total weight exceeds this value. Default is infinity.
                weight_name (str): The name of the edge attribute to use as weights for the pathfinding
                                   algorithm. Default is "none".
                num_thread (int): The number of threads to use for parallel computation. Default is 1.
                                  Must be an integer.

            Raises:
                ValueError: If any of the input parameters have invalid types or values:
                    - 'start_nodes' must be a list of integers.
                    - 'end_nodes' must be a list of integers.
                    - 'method' must be one of "Dijkstra".
                    - 'cut_off' must be a non-negative number.
                    - 'weight_name' must be a string.
                    - 'num_thread' must be an integer.

            Returns:
                numpy.ndarray: A NumPy array where each element represents the shortest path cost
                               between a start node and an end node, based on the specified algorithm.
                               The dimensions of the array will be len(start_nodes) x len(end_nodes).
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

        result = self.graph.cost_matrix_to_numpy(start_nodes, end_nodes, method, cut_off, weight_name, num_thread)
        return result

    def path_list_to_numpy(self,
                           start_nodes: list[list[int]],
                           end_nodes: list[list[int]],
                           method: str = "Dijkstra",
                           cut_off: float = float('inf'),
                           weight_name: str = None,
                           num_thread: int = 1) -> dict[tuple[int, int], list[int]]:
        """
            Computes the path list (shortest paths) between multiple pairs of start and end nodes using
            a specified pathfinding algorithm, and returns the result as a NumPy array.

            Args:
                start_nodes (list of ints): A list of starting nodes for the path search. Each element should
                                             be an integer representing a single start node.
                end_nodes (list of ints): A list of ending nodes for the path search. Each element should
                                          be an integer representing a single end node.
                method (str): The algorithm to use for pathfinding. Valid options are "Dijkstra". Defaults to "Dijkstra".
                cut_off (float or int): The maximum distance to search for. A path is discarded
                                       if its total weight exceeds this value. Default is infinity.
                weight_name (str): The name of the edge attribute to use as weights for the pathfinding
                                   algorithm. Default is "none".
                num_thread (int): The number of threads to use for parallel computation. Default is 1.
                                  Must be an integer.

            Raises:
                ValueError: If any of the input parameters have invalid types or values:
                    - 'start_nodes' must be a list of integers.
                    - 'end_nodes' must be a list of integers.
                    - 'method' must be one of "Dijkstra".
                    - 'cut_off' must be a non-negative number.
                    - 'weight_name' must be a string.
                    - 'num_thread' must be an integer.

            Returns:
                numpy.ndarray: A NumPy array where each element represents the shortest path (as a list of nodes)
                               between a start node and an end node, based on the specified algorithm.
                               The dimensions of the array will be len(start_nodes) x len(end_nodes), and each
                               element will contain the node sequence representing the path.
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

        result = self.graph.path_list_to_numpy(start_nodes, end_nodes, method, cut_off, weight_name, num_thread)
        return result
