cmake_minimum_required(VERSION 3.10)
project(wmbusmeters-cli-ex)
enable_language(CXX)

find_package(PkgConfig REQUIRED)
pkg_check_modules(WMBUS REQUIRED wmbusmeters)

add_executable(main main.cpp)
target_include_directories(main PRIVATE ${WMBUS_INCLUDE_DIRS})
target_link_libraries(main ${WMBUS_LDFLAGS})
