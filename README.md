
<div align="center">
<a href="https://gotrackit.readthedocs.io/en/latest/">
    <img src="docs/_static/logo.svg"  width="320" alt="GoTrackIt"/>
</a>
</div>


<div align=center>

![PyPI - Version](https://img.shields.io/pypi/v/graphworkc)
![GitHub License](https://img.shields.io/github/license/ZheChengData/graphworkc)




GraphWorkC是一个基于c++实现的图论分析python库，兼具高效与易用性

🔑graphworkc迭代更新很快，记得关注版本更新信息哦🔑

**❗❗❗不要下载GitHub仓库上的代码来使用!!!  直接pip安装graphworkc为第三方库即可使用❗❗❗**

😆😁👉[graphworkc用户手册](https://github.com/ZheChengData/graphworkc)👈😝😉

</div>

# 简介
**GraphWorkC** 是一个基于pybind11的高效图论分析工具，作为开源的python库，采用混合编程（C++计算与Python数据处理）方式，实现了快速的图论算法分析。项目支持多源计算、单源计算以及多个单源计算等功能，旨在提供一种高效的方式来处理和分析大规模图数据

## 主要功能

- **最短路径计算**：基于给定的 OD（Origin-Destination）对，计算并返回最短路径。
- **最少花费计算**：考虑路网中的花费信息，计算从源点到目的点的最少花费路径。
- **高效算法**：使用高效的图论算法，确保即使在大规模路网数据下也能快速计算结果。
- **灵活的配置**：支持用户根据实际需求自定义路网结构和计算模型，便于适应各种应用场景。

## 使用场景

- **交通运输**：适用于城市交通管理、道路规划等领域，帮助计算最优的行驶路线，优化交通流量。
- **网络优化**：可以用于计算信息传输的最短路径或最低成本路径，提升网络效率。
- **智能物流**：为物流公司提供路径优化方案，降低运输成本和时间。

## 特性

- **开源**：项目完全开源，任何人都可以自由查看和贡献代码。
- **跨平台**：支持多种操作系统，方便用户在不同环境下使用。
- **易于集成**：提供简单易用的 API 接口，便于与其他系统集成。

# 安装与使用

此开源库当前已上传至pypi官网，可直接pip下载使用即可

## 前提条件

- 安装Python 3.6+

pip下载(graphworkc不依赖于任何第三方库)

```
pip install graphworkc
```

# 贡献

GraphWorkC 是一个社区驱动的开源项目，我们欢迎所有开发者为项目贡献代码、报告 bug 或提出改进建议。请查阅项目的 [贡献指南](https://xio919.aitianhu1.top/CONTRIBUTING.md) 了解更多信息。



