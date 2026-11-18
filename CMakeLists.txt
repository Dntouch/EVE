cmake_minimum_required(VERSION 3.16)

project(EVE VERSION 0.1.0 LANGUAGES CXX)

find_package(Threads REQUIRED)

add_library(eve_core
    src/neural/network.cpp
    src/brainstem/brainstem.cpp
    src/bios/runtime.cpp
)
target_include_directories(eve_core PUBLIC include)
target_compile_features(eve_core PUBLIC cxx_std_17)

if(MSVC)
    target_compile_options(eve_core PRIVATE /W4 /permissive-)
else()
    target_compile_options(eve_core PRIVATE -Wall -Wextra -Wpedantic -Wshadow)
endif()

add_executable(eve_experiment src/main.cpp)
target_link_libraries(eve_experiment PRIVATE eve_core)

add_executable(eve_sequence_experiment src/sequence_experiment.cpp)
target_link_libraries(eve_sequence_experiment PRIVATE eve_core)

add_executable(eve_performance_benchmark src/performance_benchmark.cpp)
target_link_libraries(eve_performance_benchmark PRIVATE eve_core)

add_executable(eve_research_002 src/research_002.cpp)
target_link_libraries(eve_research_002 PRIVATE eve_core)

add_executable(eve_experiment_002 src/experiment_002.cpp)
target_link_libraries(eve_experiment_002 PRIVATE eve_core)

add_executable(eve_experiment_003 src/experiment_003.cpp)
target_link_libraries(eve_experiment_003 PRIVATE eve_core Threads::Threads)

add_executable(eve_experiment_003_benchmark src/experiment_003_benchmark.cpp)
target_link_libraries(eve_experiment_003_benchmark PRIVATE eve_core)

add_executable(eve_research_stage_a src/research_stage_a.cpp)
target_link_libraries(eve_research_stage_a PRIVATE eve_core)

add_executable(eve_research_stage_a1 src/research_stage_a1.cpp)
target_link_libraries(eve_research_stage_a1 PRIVATE eve_core)

add_executable(eve_research_stage_a2 src/research_stage_a2.cpp)
target_link_libraries(eve_research_stage_a2 PRIVATE eve_core)

add_executable(eve_research_stage_b src/research_stage_b.cpp)
target_link_libraries(eve_research_stage_b PRIVATE eve_core)

add_executable(eve_research_stage_b1 src/research_stage_b1.cpp)
target_link_libraries(eve_research_stage_b1 PRIVATE eve_core)

add_executable(eve_research_stage_b2 src/research_stage_b2.cpp)
target_link_libraries(eve_research_stage_b2 PRIVATE eve_core)

add_executable(eve_research_stage_b3 src/research_stage_b3.cpp)
target_link_libraries(eve_research_stage_b3 PRIVATE eve_core)

add_executable(eve_research_stage_b31 src/research_stage_b31.cpp)
target_link_libraries(eve_research_stage_b31 PRIVATE eve_core)

add_executable(eve_research_stage_b4 src/research_stage_b4.cpp)
target_link_libraries(eve_research_stage_b4 PRIVATE eve_core)

add_executable(eve_research_stage_b41 src/research_stage_b41.cpp)
target_link_libraries(eve_research_stage_b41 PRIVATE eve_core)

add_executable(eve_research_stage_b42 src/research_stage_b42.cpp)
target_link_libraries(eve_research_stage_b42 PRIVATE eve_core)

add_executable(eve_research_stage_b43 src/research_stage_b43.cpp)
target_link_libraries(eve_research_stage_b43 PRIVATE eve_core)

add_executable(eve_research_stage_b44 src/research_stage_b44.cpp)
target_link_libraries(eve_research_stage_b44 PRIVATE eve_core)

add_executable(eve_research_stage_b45 src/research_stage_b45.cpp)
target_link_libraries(eve_research_stage_b45 PRIVATE eve_core)

add_executable(eve_research_stage_b46 src/research_stage_b46.cpp)
target_link_libraries(eve_research_stage_b46 PRIVATE eve_core)

add_executable(eve_research_stage_b47 src/research_stage_b47.cpp)
target_link_libraries(eve_research_stage_b47 PRIVATE eve_core)

add_executable(eve_research_stage_b48 src/research_stage_b48.cpp)
target_link_libraries(eve_research_stage_b48 PRIVATE eve_core)

include(CTest)
if(BUILD_TESTING)
    add_executable(eve_tests tests/test_main.cpp)
    target_link_libraries(eve_tests PRIVATE eve_core)
    add_test(NAME eve_core_tests COMMAND eve_tests)
endif()
