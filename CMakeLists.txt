cmake_minimum_required(VERSION 3.11.0)
project(nctx)
set (nctx_VERSION_MAJOR 0)
set (nctx_VERSION_MINOR 1)
set (NctxPySrc "src")

SET(PYTHON_MIN_VERSION "3")
SET(BOOST_MIN_VERSION "1.65.0")
set(Boost_USE_MULTITHREADED     OFF)
set(Boost_USE_STATIC_RUNTIME    OFF)

find_package(PythonLibs ${PYTHON_MIN_VERSION} REQUIRED)
find_package(PythonExtensions REQUIRED)
find_package(Boost ${BOOST_MIN_VERSION} REQUIRED COMPONENTS graph python)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


file(GLOB SOURCES "${NctxPySrc}/*.cpp")
include_directories ("${NctxPySrc}/python_cpp")
include_directories ("${NctxPySrc}/python_defs")
include_directories(${Boost_INCLUDE_DIRS})

add_library(core MODULE "${NctxPySrc}/core.cpp")
target_link_libraries(core ${Boost_LIBRARIES})
python_extension_module(core)

add_library(directed MODULE "${NctxPySrc}/directed.cpp")
target_link_libraries(directed ${Boost_LIBRARIES})
python_extension_module(directed)

add_library(undirected MODULE "${NctxPySrc}/undirected.cpp")
target_link_libraries(undirected ${Boost_LIBRARIES})
python_extension_module(undirected)

install(TARGETS core LIBRARY DESTINATION nctx)
install(TARGETS directed LIBRARY DESTINATION nctx)
install(TARGETS undirected LIBRARY DESTINATION nctx)
