cmake_minimum_required (VERSION 3.16)

project (turbo)

function(turbo_set_warnings t)
    target_compile_options(${t} PRIVATE
        -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wcast-qual -Wformat=2 -Wundef -g -D_GLIBCXX_DEBUG
    )
endfunction()

# Dependencies

include(deps/CMakeLists.txt)

# Target 'turbo-core'

file(GLOB_RECURSE TURBO_CORE_SRC "${CMAKE_CURRENT_LIST_DIR}/source/turbo-core/*.cpp")
add_library(turbo-core
    ${TURBO_CORE_SRC}
)

target_compile_features(turbo-core PRIVATE cxx_std_20)
turbo_set_warnings(turbo-core)
target_include_directories(turbo-core PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>"
    "$<INSTALL_INTERFACE:include>"
)
target_include_directories(turbo-core PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/source/turbo-core"
)
target_link_libraries(turbo-core PUBLIC
    tvision
    efsw
)

# Target 'scintilla'
file(GLOB_RECURSE SCINTILLA_SRC "${CMAKE_CURRENT_LIST_DIR}/source/scintilla/*.cxx")
add_library(scintilla
    ${SCINTILLA_SRC}
)
target_compile_features(scintilla PRIVATE cxx_std_17)
# turbo_set_warnings(scintilla)
target_include_directories(scintilla PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include/scintilla>"
    "$<INSTALL_INTERFACE:include/scintilla>"
)
target_include_directories(scintilla PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/source/scintilla"
)

# Target 'turbo'

file(GLOB_RECURSE TURBO_SRC "${CMAKE_CURRENT_LIST_DIR}/source/turbo/*.cpp")
add_executable(turbo ${TURBO_SRC})
target_compile_features(turbo PRIVATE cxx_std_20)
turbo_set_warnings(turbo)
target_link_libraries(turbo PRIVATE
    turbo-core
    scintilla
)
