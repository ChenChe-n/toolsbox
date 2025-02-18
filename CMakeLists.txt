# CMakeList.txt: toolsbox 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.12)

# 如果支持，请为 MSVC 编译器启用热重载。
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("toolsbox")

# 包含所有 tools 目录中的源文件
file(GLOB_RECURSE TOOLS_SOURCES module/*.cpp)

# 包含目录（头文件所在目录）
include_directories(${CMAKE_SOURCE_DIR}/module)

# 定义主源文件
set(MAIN_FILE ${CMAKE_SOURCE_DIR}/main.cpp)

# 添加可执行目标
add_executable(${PROJECT_NAME} ${MAIN_FILE} ${TOOLS_SOURCES})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET toolsbox PROPERTY CXX_STANDARD 20)
endif()

# O2 优化
if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET toolsbox PROPERTY CXX_FLAGS_RELEASE "-O2")
endif()


# TODO: 如有需要，请添加测试并安装目标。
