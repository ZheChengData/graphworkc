# CGraph

## CGraph类初始化

Parameters:

| Name             | Type                                          | Description                                                  | Default |
| ---------------- | --------------------------------------------- | ------------------------------------------------------------ | ------- |
| m_nNode          | int                                           | 节点数量                                                     | 0       |
| m_nLink          | int                                           | 路段数量                                                     | 0       |
| m_Node           | vector                                        | 路网节点存储列表                                             | None    |
| m_Link           | vector                                        | 路网路段存储列表                                             | None    |
| LinkIndex        | unordered_map<pair<int, int>, int, pair_hash> | 起点和终点到边ID的映射表                                     | None    |
| ID2Index_map     | unordered_map<int, size_t>                    | 节点ID与对应节点的列表索引的映射表                           | None    |
| ID2Index_linkmap | unordered_map<int, size_t>                    | 路段ID与对应路段的列表索引的映射表                           | None    |
| dic_cost         | unordered_map<int, double>                    | 临时存储当前最短路径算法中的最短路径花费                     | None    |
| dic_path         | unordered_map<int, vector<int>>               | 临时存储当前最短路径算法中的最短路径列表                     | None    |
| m_path_result    | unordered_map < int, path_result>             | 存储所有计算得到的单源最短路径结果：键为 ：源节点 值为 ：最短路径花费和最短路径列表 | None    |

## CNetwork类方法



### 基本操作：

#### add_edge：

- 加入一条有向边

Parameters:

| Name       | Type | Descriptio       |
| ---------- | ---- | ---------------- |
| start_node | int  | 起点ID           |
| end_node   | int  | 终点ID           |
| attribute  | dict | 字典，存有边属性 |



#### add_edges：

- 加入多条有向边

Parameters：

