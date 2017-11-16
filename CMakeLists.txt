cmake_minimum_required(VERSION 3.9)

project(steiner_trees VERSION 0.1 DESCRIPTION "Steiner Tree MIP Solver")

set(WARNING_FLAGS "-Wall -Wextra -Werror -pedantic")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${WARNING_FLAGS}")

set(SOURCE_FILES
        source/main.cpp
        source/debug.cpp
        source/json.cpp
        source/python.cpp
        source/Output.cpp
        source/Storage.cpp
        source/Logger.cpp
)

set(HELPER_FILES
        source/helper/vector.cpp
        source/helper/map.cpp
)

set(GRAPH_FILES
        source/graph/typedefs.cpp
        source/graph/Node.cpp
        source/graph/Edge.cpp
        source/graph/Graph.cpp
        source/graph/GraphPrinter.cpp
        source/graph/GraphFactory.cpp
        source/graph/NetsFactory.cpp
        source/graph/CostsFactory.cpp
        source/graph/Net.cpp
        source/graph/Costs.cpp
        source/graph/algorithms/Dijkstra.cpp
        source/graph/algorithms/MooreBellmanFord.cpp
)

set(MIP_FILES
        source/mip/Variable.cpp
        source/mip/VariableStorage.cpp
        source/mip/Constraint.cpp
        source/mip/Group.cpp
        source/mip/GroupManager.cpp
        source/mip/MIPModel.cpp
        source/mip/MIPSolver.cpp
        source/mip/MIP.cpp
        source/mip/Solution.cpp source/mip/Solution.hpp)

set(STEINER_TREE_FILES
        source/steiner_trees.cpp
        source/steiner_trees/SteinerTreeProblem.cpp
        source/steiner_trees/SteinerTreeSolution.cpp
        source/steiner_trees/SteinerTreeSolver.cpp
        source/steiner_trees/SteinerTreeCut.cpp
)

set(STEINER_TREE_MIPS_FILES
        source/steiner_trees/mips/natural_multi_commodity_flow/GroupEdges.cpp
        source/steiner_trees/mips/natural_multi_commodity_flow/GroupMultiCommodityFlow.cpp
        source/steiner_trees/mips/natural_multi_commodity_flow/GroupEdgesMultiCommodityFlow.cpp
        source/steiner_trees/mips/natural_multi_commodity_flow/GroupSteinerTreeCuts.cpp
        source/steiner_trees/mips/SteinerTreeMIPFactory.cpp source/steiner_trees/mips/SteinerTreeMIPFactory.hpp
        source/steiner_trees/mips/SteinerTreeMIP.cpp source/steiner_trees/mips/SteinerTreeMIP.hpp)

set(YANNICK_FILES
        source/yannick.cpp
        source/yannick/YannickProblem.cpp
        source/yannick/YannickSolution.cpp
        source/yannick/YannickSolver.cpp
        source/yannick/YannickMIP.cpp
        source/yannick/YannickMIPFactory.cpp
        source/yannick/GroupTaskTime.cpp
        source/yannick/GroupTaskCycle.cpp
        source/yannick/GroupTaskMachine.cpp
        source/yannick/GroupTaskMachineOrder.cpp
)

# Python
find_package(PythonLibs 3.5 REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})

# Python
find_package(Threads REQUIRED)

# Boost
#set(BOOST_LOG_DYN_LINK ON)
find_package(Boost 1.61 COMPONENTS python-py35 REQUIRED) # log
include_directories(${Boost_INCLUDE_DIR})

# Google or-tools
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUSE_GLOP -DUSE_CBC") # -fwrapv -DNDEBUG

include_directories(
        or-tools-6.4/
        or-tools-6.4/dependencies/install/include
        or-tools-6.4/ortools/gen/
)

link_directories(
        or-tools-6.4/lib
)

add_executable(steiner_trees
        ${SOURCE_FILES}
        ${HELPER_FILES}
        ${GRAPH_FILES}
        ${MIP_FILES}
        ${STEINER_TREE_FILES}
        ${STEINER_TREE_MIPS_FILES}
)

add_executable(yannick
        ${SOURCE_FILES}
        ${HELPER_FILES}
        ${GRAPH_FILES}
        ${MIP_FILES}
        ${YANNICK_FILES}
)

add_library(steiner_trees_python_lib SHARED
        ${SOURCE_FILES}
        ${HELPER_FILES}
        ${GRAPH_FILES}
        ${MIP_FILES}
        ${STEINER_TREE_FILES}
        ${STEINER_TREE_MIPS_FILES}
)

target_link_libraries(steiner_trees
        ${PYTHON_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
        ${Boost_LIBRARIES}
        libortools.so
)

target_link_libraries(yannick
        ${PYTHON_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
        ${Boost_LIBRARIES}
        libortools.so
)

target_link_libraries(steiner_trees_python_lib
        ${PYTHON_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
        ${Boost_LIBRARIES}
        libortools.so
)

include(GNUInstallDirs)
install(TARGETS steiner_trees_python_lib
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)