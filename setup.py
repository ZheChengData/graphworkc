import glob
from setuptools import setup
from pybind11.setup_helpers import Pybind11Extension, build_ext
from pybind11 import get_include
import os

# 查找所有 C++ 源文件（如果有子目录，修改路径）
cpp_sources = glob.glob("src/*.cpp", recursive=True)

# 定义 Pybind11 扩展模块
ext_modules = [
    Pybind11Extension(
        "graphwork",  # C++ 扩展模块名称
        cpp_sources,  # 使用 C++ 源文件
        define_macros=[('EXAMPLE_MACRO', '1')],  # 可选参数，例如定义宏
        include_dirs=["src", get_include()],  # 确保编译器能找到头文件
    ),
]

# 获取当前目录
here = os.path.dirname(os.path.abspath(__file__))

# 确保 Python 文件被包含
py_modules = [
    "graphworkc",  # 你的 Python 模块
]

setup(
    name="graphworkc",  # 这里与模块名称一致
    version="1.1.12",
    author="ZC",
    description="A Python package with pybind11 extensions",
    long_description="This package contains a C++ extension wrapped using pybind11, providing graph-related functionality.",
    long_description_content_type="text/markdown",
    ext_modules=ext_modules,
    cmdclass={"build_ext": build_ext},
    py_modules=py_modules,  # 显式列出所有需要打包的 Python 文件
    # package_data={
    #     "": [
    #         "graphwork.so",   # 加入编译后的 C++ 扩展库
    #         "graphwork.pyd",  # Windows 环境下的 .pyd 文件
    #     ],
    # },
    include_package_data=True,  # 确保包含 `package_data`
    python_requires=">=3.9",  # 指定支持的Python版本
    install_requires=["pybind11>=2.5.0", "numpy"],  # 确保安装pybind11
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: MIT License",
        "Operating System :: OS Independent",
    ],
    package_dir={"": "src"},  # Python 文件位于 src 目录下
)
