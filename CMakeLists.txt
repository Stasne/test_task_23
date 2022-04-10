cmake_minimum_required(VERSION "2.8" )
project(saber_test)
set(CMAKE_CXX_STANDARD 17)
file (GLOB SRC "src/*.cpp")
file (GLOB HDR "src/*.h")

add_executable(${PROJECT_NAME} ${SRC} ${HDR})