| Name      | Type                                            | Description                                                  |
| --------- | ----------------------------------------------- | ------------------------------------------------------------ |
| tupleList | list[ tuple( int, int, dict{{"weight":double} ] | 列表：里面的元素是三元元组：（起点，终点, 字典(内有权重) }），必须参数 |



#### remove_edge：

- 删除一条路段

Parameters：

| Name  | Type | Description      |
| ----- | ---- | ---------------- |
| start | int  | 待删除路段的起点 |
| end   | int  | 待删除路段的终点 |



#### remove_edges：

- 删除多条路段

Parameters：

| Name  | Type                      | Description                                |
| ----- | ------------------------- | ------------------------------------------ |
| edges | list[ tuple(start, end) ] | 列表：元素为包含了路段起点和终点的二元元组 |



#### clear_all：

-  清空重置网络

Parameters:

None（直接使用即可）



### 计算方法：

#### multi_source_path：

-  多源最短路径算法 仅返回OD对的路径

Parameters:

| Name        | Type      | Description                                                 |
| ----------- | --------- | ----------------------------------------------------------- |
| start_nodes | list[int] | 源节点列表，必须参数                                        |
| method      | string    | 计算最短路径使用的方法，默认使用Dijkstra                    |
| cutoff_name | string    | 搜索半径所需的权重对应的字段名称, 如无输入，默认搜索半径为1 |

Returns：

| type                | Description                    |
| ------------------- | ------------------------------ |
| dict{int:list[int]} | 哈希表：一个终点ID对应一个路径 |



#### multi_source_cost：

-  多源最短路径算法 仅返回OD对的花费

Parameters:

| Name        | Type      | Description                                                 |
| ----------- | --------- | ----------------------------------------------------------- |
| start_nodes | list[int] | 源节点，必须参数                                            |
| method      | string    | 计算最短路径使用的方法，默认使用Dijkstra                    |
| cutoff_name | string    | 搜索半径所需的权重对应的字段名称, 如无输入，默认搜索半径为1 |

Returns：

| type             | Description                    |
| ---------------- | ------------------------------ |
| dist{int:double} | 哈希表：一个终点ID对应一个花费 |



#### multi_source_all：

-  多源最短路径算法 返回OD对的花费和路径

Parameters:

| Name        | Type      | Description                                                 |
| ----------- | --------- | ----------------------------------------------------------- |
| start_nodes | list[int] | 源节点，必须参数                                            |
| method      | string    | 计算最短路径使用的方法，默认使用Dijkstra                    |
| cutoff_name | string    | 搜索半径所需的权重对应的字段名称, 如无输入，默认搜索半径为1 |

Returns：

| type                                         | Description                                                  |
| -------------------------------------------- | ------------------------------------------------------------ |
| tuple(dict{int:list[int]}, dist{int:double}) | 多源节点的最短路径计算的花费和路径；一个元组包含两个元素：到所有节点的花费、到所有节点的路径； |





#### single_source_path：

-  单源最短路径算法 仅返回路径

Parameters:

| Name        | Type   | Description                                                 |
| ----------- | ------ | ----------------------------------------------------------- |
| start       | int    | 源节点，必须参数                                            |
| method      | string | 计算最短路径使用的方法，默认使用Dijkstra                    |
| cutoff_name | string | 搜索半径所需的权重对应的字段名称, 如无输入，默认搜索半径为1 |

Returns：

| type                | Description                    |
| ------------------- | ------------------------------ |
| dict{int:list[int]} | 哈希表：一个终点ID对应一个路径 |



#### single_source_cost：

-  单源最短路径算法 仅返回花费

Parameters:

| Name        | Type   | Description                                                 |
| ----------- | ------ | ----------------------------------------------------------- |
| start       | int    | 源节点，必须参数                                            |
| method      | string | 计算最短路径使用的方法，默认使用Dijkstra                    |
| cutoff_name | string | 搜索半径所需的权重对应的字段名称, 如无输入，默认搜索半径为1 |

Returns：

| type             | Description                    |
| ---------------- | ------------------------------ |
| dist{int:double} | 哈希表：一个终点ID对应一个花费 |



#### single_source_all：

-  单源最短路径算法 返回花费和路径

Parameters:

| Name        | Type   | Description                                                 |
| ----------- | ------ | ----------------------------------------------------------- |
| start       | int    | 源节点，必须参数                                            |
| method      | string | 计算最短路径使用的方法，默认使用Dijkstra                    |
| cutoff_name | string | 搜索半径所需的权重对应的字段名称, 如无输入，默认搜索半径为1 |

Returns：

| type                                         | Description                                                  |
| -------------------------------------------- | ------------------------------------------------------------ |
| tuple(dict{int:list[int]}, dist{int:double}) | 单源节点的最短路径计算的花费和路径；一个元组包含两个元素：到所有节点的花费、到所有节点的路径； |



#### multi_single_source_path：

-  多个单源最短路径算法 仅返回路径

Parameters:

| Name        | Type      | Description                                                 |
| ----------- | --------- | ----------------------------------------------------------- |
| StartNodes  | list[int] | 列表：元素为节点ID，代表多源节点                            |
| method      | string    | 计算最短路径使用的方法，默认使用Dijkstra                    |
| cutoff_name | string    | 搜索半径所需的权重对应的字段名称, 如无输入，默认搜索半径为1 |

Returns：

| type                              | Description                                                  |
| --------------------------------- | ------------------------------------------------------------ |
| dict{int : {dict{int:list[int]}}} | 二维哈希表：一个源节点ID对应一个一维哈希表； 一维哈希表内存有源节点的路径返回结果（与单源算法返回的结果结构相同） |



#### multi_single_source_cost：

-  多个单源最短路径算法 仅返回花费

Parameters:

| Name        | Type      | Description                                                 |
| ----------- | --------- | ----------------------------------------------------------- |
| StartNodes  | list[int] | 列表：元素为节点ID，代表多源节点                            |
| method      | string    | 计算最短路径使用的方法，默认使用Dijkstra                    |
| cutoff_name | string    | 搜索半径所需的权重对应的字段名称, 如无输入，默认搜索半径为1 |

Returns：

| type                         | Description                                                  |
| ---------------------------- | ------------------------------------------------------------ |
| dict{int:{dist{int:double}}} | 二维哈希表：一个源节点ID对应一个一维哈希表； 一维哈希表内存有源节点的花费返回结果（与单源算法返回的结果结构相同） |



#### multi_single_source_all：

-  多个单源最短路径算法 返回路径和花费

Parameters:

| Name        | Type      | Description                                                 |
| ----------- | --------- | ----------------------------------------------------------- |
| StartNodes  | list[int] | 列表：元素为节点ID，代表多源节点                            |
| method      | string    | 计算最短路径使用的方法，默认使用Dijkstra                    |
| cutoff_name | string    | 搜索半径所需的权重对应的字段名称, 如无输入，默认搜索半径为1 |

Returns：

| type                                                         | Description                                                  |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| tuple(dict{int : {dict{int:list[int]}}}, dict{int:{dist{int:double}}}) | 元组：<br />序列0：二维哈希表：一个源节点ID对应一个一维哈希表； 一维哈希表内存有对应源节点的路径返回结果（与单源算法返回的结果结构相同）<br />序列1：二维哈希表：一个源节点ID对应一个一维哈希表； 一维哈希表内存有源节点的花费返回结果（与单源算法返回的结果结构相同） |



#### multi_multi_source_path：

-  多个多源最短路径算法 仅返回路径

Parameters:

| Name        | Type            | Description                                                 |
| ----------- | --------------- | ----------------------------------------------------------- |
| StartNodes  | list[list[int]] | 二维列表：[元素为每个多源节点列表：[元素为源节点ID]]        |
| method      | string          | 计算最短路径使用的方法，默认使用Dijkstra                    |
| cutoff_name | string          | 搜索半径所需的权重对应的字段名称, 如无输入，默认搜索半径为1 |

Returns：

| type                             | Description                                                  |
| -------------------------------- | ------------------------------------------------------------ |
| dict{int: {dict{int:list[int]}}} | 二维哈希表：一个 **StartNodes列表序列** 对应一个一维哈希表； 一维哈希表内存有对应源节点的路径返回结果（与单源算法返回的结果结构相同） |



#### multi_multi_source_cost：

-  多个多源最短路径算法 仅返回花费

Parameters:

| Name        | Type            | Description                                                 |
| ----------- | --------------- | ----------------------------------------------------------- |
| StartNodes  | list[list[int]] | 二维列表：[元素为每个多源节点列表：[元素为源节点ID]]        |
| method      | string          | 计算最短路径使用的方法，默认使用Dijkstra                    |
| cutoff_name | string          | 搜索半径所需的权重对应的字段名称, 如无输入，默认搜索半径为1 |

Returns：

| type                         | Description                                                  |
| ---------------------------- | ------------------------------------------------------------ |
| dict{int:{dist{int:double}}} | 二维哈希表：一个 **StartNodes列表序列 **对应一个一维哈希表； 一维哈希表内存有对应源节点的花费返回结果（与单源算法返回的结果结构相同） |



#### multi_multi_source_all：

-  多个多源最短路径算法 返回路径和花费

Parameters:

| Name        | Type            | Description                                                 |
| ----------- | --------------- | ----------------------------------------------------------- |
| StartNodes  | list[list[int]] | 二维列表：[元素为每个多源节点列表：[元素为源节点ID]]        |
| method      | string          | 计算最短路径使用的方法，默认使用Dijkstra                    |
| cutoff_name | string          | 搜索半径所需的权重对应的字段名称, 如无输入，默认搜索半径为1 |

Returns：

| type                                                         | Description                                                  |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| tuple(dict{int : {dict{int:list[int]}}}, dict{int:{dist{int:double}}}) | 元组：<br />序列0：二维哈希表：一个 **StartNodes列表序列 ** 对应一个一维哈希表； 一维哈希表内存有对应源节点的路径返回结果（与单源算法返回的结果结构相同）<br />序列1：二维哈希表：一个 **StartNodes列表序列 ** 对应一个一维哈希表； 一维哈希表内存有对应源节点的花费返回结果（与单源算法返回的结果结构相同） |







### 数据获取：

#### cost_martix_to_numpy：

-  输出一个花费矩阵

Parameters:

| Name       | Type      | Description            |
| ---------- | --------- | ---------------------- |
| StartNodes | list[int] | 列表：元素为起始节点ID |
| EndNodes   | list[int] | 列表：元素为终点节点ID |

Returns：

| type                | Description                                                  |
| ------------------- | ------------------------------------------------------------ |
| numpy.array（m, n） | m代表了StartNodes数量，n代表了EndNodes数量，最后获得一个numpy二维数组 |



#### path_list_to_numpy：

-  输出一个路径列表

Parameters:

| Name       | Type      | Description            |
| ---------- | --------- | ---------------------- |
| StartNodes | list[int] | 列表：元素为起始节点ID |
| EndNodes   | list[int] | 列表：元素为终点节点ID |

Returns：

| type                              | Description                                          |
| --------------------------------- | ---------------------------------------------------- |
| list[startnode, endnode, [path] ] | startnode为起点，endnode为终点，[path]为OD的最短路径 |