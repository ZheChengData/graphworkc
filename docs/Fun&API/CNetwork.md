# CNetwork

## CNetwork类初始化

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

### add_edges_from_tuple：

- 加入一条路段

Parameters:

| Name        | Type                                      | Description                                          |
| ----------- | ----------------------------------------- | ---------------------------------------------------- |
| input_tuple | tuple( int, int, dict{{"weight":double} ) | 三元元组：（起点，终点, 字典(内有权重) }），必须参数 |



### add_edges_from_list：

- 加入多条路段

Parameters：

| Name      | Type                                            | Description                                                  |
| --------- | ----------------------------------------------- | ------------------------------------------------------------ |
| tupleList | list[ tuple( int, int, dict{{"weight":double} ] | 列表：里面的元素是三元元组：（起点，终点, 字典(内有权重) }），必须参数 |



### remove_edge：

- 删除一条路段

Parameters：

| Name  | Type | Description      |
| ----- | ---- | ---------------- |
| start | int  | 待删除路段的起点 |
| end   | int  | 待删除路段的终点 |



### remove_edges：

- 删除多条路段

Parameters：

| Name  | Type                      | Description                                |
| ----- | ------------------------- | ------------------------------------------ |
| edges | list[ tuple(start, end) ] | 列表：元素为包含了路段起点和终点的二元元组 |



### single_source_path：

-  单源最短路径算法 仅返回OD对的路径

Parameters:

| Name    | Type   | Description                              |
| ------- | ------ | ---------------------------------------- |
| start   | int    | 源节点，必须参数                         |
| method  | string | 计算最短路径使用的方法，默认使用Dijkstra |
| cut_off | double | 搜索权重半径, 默认9999999                |

Returns：

| type                                   | Description                    |
| -------------------------------------- | ------------------------------ |
| unordered_map<int, std::vector<int\>\> | 哈希表：一个终点ID对应一个路径 |



### single_source_cost：

-  单源最短路径算法 仅返回OD对的花费

Parameters:

| Name    | Type   | Description                              |
| ------- | ------ | ---------------------------------------- |
| start   | int    | 源节点，必须参数                         |
| method  | string | 计算最短路径使用的方法，默认使用Dijkstra |
| cut_off | double | 搜索权重半径, 默认9999999                |

Returns：

| type                        | Description                    |
| --------------------------- | ------------------------------ |
| unordered_map<int, double\> | 哈希表：一个终点ID对应一个花费 |



### single_source_all：

-  单源最短路径算法 返回OD对的花费和路径

Parameters:

| Name    | Type   | Description                              |
| ------- | ------ | ---------------------------------------- |
| start   | int    | 源节点，必须参数                         |
| method  | string | 计算最短路径使用的方法，默认使用Dijkstra |
| cut_off | double | 搜索权重半径, 默认9999999                |

Returns：

| type                                                         | Description                                                  |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| pair<unordered_map<int, double\>, unordered_map<int, std::vector<int\>\> | 单源节点的最短路径计算的花费和路径；一个元组包含两个元素：单源节点及其到所有节点的花费、所有单源节点及其到所有节点的路径； |



### multi_source_path：

-  多源最短路径算法 仅返回各源节点的OD对的路径

Parameters:

| Name       | Type      | Description                              |
| ---------- | --------- | ---------------------------------------- |
| StartNodes | list[int] | 列表：元素为节点ID，代表多源节点         |
| method     | string    | 计算最短路径使用的方法，默认使用Dijkstra |
| cut_off    | double    | 搜索权重半径，默认9999999                |

Returns：

| type                                                    | Description                                                  |
| ------------------------------------------------------- | ------------------------------------------------------------ |
| unordered_map <int, unordered_map<int, vector<int\>\>\> | 二维哈希表：一个源节点ID对应一个一维哈希表； 一维哈希表内存有对应源节点的路径返回结果（与单源算法返回的结果结构相同） |



### multi_source_cost：

-  多源最短路径算法 仅返回各源节点的OD对的花费

Parameters:

| Name       | Type      | Description                              |
| ---------- | --------- | ---------------------------------------- |
| StartNodes | list[int] | 列表：元素为节点ID，代表多源节点         |
| method     | string    | 计算最短路径使用的方法，默认使用Dijkstra |
| cut_off    | double    | 搜索权重半径，默认9999999                |

Returns：

| type                                             | Description                                                  |
| ------------------------------------------------ | ------------------------------------------------------------ |
| unordered_map<int, unordered_map<int, double\>\> | 二维哈希表：一个源节点ID对应一个一维哈希表； 一维哈希表内存有对应源节点的花费返回结果（与单源算法返回的结果结构相同） |



### multi_source_all：

-  多源最短路径算法 返回各源节点的OD对的路径和花费

Parameters:

| Name       | Type      | Description                              |
| ---------- | --------- | ---------------------------------------- |
| StartNodes | list[int] | 列表：元素为节点ID，代表多源节点         |
| method     | string    | 计算最短路径使用的方法，默认使用Dijkstra |
| cut_off    | double    | 搜索权重半径，默认9999999                |

Returns：

| type                                                         | Description                                                  |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| pair<unordered_map <int, unordered_map<int, vector<int\>\>\>, unordered_map<int, unordered_map<int, double\>\>\> | 元组：<br />序列0：二维哈希表：一个源节点ID对应一个一维哈希表； 一维哈希表内存有对应源节点的路径返回结果（与单源算法返回的结果结构相同）<br />序列1：二维哈希表：一个源节点ID对应一个一维哈希表； 一维哈希表内存有对应源节点的花费返回结果（与单源算法返回的结果结构相同） |





### CostMartixToCsv：

-  输出一个最短路径花费矩阵csv文件

Parameters:

| Name       | Type      | Description                   |
| ---------- | --------- | ----------------------------- |
| StartNodes | list[int] | 列表：元素为起始点节点ID      |
| EndNodes   | list[int] | 列表：元素为终点节点ID        |
| file_path  | string    | 结果保存文件路径:" ***.csv  " |



### CostMartixToCsv：

-  输出一个最短路径列表csv文件

Parameters:

| Name       | Type      | Description                   |
| ---------- | --------- | ----------------------------- |
| StartNodes | list[int] | 列表：元素为起始点节点ID      |
| EndNodes   | list[int] | 列表：元素为终点节点ID        |
| file_path  | string    | 结果保存文件路径:" ***.csv  " |



### ClearAll：

-  清空重置网络

Parameters:

None（直接使用即可）