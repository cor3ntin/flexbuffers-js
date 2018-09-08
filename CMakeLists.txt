cmake_minimum_required(VERSION 3.12)
project(flexbuffers_wasm)

set(CMAKE_CXX_STANDARD 17)

#
# Trying usual cmake project compiler execution
#

add_definitions("-fno-rtti -fno-exceptions")
add_executable(flexbuffer flexbuffer.cpp)
target_include_directories(flexbuffer PRIVATE ${EMSCRIPTEN_ROOT_PATH}/system/include flatbuffers/include)
target_compile_definitions(flexbuffer PUBLIC -DEMSCRIPTEN_HAS_UNBOUND_TYPE_NAMES=0)
set_target_properties(flexbuffer PROPERTIES LINK_FLAGS "-DEMSCRIPTEN_HAS_UNBOUND_TYPE_NAMES=0 -s DISABLE_EXCEPTION_CATCHING=1 -s NO_FILESYSTEM=1 -O3 -fno-rtti -fno-exceptions --llvm-lto 1 --bind --closure 1")
