cmake_minimum_required(VERSION 3.22)
set(TARGET_NAME FUEquip)
project(${TARGET_NAME})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

############# 3rd part Files ###############
set(EXTERN_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/extern)

if (NOT TARGET glfw)
    option(GLFW_BUILD_EXAMPLES "" OFF)
    option(GLFW_BUILD_TESTS "" OFF)
    option(GLFW_BUILD_DOCS "" OFF)
    option(GLFW_INSTALL "" OFF)
    add_subdirectory(${EXTERN_INCLUDE}/glfw)
endif ()

if (NOT TARGET rapidjson)
    file(GLOB rapidjson_sources CONFIGURE_DEPENDS  "${EXTERN_INCLUDE}/rapidjson/include/*.h")
    add_library(rapidjson INTERFACE ${rapidjson_sources})
    target_include_directories(rapidjson INTERFACE ${EXTERN_INCLUDE}/rapidjson/include)
endif ()

if (NOT TARGET spdlog)
    file(GLOB spdlog_sources CONFIGURE_DEPENDS  "${EXTERN_INCLUDE}/spdlog/include/*.h")
    add_library(spdlog INTERFACE ${spdlog_sources})
    target_include_directories(spdlog INTERFACE ${EXTERN_INCLUDE}/spdlog/include)
endif ()

############# Add Source File ###############
set(RUNTIME_TARGET_NAME FUEquipRuntime)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
file(GLOB_RECURSE HEADER_FILES "src/*.h")
file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${HEADER_FILES} ${SOURCE_FILES})

add_library(${RUNTIME_TARGET_NAME} ${HEADER_FILES} ${SOURCE_FILES})

target_link_libraries(${RUNTIME_TARGET_NAME} PUBLIC glfw)
target_link_libraries(${RUNTIME_TARGET_NAME} PUBLIC rapidjson)
target_link_libraries(${RUNTIME_TARGET_NAME} PUBLIC spdlog)

############# TEST Project ###############
add_subdirectory(test)

############# Final Project ###############
add_executable(${TARGET_NAME} main.cpp)
target_link_libraries(${TARGET_NAME} PUBLIC ${RUNTIME_TARGET_NAME})

## 设置编译类型
#set(CMAKE_BUILD_TYPE Debug) # 或 Release
# 根据编译类型定义宏
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DMYDEBUG)
endif()


# Asset Load
add_custom_command(
        TARGET ${TARGET_NAME}
        COMMENT "Copy Config"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${TARGET_NAME}>/assets
        VERBATIM)