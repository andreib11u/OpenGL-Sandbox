cmake_minimum_required(VERSION 3.30)
project(OpenGLApp)

set(CMAKE_CXX_STANDARD 20)

set(THIRD_PARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../third-party)
set(THIRD_PARTY_INCLUDE ${THIRD_PARTY_DIR}/include)
set(THIRD_PARTY_LIB ${THIRD_PARTY_DIR}/lib)

file(GLOB APP_SOURCES CONFIGURE_DEPENDS "*.h" "*.cpp" "*.inl" "*.c") # todo: add more extensions

add_executable(OpenGLApp ${APP_SOURCES})

target_include_directories(OpenGLApp PUBLIC ${THIRD_PARTY_INCLUDE})
target_link_directories(OpenGLApp PRIVATE ${THIRD_PARTY_LIB})
target_link_libraries(OpenGLApp PRIVATE glfw3)

set(SHADERS_DIR ${CMAKE_BINARY_DIR})

if (MSVC)
    set(SHADERS_DIR ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}) # MSVC puts binaries into a subfolder named as config
endif ()

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders DESTINATION ${SHADERS_DIR})