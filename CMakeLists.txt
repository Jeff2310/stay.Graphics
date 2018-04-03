cmake_minimum_required(VERSION 3.9)
project(Graphics)

set(CMAKE_CXX_STANDARD 11)

#设置GLFW库的头文件目录
set(GLFW3_INCLUDE_DIR ${SUMMARY_DEBUG_SOURCE_DIR}/opengl_helper/include/GLFW)
#设置GLFW库的库文件路径
set(GLFW3_LIBRARY ${SUMMARY_DEBUG_SOURCE_DIR}/opengl_helper/x64/libs/glfw3.lib)

#设置GLEW库的头文件目录
set(GLEW_INCLUDE_DIR ${SUMMARY_DEBUG_SOURCE_DIR}/opengl_helper/include/GLEW)
#设置GLEW库的库文件目录
set(GLEW_LIBRARY ${SUMMARY_DEBUG_SOURCE_DIR}/opengl_helper/x64/libs/glew32.lib)

#设置glm库的头文件目录，改库不需要预先进行编译，可以直接使用
set(GLM_INCLUDE_DIRS ${SUMMARY_DEBUG_SOURCE_DIR}/opengl_helper/include/glm)

#将上述的头文件目录添加到一个列表变量之中
list( APPEND SUMMARY_DEBUG_INCLUDE ${GLM_INCLUDE_DIRS} )
list( APPEND SUMMARY_DEBUG_INCLUDE ${GLFW3_INCLUDE_DIR} )
list( APPEND SUMMARY_DEBUG_INCLUDE ${GLEW_INCLUDE_DIR} )

#将上述的库文件目录添加到一个列表变量之中
list( APPEND SUMMARY_DEBUG_LIBS ${GLFW3_LIBRARY} )
list( APPEND SUMMARY_DEBUG_LIBS ${GLEW_LIBRARY} )

#利用find_package命令找到OpenGL
find_package( OpenGL REQUIRED )
list( APPEND SUMMARY_DEBUG_LIBS ${OPENGL_gl_LIBRARY} )

#设置项目的头文件包含目录
include_directories( ${SUMMARY_DEBUG_INCLUDE} )

#在当前目录下搜索头文件及源文件
file(GLOB_RECURSE CURRENT_HEADERS  *.h *.hpp)
file(GLOB CURRENT_SOURCES  *.cpp)
#为头文件和源文件进行分组
source_group("Include" FILES ${CURRENT_HEADERS})
source_group("Source" FILES ${CURRENT_SOURCES})

add_executable(main_project ${CURRENT_HEADERS} ${CURRENT_SOURCES} Camera.h)
#设置项目的外部依赖库
target_link_libraries( main_project  -lglfw -lGLEW -lGLU -lGL)