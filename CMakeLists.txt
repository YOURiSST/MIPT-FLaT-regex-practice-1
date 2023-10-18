cmake_minimum_required(VERSION 3.23)
project(REGEX_SOLVER)

set(CMAKE_CXX_STANDARD 20)

add_executable(REGEX_SOLVER main.cpp regex_solver.cpp)

cmake_minimum_required(VERSION 3.6)
project(coverage-example)

if(ENABLE_COVERAGE)
    set(CMAKE_CXX_FLAGS "-O0 -coverage")
    find_program(LCOV lcov REQUIRED)
    find_program(GENHTML genhtml REQUIRED)

    add_custom_target(coverage
            COMMAND ${LCOV} --directory . --capture --output-file coverage.info
            COMMAND ${GENHTML} --demangle-cpp -o coverage coverage.info
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
endif()

add_executable(REGEX_SOLVER_TESTING test.cpp regex_solver.cpp)