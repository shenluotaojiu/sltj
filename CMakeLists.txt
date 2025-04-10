cmake_minimum_required(VERSION 3.28.3) # 最小版本
project(server)

set (CMAKE_VERBOSE_MAKEFILE ON) # 
set (CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O3 -g -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function")

include_directories(.)

# 设置源文件别名
set(LIB_SRC
    src/log.cc
    src/util.cc
    src/config.cc
    src/thread.cc
)

add_library(sltj SHARED ${LIB_SRC})
#add_library(sltj_static STATIC ${LIB_SRC})
#SET_TARGET_PROPERTIES (sltj_static PROPERTIES OUTPUT_NAME "sltj")

# 设置依赖库别名
set (LIB_LIB
    sltj
    pthread
    )

# 执行文件项，也就是main函数所在位置
add_executable(test test/test.cc)
add_dependencies(test sltj)
target_link_libraries(test ${LIB_LIB})

add_executable(test_config test/test_config.cc)
# 添加依赖项
add_dependencies(test_config sltj)
# 链接外部库
target_link_libraries(test_config ${LIB_LIB})

add_executable(test_thread test/test_thread.cc)
add_dependencies(test_thread sltj)
target_link_libraries(test_thread ${LIB_LIB})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
