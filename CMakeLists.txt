cmake_minimum_required(VERSION 3.9)

project(steiner_trees LANGUAGES CXX VERSION 0.1 DESCRIPTION "Steiner Tree MIP Solver")

include(GNUInstallDirs)

set(WARNING_FLAGS "-Wall -Wextra -Werror -pedantic")
set(OPTIMIZATION_FLAGS "-DNDEBUG -O3")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${WARNING_FLAGS} ${OPTIMIZATION_FLAGS}")

set(SOURCE_FILES
        source/main.cpp
        source/typedefs.cpp
        source/debug.cpp
        source/json.cpp
#        source/python.cpp
        source/Output.cpp
        source/Storage.cpp
        source/Logger.cpp
)

set(HELPER_FILES
        source/helper/vector.cpp
        source/helper/map.cpp
        source/helper/PowerSetIterator.cpp
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
        source/graph/TerminalInstance.cpp
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

set(DIJKSTRA_STEINER_FILES
        source/steiner_trees/dijkstra_steiner/DijkstraSteiner.cpp
        source/steiner_trees/dijkstra_steiner/DijkstraSteinerAlgorithm.hpp
        source/steiner_trees/dijkstra_steiner/SteinerTreeKey.hpp
        source/steiner_trees/dijkstra_steiner/SteinerTreeLabel.hpp
        source/steiner_trees/dijkstra_steiner/SteinerTreeLabelQueue.hpp
        source/steiner_trees/dijkstra_steiner/SteinerTreeLabelTable.hpp
        source/steiner_trees/dijkstra_steiner/SteinerTreeLowerBound.hpp
        source/steiner_trees/dijkstra_steiner/SteinerTreesManager.hpp
)

set(DATA_FILES
        source/data/BinaryHeap.hpp
        source/data/BitSet.hpp
        source/data/BitSetEnumerator.hpp
        source/data/BitSetTable.hpp
)

set(STEINER_TREE_FILES
        source/steiner_trees.cpp
        source/steiner_trees/SteinerTreeProblem.cpp
        source/steiner_trees/SteinerTreeSolution.cpp
        source/steiner_trees/SteinerTreeSolver.cpp
        source/steiner_trees/SteinerTreeCut.cpp
#        source/steiner_trees/optimal_integrality_gap/OptimalIntegralityGap.cpp
#        source/steiner_trees/optimal_integrality_gap/OptimalEdgeWeights.cpp
)

set(STEINER_TREE_MIPS_FILES
        source/steiner_trees/mips/groups/GroupBidirectedMultiCommodityCommonFlow.cpp
        source/steiner_trees/mips/bidirected_cuts_dual/GroupBidirectedCutsDual.cpp
        source/steiner_trees/mips/multi_commodity_dual/GroupMultiCommodityDual.cpp
        source/steiner_trees/mips/natural_multi_commodity_flow/GroupDynamicGraph.cpp
        source/steiner_trees/mips/natural_multi_commodity_flow/GroupCommonFlow.cpp
        source/steiner_trees/mips/natural_multi_commodity_flow/GroupEdges.cpp
        source/steiner_trees/mips/natural_multi_commodity_flow/GroupMultiCommodityFlow.cpp
        source/steiner_trees/mips/natural_multi_commodity_flow/GroupSteinerTreeCuts.cpp
        source/steiner_trees/mips/simplex_embedding/GroupSimplexEmbedding.cpp
        source/steiner_trees/mips/objective/GroupMultiCommodityOptimal.cpp
        source/steiner_trees/mips/SteinerTreeMIPFactory.cpp source/steiner_trees/mips/SteinerTreeMIPFactory.hpp
        source/steiner_trees/mips/SteinerTreeMIP.cpp source/steiner_trees/mips/SteinerTreeMIP.hpp)

set(UNIT_TESTS_FILES
        source/unit_tests/unit_tests.cpp
        source/unit_tests/test_BitSetEnumerator.cpp)

set(SKUTELLA_FILES
        source/skutella.cpp
        source/skutella/SkutellaProblem.cpp
        source/skutella/SkutellaSolution.cpp
        source/skutella/SkutellaSolver.cpp
        source/skutella/SkutellaMIP.cpp
        source/skutella/SkutellaMIPFactory.cpp
        source/skutella/GroupSkutella.cpp
)

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
        source/yannick/GroupTaskPrecedence.cpp
        source/yannick/GroupMachine.cpp
        source/yannick/GroupCycle.cpp
        source/yannick/GroupMachineCycle.cpp
)

# Python
#find_package(PythonLibs 3.5 REQUIRED)
#include_directories(${PYTHON_INCLUDE_DIRS})

# Python
#find_package(Threads REQUIRED)

# Boost
#set(BOOST_LOG_DYN_LINK ON)
#find_package(Boost 1.61 COMPONENTS python-py35 REQUIRED) # log
#include_directories(${Boost_INCLUDE_DIR})

# Google or-tools
set(GOOGLE_OR_TOOLS_DIR or-tools-6.7.1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/opt/gurobi800/linux64/include -DUSE_GUROBI")

include_directories(
        ${GOOGLE_OR_TOOLS_DIR}
        ${GOOGLE_OR_TOOLS_DIR}/dependencies/install/include
        ${GOOGLE_OR_TOOLS_DIR}/ortools/gen/
)

link_directories(
        ${GOOGLE_OR_TOOLS_DIR}/lib
)

add_executable(steiner_trees
        ${SOURCE_FILES}
        ${HELPER_FILES}
        ${DATA_FILES}
        ${GRAPH_FILES}
        ${MIP_FILES}
        ${DIJKSTRA_STEINER_FILES}
        ${STEINER_TREE_FILES}
        ${STEINER_TREE_MIPS_FILES}
        ${SKUTELLA_FILES}
)

add_executable(steiner_trees_unit_tests
        ${DATA_FILES}
        ${UNIT_TESTS_FILES}
        )

target_include_directories(steiner_trees PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/source)

add_executable(yannick
        ${SOURCE_FILES}
        ${HELPER_FILES}
        ${GRAPH_FILES}
        ${MIP_FILES}
        ${YANNICK_FILES}
)

#add_library(steiner_trees_python_lib SHARED
#        ${SOURCE_FILES}
#        ${HELPER_FILES}
#        ${GRAPH_FILES}
#        ${MIP_FILES}
#        ${STEINER_TREE_FILES}
#        ${STEINER_TREE_MIPS_FILES}
#)

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

#target_link_libraries(steiner_trees_python_lib
#        ${PYTHON_LIBRARIES}
#        ${CMAKE_THREAD_LIBS_INIT}
#        ${Boost_LIBRARIES}
#        libortools.so
#)

#include(GNUInstallDirs)
#install(TARGETS steiner_trees_python_lib
#        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
#)
