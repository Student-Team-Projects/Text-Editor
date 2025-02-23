cmake_minimum_required (VERSION 3.16)

project (text-editor)

function(set_warnings t)
    target_compile_options(${t} PRIVATE
        -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wcast-qual -Wformat=2 -Wundef -g -D_GLIBCXX_DEBUG
    )
endfunction()

# Dependencies

include(deps/CMakeLists.txt)

# Target 'text-editor-core'

file(GLOB_RECURSE TEXT_EDITOR_CORE_SRC "${CMAKE_CURRENT_LIST_DIR}/source/text-editor-core/*.cpp")
add_library(text-editor-core
    ${TEXT_EDITOR_CORE_SRC}
)

target_compile_features(text-editor-core PRIVATE cxx_std_20)
set_warnings(text-editor-core)
target_include_directories(text-editor-core PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>"
    "$<INSTALL_INTERFACE:include>"
)
target_include_directories(text-editor-core PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/source/text-editor-core"
)
target_link_libraries(text-editor-core PUBLIC
    tvision
    efsw
)

# Target 'text-editor'

file(GLOB_RECURSE TEXT_EDITOR_SRC "${CMAKE_CURRENT_LIST_DIR}/source/text-editor/*.cpp")
add_executable(text-editor ${TEXT_EDITOR_SRC})
target_compile_features(text-editor PRIVATE cxx_std_20)
set_warnings(text-editor)
target_link_libraries(text-editor PRIVATE
text-editor-core
tvterm-core
)
