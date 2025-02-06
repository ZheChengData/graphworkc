# CNetwork

## CNetwork类初始化

Parameters:

| Name             | Type                                | Description                                                  | Default |
| ---------------- | ----------------------------------- | ------------------------------------------------------------ | ------- |
| m_nNode          | int                                 | 节点数量                                                     | 0       |
| m_nLink          | int                                 | 路段数量                                                     | 0       |
| m_Node           | vector                              | 路网节点存储列表                                             | None    |
| m_Link           | vector                              | 路网路段存储列表                                             | None    |
| LinkIndex        | std::unordered_map, int, pair_hash> | 起点和终点到边ID的映射表                                     | None    |
| ID2Index_map     | unordered_map                       | 节点ID与对应节点的列表索引的映射表                           | None    |
| ID2Index_linkmap | unordered_map                       | 路段ID与对应路段的列表索引的映射表                           | None    |
| dic_cost         | unordered_map                       | 临时存储当前最短路径算法中的最短路径花费                     | None    |
| dic_path         | unordered_map>                      | 临时存储当前最短路径算法中的最短路径列表                     | None    |
| m_path_result    | unordered_map < int, path_result>   | 存储所有计算得到的单源最短路径结果：键为 ：源节点 值为 ：最短路径花费和最短路径列表 | None    |



### CNetwork类方法 - AddEdgeFromTuple：

- 加入一条路段

Parameters:

| Name        | Type                                      | Description                                          |
| ----------- | ----------------------------------------- | ---------------------------------------------------- |
| input_tuple | tuple( int, int, dict{{"weight":double} ) | 三元元组：（起点，终点, 字典(内有权重) }），必须参数 |



### CNetwork类方法 - AddEdgesFromList：

- 加入多条路段

Parameters：

| Name      | Type                                            | Description                                                  |
| --------- | ----------------------------------------------- | ------------------------------------------------------------ |
| tupleList | list[ tuple( int, int, dict{{"weight":double} ] | 列表：里面的元素是三元元组：（起点，终点, 字典(内有权重) }），必须参数 |



CNetwork类方法 -RemoveEdge：

- 删除一条路段

Parameters：

| Name  | Type | Description      |
| ----- | ---- | ---------------- |
| start | int  | 待删除路段的起点 |
| end   | int  | 待删除路段的终点 |



### CNetwork类方法 - RemoveEdges：

- 删除多条路段

Parameters：

| Name  | Type                      | Description                                |
| ----- | ------------------------- | ------------------------------------------ |
| edges | list[ tuple(start, end) ] | 列表：元素为包含了路段起点和终点的二元元组 |



### CNetwork类方法 - SingleSourcePath：

-  单源最短路径算法

Parameters:

| Name    | Type   | Description                              |
| ------- | ------ | ---------------------------------------- |
| start   | int    | 源节点，必须参数                         |
| method  | string | 计算最短路径使用的方法，默认使用Dijkstra |
| cut_off | double | 搜索权重半径, 默认9999999                |

Returns：计算结果默认保存进m_path_result



### CNetwork类方法 - MultiSourcePath：

-  多源最短路径算法

Parameters:

| Name       | Type      | Description                              |
| ---------- | --------- | ---------------------------------------- |
| StartNodes | list[int] | 列表：元素为节点ID，代表多源节点         |
| method     | string    | 计算最短路径使用的方法，默认使用Dijkstra |
| cut_off    | double    | 搜索权重半径，默认9999999                |

Returns：计算结果默认保存进m_path_result



### CNetwork类方法 - CostMartixToCsv：

-  输出一个最短路径花费矩阵csv文件

Parameters:

| Name       | Type      | Description                   |
| ---------- | --------- | ----------------------------- |
| StartNodes | list[int] | 列表：元素为起始点节点ID      |
| EndNodes   | list[int] | 列表：元素为终点节点ID        |
| file_path  | string    | 结果保存文件路径:" ***.csv  " |



### CNetwork类方法 - CostMartixToCsv：

-  输出一个最短路径列表csv文件

Parameters:

| Name       | Type      | Description                   |
| ---------- | --------- | ----------------------------- |
| StartNodes | list[int] | 列表：元素为起始点节点ID      |
| EndNodes   | list[int] | 列表：元素为终点节点ID        |
| file_path  | string    | 结果保存文件路径:" ***.csv  " |



### CNetwork类方法 - ClearAll：

-  清空重置网络

Parameters:

None（直接使用即可）