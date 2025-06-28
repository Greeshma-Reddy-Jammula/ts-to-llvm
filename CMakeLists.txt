cmake_minimum_required(VERSION 3.15)
project(ts_to_llvm)

find_package(LLVM REQUIRED CONFIG)

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_executable(ts_to_llvm codegen.cpp)
llvm_map_components_to_libnames(llvm_libs support core irreader passes analysis)
target_link_libraries(ts_to_llvm PRIVATE ${llvm_libs})
