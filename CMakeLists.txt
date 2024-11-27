cmake_minimum_required (VERSION 3.16)

option(TURBO_BUILD_APP "Build main editor application" ON)
option(TURBO_OPTIMIZE_BUILD "Use Precompiled Headers and Unity Build for the core library" ON) # messes up clangd

cmake_policy(SET CMP0091 NEW)

project (turbo)

set(TURBO_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})

include(GNUInstallDirs)

# Target 'scintilla'

# These two could be built in one single target, but we want to enable Unity Build
# for 'scintilla' and precompiled headers for 'scilexers'.

file(GLOB_RECURSE SCINTILLA_SRC
    "${CMAKE_CURRENT_LIST_DIR}/deps/scintilla/src/*.cxx"
    "${CMAKE_CURRENT_LIST_DIR}/deps/scintilla/lexlib/*.cxx"
)

list(APPEND SCILEXERS_SRC
    "${CMAKE_CURRENT_LIST_DIR}/deps/scintilla/lexers/LexCPP.cxx"
)

add_library(scintilla OBJECT ${SCINTILLA_SRC})
add_library(scilexers OBJECT ${SCILEXERS_SRC})

function(turbo_set_warnings t)
    target_compile_options(${t} PRIVATE
        -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wcast-qual -Wformat=2 -Wundef -g -D_GLIBCXX_DEBUG
    )
endfunction()

foreach(t scintilla scilexers)
    target_compile_features(${t} PUBLIC cxx_std_20)
    turbo_set_warnings(${t})
    target_include_directories(${t} PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/include/scintilla/include"
        "${CMAKE_CURRENT_LIST_DIR}/include/scintilla/lexlib"
        "${CMAKE_CURRENT_LIST_DIR}/include/scintilla/src"
    )
    target_compile_definitions(${t} PRIVATE
        CURSES
        SCI_LEXER
    )
endforeach()

# Dependencies

include(deps/CMakeLists.txt)

# Target 'turbo-core'

set(TURBO turbo)

file(GLOB_RECURSE TURBO_CORE_SRC "${CMAKE_CURRENT_LIST_DIR}/source/${TURBO}-core/*.cpp")
add_library(${TURBO}-core
    ${TURBO_CORE_SRC}
    $<TARGET_OBJECTS:scintilla>
    $<TARGET_OBJECTS:scilexers>
)

target_compile_features(${TURBO}-core PRIVATE cxx_std_20)
turbo_set_warnings(${TURBO}-core)
target_include_directories(${TURBO}-core PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>"
    "$<INSTALL_INTERFACE:include>"
)
target_include_directories(${TURBO}-core PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/source/${TURBO}-core"
)
target_link_libraries(${TURBO}-core PUBLIC
    tvision
)

install(TARGETS ${TURBO}-core
    EXPORT ${TURBO}-config
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    COMPONENT library
)
install(EXPORT ${TURBO}-config
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${TURBO}
    NAMESPACE ${TURBO}::
    FILE ${TURBO}-config.cmake
    COMPONENT library
)
install(DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/include/turbo" DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Optional dependencies

# find_library(MAGIC magic)
# if (MAGIC)
#     target_link_libraries(${TURBO}-core PRIVATE ${MAGIC})
#     target_compile_definitions(${TURBO}-core PRIVATE HAVE_MAGIC)
# endif()

# Target 'turbo'

if (TURBO_BUILD_APP)
    file(GLOB_RECURSE TURBO_SRC "${CMAKE_CURRENT_LIST_DIR}/source/${TURBO}/*.cpp")
    add_executable(${TURBO} ${TURBO_SRC})
    target_compile_features(${TURBO} PRIVATE cxx_std_20)
    turbo_set_warnings(${TURBO})
    target_link_libraries(${TURBO} PRIVATE
        ${TURBO}-core
    )
    install(TARGETS ${TURBO} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()

# if (TURBO_OPTIMIZE_BUILD)
#     message(STATUS "Using Precompiled Headers and Unity Build")
#     set_target_properties(${TURBO}-core PROPERTIES UNITY_BUILD ON)
#     target_precompile_headers(${TURBO}-core PRIVATE
#         "${CMAKE_CURRENT_LIST_DIR}/include/TurboPCH.hpp"
#     )
#     # set_target_properties(scintilla PROPERTIES UNITY_BUILD ON)
#     # target_precompile_headers(scilexers PRIVATE
#     #     "${CMAKE_CURRENT_LIST_DIR}/include/ScilexersPCH.hpp"
#     # )
#     if (TURBO_BUILD_APP)
#         set_target_properties(${TURBO} PROPERTIES UNITY_BUILD ON)
#     endif()
# endif()