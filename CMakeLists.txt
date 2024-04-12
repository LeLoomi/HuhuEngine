cmake_minimum_required(VERSION 3.28.0)
set (CMAKE_CXX_STANDARD 17)

#---------------------------#
set(PROJECT_NAME HuhuEngine)
set(PROJECT_VERSION 0.1.43)

set(COMPILER_PATH "/usr/bin/g++")
set(VULKAN_SDK_PATH "/Users/eliahlohr/VulkanSDK/1.3.275.0/macOS")
set(GLFW_PATH "/opt/homebrew/Cellar/GLFW/3.4")
#---------------------------#

project(${PROJECT_NAME} VERSION ${PROJECT_VERSION})
set(CMAKE_CXX_COMPILER  ${COMPILER_PATH})

# TODO: Upgrade away from globbing since it can cause issues on other OSs
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/src/*.cpp)
# target_sources(${PROJECT_NAME} PUBLIC ${SOURCES})
add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC
    ${VULKAN_SDK_PATH}/include
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/external/tinyobjloader
    ${GLFW_PATH}/include
)

target_link_libraries(${PROJECT_NAME}
    ${VULKAN_SDK_PATH}/lib/libvulkan.dylib
    ${GLFW_PATH}/lib/libglfw.3.dylib
)

