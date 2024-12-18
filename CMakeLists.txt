cmake_minimum_required(VERSION 3.10)

project(Octavian)

set(CMAKE_CXX_STANDART 17)
set(CMAKE_CXX_STANDART_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-g")

set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")

set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")

set(LIB_DIR "${CMAKE_SOURCE_DIR}/lib")


add_executable(Octavian  ${SOURCE_DIR}/main.cpp ${LIB_DIR}/glad.c)

target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR})

find_package(OpenGL REQUIRED)
find_package(GLU)
find_package(glfw3 REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)
find_package(Freetype REQUIRED)
find_package(glm REQUIRED)


include_directories(${GLEW_INCLUDE_DIRS} )
include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS}) 
include_directories(${FREETYPE_INCLUDE_DIR_freetype2})

target_link_libraries(${PROJECT_NAME} glm::glm Freetype::Freetype OpenGL::GL glfw GLU GLUT::GLUT  ${GLEW_LIBRARIES})


