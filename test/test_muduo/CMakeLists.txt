# cmake_minimum_required(VERSION 3.0)
# project(main) # 是工程的名字 不是可执行文件的名字

# 设置debug模式 如果没有 将不能调试设置断点
# set(CMAKE_BUILD_TYPE "Debug")

# 配置编译选项
# set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -g) # -g 可执行文件 可以调试

# 指定搜索的子目录
# add_subdirectory(chat/test_muduo)

# 设置可执行文件最终的存储路径
# set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# 配置头文件搜索路径
# include_directories()

# 配置库文件搜索路径
# link_directories()

# 设置需要编译的源文件列表
# set(SRC_LIST ./src/muduo_server.cpp)
set(SRC_LIST ./src/muduo_server.cpp)

# 设置可执行文件最终的存储路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
# set(EXECUTABLE_OUTPUT_PATH ../bin)

# 设置生成的静态库和动态库的存储目录 一般为${PROJECT_SOURCE_DIR}/lib
# LIBRARY_OUTPUT_PATH

# 把.指定路径下的所有源文件名字放入变量名SRC_LIST里面
# aux_source_directory(. SRC_LIST)

# 表示生成可执行文件server_cmake 由SRC_LIST变量所定义的源文件编译而来
add_executable(server_cmake ${SRC_LIST})
# add_executable(server muduo_server.cpp)
# add_library()

# 进入子目录下 执行 CMakeLists.txt文件  这里的子目录下都有可以编译的代码文件
# add_subdirectory()

# 表示server_cmake这个目标文件 需要链接 muduo_net muduo_base pthread 这三个库文件
target_link_libraries(server_cmake muduo_net muduo_base pthread)
