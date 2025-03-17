cmake_minimum_required(VERSION 3.4...3.18)
project(graphwork  LANGUAGES CXX)

add_subdirectory(pybind11)
pybind11_add_module(graphwork src/CGraphBase.h src/CGraphBase.cpp src/GraphAlgorithms.h  src/GraphAlgorithms.cpp src/main.cpp)