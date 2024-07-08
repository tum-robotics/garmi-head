cmake_minimum_required(VERSION 3.15...3.26)
project(${SKBUILD_PROJECT_NAME} LANGUAGES CXX)

set(THREADS_PREFER_PTHREAD_FLAG ON)
set(PYBIND11_FINDPYTHON ON)
set(HARDWARE_NAME "tum_head")
find_package(pybind11 CONFIG REQUIRED)
find_package(ethercat_interface REQUIRED)
find_package(${HARDWARE_NAME}_driver REQUIRED)
find_package(Threads REQUIRED)

pybind11_add_module(_core MODULE src/main.cpp)

target_link_libraries(_core PUBLIC
    ${HARDWARE_NAME}_driver::${HARDWARE_NAME}_driver Threads::Threads
)

target_include_directories(_core SYSTEM PUBLIC
    include
)

install(TARGETS _core DESTINATION ${SKBUILD_PROJECT_NAME})
