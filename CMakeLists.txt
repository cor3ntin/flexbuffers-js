cmake_minimum_required(VERSION 3.12)
project(flexbuffers_wasm)

set(CMAKE_CXX_STANDARD 17)

#
# Trying usual cmake project compiler execution
#

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY dist)
SET(ENV{EMCC_CLOSURE_ARGS} "--language_in=ECMASCRIPT6")

add_definitions("-fno-rtti -fno-exceptions -Os -s NO_FILESYSTEM=1")
add_executable(flexbuffers.js flexbuffer.cpp pre.js)
target_include_directories(flexbuffers.js PRIVATE ${EMSCRIPTEN_ROOT_PATH}/system/include flatbuffers/include)
target_compile_definitions(flexbuffers.js PUBLIC -DEMSCRIPTEN_HAS_UNBOUND_TYPE_NAMES=0)
set_target_properties(flexbuffers.js PROPERTIES LINK_FLAGS "-s ENVIRONMENT=web -s NO_FILESYSTEM=1 -s DEFAULT_LIBRARY_FUNCS_TO_INCLUDE=\"['memcpy','memset','malloc','free','strlen']\" -s WASM=1 -s EXPORT_NAME=FlexBuffers -DEMSCRIPTEN_HAS_UNBOUND_TYPE_NAMES=0 -g0 -s DISABLE_EXCEPTION_CATCHING=1 -s SINGLE_FILE=1 -Os -fno-rtti -fno-exceptions --llvm-lto 1 --bind -s MODULARIZE=1 -s EXPORT_ES6=1 --pre-js ${CMAKE_CURRENT_SOURCE_DIR}/pre.js")

configure_file(package.json dist/package.json COPYONLY)
configure_file(flexbuffers.d.ts dist/flexbuffers.d.ts COPYONLY)
configure_file(flatbuffers/js/flatbuffers.js dist/flatbuffers.js COPYONLY)
