# CGraph

## CGraph类初始化

Parameters:

| Name    | Type | Description | Default |
| ------- | ---- | ----------- | ------- |
| m_nNode | int  | 节点数量    | 0       |
| m_nLink | int  | 路段数量    | 0       |



## CGraph类方法

### 基本操作：

#### get_graph_info：

-  查看图信息

Parameters:

无需输入

return :
会展示当前图的一系列信息



#### get_node_info：

- 查看node信息

Parameters:

| Name | Type | Descriptio |
| ---- | ---- | ---------- |
| node | int  | 节点ID     |

return :
会展示当前节点的一系列信息



#### get_link_info：

- 查看边信息

Parameters:

| Name       | Type | Descriptio |
| ---------- | ---- | ---------- |
| start_node | int  | 起点ID     |
| end_node   | int  | 终点ID     |

return :
会展示当前边的一系列信息



#### add_edge：

- 加入一条有向边

Parameters:

| Name       | Type | Descriptio       |
| ---------- | ---- | ---------------- |
| start_node | int  | 起点ID           |
| end_node   | int  | 终点ID           |
| attribute  | dict | 字典，存储边属性 |



#### add_edges：

- 加入多条有向边

Parameters：

| Name      | Type                                            | Description                                                  |
| --------- | ----------------------------------------------- | ------------------------------------------------------------ |
| tupleList | list[ tuple( int, int, dict{{"weight":double} ] | 列表：里面的元素是三元元组：（起点，终点, 字典(内有权重) }），必须参数 |



#### remove_edge：

- 删除一条有向边

Parameters：

| Name  | Type | Description      |
| ----- | ---- | ---------------- |
| start | int  | 待删除路段的起点 |
| end   | int  | 待删除路段的终点 |



#### remove_edges：

- 删除多条有向边

Parameters：

| Name  | Type                      | Description                                |
| ----- | ------------------------- | ------------------------------------------ |
| edges | list[ tuple(start, end) ] | 列表：元素为包含了路段起点和终点的二元元组 |



### 计算方法：

#### multi_source_path：

-  多源最短路径算法 仅返回OD对的路径

Parameters:

| Name        | Type      | Description                                                  |
| ----------- | --------- | ------------------------------------------------------------ |
| list_o      | list[int] | 源节点列表，必须参数                                         |
| method      | string    | 计算最短路径使用的方法，默认使用Dijkstra，（可选）           |
| weight_name | string    | 搜索半径所需的权重对应的字段名称, 如无输入，默认搜索半径为1，（可选） |
| target      | int       | 目标节点，默认为-1，（可选）                                 |
| cutoff      | double    | 搜索长度，默认正无穷，（可选）                               |

Returns：

| type                | Description                    |
| ------------------- | ------------------------------ |
| dict{int:list[int]} | 哈希表：一个终点ID对应一个路径 |



#### multi_source_cost：

-  多源最短路径算法 仅返回OD对的花费

Parameters:

| Name        | Type      | Description                                                  |
| ----------- | --------- | ------------------------------------------------------------ |
| list_o      | list[int] | 源节点列表，必须参数                                         |
| method      | string    | 计算最短路径使用的方法，默认使用Dijkstra，（可选）           |
| weight_name | string    | 搜索半径所需的权重对应的字段名称, 如无输入，默认搜索半径为1，（可选） |
| target      | int       | 目标节点，默认为-1，（可选）                                 |
| cutoff      | double    | 搜索长度，默认正无穷，（可选）                               |

Returns：

| type             | Description                    |
| ---------------- | ------------------------------ |
| dist{int:double} | 哈希表：一个终点ID对应一个花费 |



#### multi_source_all：

-  多源最短路径算法 返回OD对的花费和路径

Parameters:

| Name        | Type      | Description                                                  |
| ----------- | --------- | ------------------------------------------------------------ |
| list_o      | list[int] | 源节点列表，必须参数                                         |
| method      | string    | 计算最短路径使用的方法，默认使用Dijkstra，（可选）           |
| weight_name | string    | 搜索半径所需的权重对应的字段名称, 如无输入，默认搜索半径为1，（可选） |
| target      | int       | 目标节点，默认为-1，（可选）                                 |
| cutoff      | double    | 搜索长度，默认正无穷，（可选）                               |

Returns：

| type                                                         | Description                                                  |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| struct result{<br />cost:dist{int:double},<br /> paths:dict{int:list[int]}<br />} | 多源最短路计算的花费和路径；<br />一个结构体包含两个元素：<br />cost：到所有节点的花费<br />paths:到所有节点的路径； |





#### single_source_path：

-  单源最短路径算法 仅返回路径

Parameters:

| Name        | Type   | Description                                                  |
| ----------- | ------ | ------------------------------------------------------------ |
| o           | lint   | 源节点，必须参数                                             |
| method      | string | 计算最短路径使用的方法，默认使用Dijkstra，（可选）           |
| weight_name | string | 搜索半径所需的权重对应的字段名称, 如无输入，默认搜索半径为1，（可选） |
| target      | int    | 目标节点，默认为-1，（可选）                                 |
| cutoff      | double | 搜索长度，默认正无穷，（可选）                               |

Returns：

| type                | Description                    |
| ------------------- | ------------------------------ |
| dict{int:list[int]} | 哈希表：一个终点ID对应一个路径 |



#### single_source_cost：

-  单源最短路径算法 仅返回花费

Parameters:

| Name        | Type   | Description                                                  |
| ----------- | ------ | ------------------------------------------------------------ |
| o           | lint   | 源节点，必须参数                                             |
| method      | string | 计算最短路径使用的方法，默认使用Dijkstra，（可选）           |
| weight_name | string | 搜索半径所需的权重对应的字段名称, 如无输入，默认搜索半径为1，（可选） |
| target      | int    | 目标节点，默认为-1，（可选）                                 |
| cutoff      | double | 搜索长度，默认正无穷，（可选）                               |

Returns：

| type             | Description                    |
| ---------------- | ------------------------------ |
| dist{int:double} | 哈希表：一个终点ID对应一个花费 |



#### single_source_all：

-  单源最短路径算法 返回花费和路径

Parameters:

| Name        | Type   | Description                                                  |
| ----------- | ------ | ------------------------------------------------------------ |
| o           | lint   | 源节点，必须参数                                             |
| method      | string | 计算最短路径使用的方法，默认使用Dijkstra，（可选）           |
| weight_name | string | 搜索半径所需的权重对应的字段名称, 如无输入，默认搜索半径为1，（可选） |
| target      | int    | 目标节点，默认为-1，（可选）                                 |
| cutoff      | double | 搜索长度，默认正无穷，（可选）                               |

Returns：

| type                                                         | Description                                                  |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| struct result{<br />dict{int:list[int]}, <br />dist{int:double}<br />} | 单源节点的最短路径计算的花费和路径；一个结构体包含两个元素：<br />cost：到所有节点的花费<br />paths:到所有节点的路径； |



#### multi_single_source_path：

-  多个单源最短路径算法 仅返回路径

Parameters:

| Name        | Type      | Description                                                  |
| ----------- | --------- | ------------------------------------------------------------ |
| list_o      | list[int] | 列表：元素为节点ID，代表多个**单源节点**                     |
| method      | string    | 计算最短路径使用的方法，默认使用Dijkstra，（可选）           |
| weight_name | string    | 搜索半径所需的权重对应的字段名称, 如无输入，默认搜索半径为1，（可选） |
| target      | int       | 目标节点，默认为-1，（可选）                                 |
| cutoff      | double    | 搜索长度，默认正无穷，（可选）                               |
| num_thread  | int       | 多线程计算核心数，默认为1,（可选）                           |

Returns：

| type                        | Description                                                  |
| --------------------------- | ------------------------------------------------------------ |
| list[{dict{int:list[int]}}] | 列表[单源节点路径结果]：列表的每一个元素都是一个单源最短路路径结果 |



#### multi_single_source_cost：

-  多个单源最短路径算法 仅返回花费

Parameters:

| Name        | Type      | Description                                                  |
| ----------- | --------- | ------------------------------------------------------------ |
| list_o      | list[int] | 列表：元素为节点ID，代表多个**单源节点**                     |
| method      | string    | 计算最短路径使用的方法，默认使用Dijkstra，（可选）           |
| weight_name | string    | 搜索半径所需的权重对应的字段名称, 如无输入，默认搜索半径为1，（可选） |
| target      | int       | 目标节点，默认为-1，（可选）                                 |
| cutoff      | double    | 搜索长度，默认正无穷，（可选）                               |
| num_thread  | int       | 多线程计算核心数，默认为1,（可选）                           |

Returns：

| type                   | Description                                                  |
| ---------------------- | ------------------------------------------------------------ |
| list[dist{int:double}] | 列表[单源节点花费结果]：列表的每一个元素都是一个单源最短路花费结果 |



#### multi_single_source_all：

-  多个单源最短路径算法 返回路径和花费

Parameters:

| Name        | Type      | Description                                                  |
| ----------- | --------- | ------------------------------------------------------------ |
| list_o      | list[int] | 列表：元素为节点ID，代表多个**单源节点**                     |
| method      | string    | 计算最短路径使用的方法，默认使用Dijkstra，（可选）           |
| weight_name | string    | 搜索半径所需的权重对应的字段名称, 如无输入，默认搜索半径为1，（可选） |
| target      | int       | 目标节点，默认为-1，（可选）                                 |
| cutoff      | double    | 搜索长度，默认正无穷，（可选）                               |
| num_thread  | int       | 多线程计算核心数，默认为1,（可选）                           |

Returns：

| type                                                         | Description                                                  |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| list[结构体result{<br />dict{int:list[int]}, <br />dist{int:double}<br />}] | 列表[单源最短路计算的花费和路径]:<br /> 每一个元素都是单源最短路的结构体：这个结构体包含两个属性：<br />cost：到所有节点的花费<br />paths:到所有节点的路径； |



#### multi_multi_source_path：

-  多个多源最短路径算法 仅返回路径

Parameters:

| Name        | Type            | Description                                                  |
| ----------- | --------------- | ------------------------------------------------------------ |
| list_o      | list[list[int]] | 二维列表：元素为list[节点ID]，代表多个**多源节点**           |
| method      | string          | 计算最短路径使用的方法，默认使用Dijkstra，（可选）           |
| weight_name | string          | 搜索半径所需的权重对应的字段名称, 如无输入，默认搜索半径为1，（可选） |
| target      | int             | 目标节点，默认为-1，（可选）                                 |
| cutoff      | double          | 搜索长度，默认正无穷，（可选）                               |
| num_thread  | int             | 多线程计算核心数，默认为1,（可选）                           |

Returns：

| type                        | Description                                                  |
| --------------------------- | ------------------------------------------------------------ |
| list[{dict{int:list[int]}}] | 列表[单源节点路径结果]：列表的每一个元素都是一个单源最短路路径结果 |



#### multi_multi_source_cost：

-  多个多源最短路径算法 仅返回花费

Parameters:

| Name        | Type            | Description                                                  |
| ----------- | --------------- | ------------------------------------------------------------ |
| list_o      | list[list[int]] | 二维列表：元素为list[节点ID]，代表多个**多源节点**           |
| method      | string          | 计算最短路径使用的方法，默认使用Dijkstra，（可选）           |
| weight_name | string          | 搜索半径所需的权重对应的字段名称, 如无输入，默认搜索半径为1，（可选） |
| target      | int             | 目标节点，默认为-1，（可选）                                 |
| cutoff      | double          | 搜索长度，默认正无穷，（可选）                               |
| num_thread  | int             | 多线程计算核心数，默认为1,（可选）                           |

Returns：

| type                   | Description                                                  |
| ---------------------- | ------------------------------------------------------------ |
| list[dist{int:double}] | 列表[单源节点花费结果]：列表的每一个元素都是一个单源最短路花费结果 |



#### multi_multi_source_all：

-  多个多源最短路径算法 返回路径和花费

Parameters:

| Name        | Type            | Description                                                  |
| ----------- | --------------- | ------------------------------------------------------------ |
| list_o      | list[list[int]] | 二维列表：元素为list[节点ID]，代表多个**多源节点**           |
| method      | string          | 计算最短路径使用的方法，默认使用Dijkstra，（可选）           |
| weight_name | string          | 搜索半径所需的权重对应的字段名称, 如无输入，默认搜索半径为1，（可选） |
| target      | int             | 目标节点，默认为-1，（可选）                                 |
| cutoff      | double          | 搜索长度，默认正无穷，（可选）                               |
| num_thread  | int             | 多线程计算核心数，默认为1,（可选）                           |

Returns：

| type                                                         | Description                                                  |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| list[结构体result{<br />dict{int:list[int]}, <br />dist{int:double}<br />}] | 列表[单源最短路计算的花费和路径]:<br /> 每一个元素都是单源最短路的结构体：这个结构体包含两个属性：<br />cost：到所有节点的花费<br />paths:到所有节点的路径； |



### 矩阵数据：

#### cost_martix_to_numpy：

-  输出一个花费矩阵

Parameters:

| Name        | Type      | Description                                                  |
| ----------- | --------- | ------------------------------------------------------------ |
| starts      | list[int] | 列表：元素为起始节点ID                                       |
| ends        | list[int] | 列表：元素为终点节点ID                                       |
| method      | string    | 计算最短路径使用的方法，默认使用Dijkstra，（可选）           |
| weight_name | string    | 搜索半径所需的权重对应的字段名称, 如无输入，默认搜索半径为1，（可选） |
| cutoff      | double    | 搜索长度，默认正无穷，（可选）                               |
| num_thread  | int       | 多线程计算核心数，默认为1,（可选）                           |

Returns：

| type                | Description                                                  |
| ------------------- | ------------------------------------------------------------ |
| numpy.array（m, n） | m代表了StartNodes数量，n代表了EndNodes数量，最后获得一个numpy二维数组，元素为最短路径花费 |



#### path_list_to_numpy：

-  输出一个路径列表

Parameters:

| Name        | Type      | Description                                                  |
| ----------- | --------- | ------------------------------------------------------------ |
| StartNodes  | list[int] | 列表：元素为起始节点ID                                       |
| EndNodes    | list[int] | 列表：元素为终点节点ID                                       |
| method      | string    | 计算最短路径使用的方法，默认使用Dijkstra，（可选）           |
| weight_name | string    | 搜索半径所需的权重对应的字段名称, 如无输入，默认搜索半径为1，（可选） |
| cutoff      | double    | 搜索长度，默认正无穷，（可选）                               |
| num_thread  | int       | 多线程计算核心数，默认为1,（可选）                           |

Returns：

| type                  | Description                                                  |
| --------------------- | ------------------------------------------------------------ |
| numpy.array（m*n, 3） | 返回一个二维numpy矩阵，第一列为起点，第二列为终点，第三列为路径 |