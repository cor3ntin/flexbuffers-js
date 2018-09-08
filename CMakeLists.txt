cmake_minimum_required(VERSION 3.12)
project(flexbuffers_wasm)

set(CMAKE_CXX_STANDARD 17)

#
# Trying usual cmake project compiler execution
#

add_definitions("-fno-rtti -fno-exceptions")
add_executable(flexbuffers flexbuffer.cpp pre.js)
target_include_directories(flexbuffers PRIVATE ${EMSCRIPTEN_ROOT_PATH}/system/include flatbuffers/include)
target_compile_definitions(flexbuffers PUBLIC -DEMSCRIPTEN_HAS_UNBOUND_TYPE_NAMES=0)
set_target_properties(flexbuffers PROPERTIES LINK_FLAGS "-s DEFAULT_LIBRARY_FUNCS_TO_INCLUDE=\"['memcpy','memset','malloc','free','strlen']\" -s WASM=1 -s EXPORT_NAME=FlexBuffers -DEMSCRIPTEN_HAS_UNBOUND_TYPE_NAMES=0 -g0 -s DISABLE_EXCEPTION_CATCHING=1 -s NO_FILESYSTEM=1 -Os -fno-rtti -fno-exceptions --llvm-lto 1 --bind -s MODULARIZE=1 --pre-js ${CMAKE_CURRENT_SOURCE_DIR}/pre.js")
