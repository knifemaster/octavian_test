cmake_minimum_required(VERSION 3.10)

project(Octavian)

set(CMAKE_CXX_STANDART 17)
set(CMAKE_CXX_STANDART_REQUIRED ON)

set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")

set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")

set(LIB_DIR "${CMAKE_SOURCE_DIR}/lib")


add_executable(Octavian  ${SOURCE_DIR}/main.cpp ${LIB_DIR}/glad.c)

target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR})

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

target_link_libraries(${PROJECT_NAME} OpenGL::GL glfw)

#include_directories(include)

#find_package(OpenGL REQUIRED COMPONENTS OpenGL)

#find_package(GLUT REQUIRED)

#add_dependencies(Octavian OpenGL::OpenGL)

#include_directories(${GLUT_INCLUDE_DIRS})

#target_link_libraries(Octavian OpenGL::OpenGL ${GLUT_LIBRARY})
