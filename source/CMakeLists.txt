cmake_minimum_required(VERSION 3.30)
project(OpenGLApp)

set(CMAKE_CXX_STANDARD 20)

find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

set(SOURCE_FILE_EXTENSIONS "*.h;*.hpp;*.hxx;*.cpp;*.cxx;*.inl;*.c")
file(GLOB_RECURSE APP_SOURCES CONFIGURE_DEPENDS ${SOURCE_FILE_EXTENSIONS})

add_executable(OpenGLApp ${APP_SOURCES})

target_include_directories(OpenGLApp PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(OpenGLApp PRIVATE glad::glad)
target_link_libraries(OpenGLApp PRIVATE glfw)

set(SHADERS_DIR ${CMAKE_BINARY_DIR})

if (MSVC)
    set(SHADERS_DIR ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}) # MSVC puts binaries into a subfolder named as config
endif ()

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Shaders DESTINATION ${SHADERS_DIR})