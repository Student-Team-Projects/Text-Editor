cmake_minimum_required (VERSION 3.16)

project (text-editor)

set(TURBO_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})

include(GNUInstallDirs)

function(set_warnings t)
    target_compile_options(${t} PRIVATE
        -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wcast-qual -Wformat=2 -Wundef -g -D_GLIBCXX_DEBUG
    )
endfunction()

# Target 'scintilla'

# These two could be built in one single target, but we want to enable Unity Build
# for 'scintilla' and precompiled headers for 'scilexers'.

file(GLOB_RECURSE SCINTILLA_SRC
    "${CMAKE_CURRENT_LIST_DIR}/source/scintilla/src/*.cxx"
    "${CMAKE_CURRENT_LIST_DIR}/source/scintilla/lexlib/*.cxx"
)

list(APPEND SCILEXERS_SRC
    "${CMAKE_CURRENT_LIST_DIR}/source/scintilla/lexers/LexAsm.cxx"
    "${CMAKE_CURRENT_LIST_DIR}/source/scintilla/lexers/LexBash.cxx"
    "${CMAKE_CURRENT_LIST_DIR}/source/scintilla/lexers/LexCPP.cxx"
    "${CMAKE_CURRENT_LIST_DIR}/source/scintilla/lexers/LexJSON.cxx"
    "${CMAKE_CURRENT_LIST_DIR}/source/scintilla/lexers/LexMake.cxx"
    "${CMAKE_CURRENT_LIST_DIR}/source/scintilla/lexers/LexPython.cxx"
    "${CMAKE_CURRENT_LIST_DIR}/source/scintilla/lexers/LexRuby.cxx"
    "${CMAKE_CURRENT_LIST_DIR}/source/scintilla/lexers/LexRust.cxx"
    "${CMAKE_CURRENT_LIST_DIR}/source/scintilla/lexers/LexYAML.cxx"
    "${CMAKE_CURRENT_LIST_DIR}/source/scintilla/lexers/LexHTML.cxx"
    "${CMAKE_CURRENT_LIST_DIR}/source/scintilla/lexers/LexProps.cxx"
    "${CMAKE_CURRENT_LIST_DIR}/source/scintilla/lexers/LexVB.cxx"
    "${CMAKE_CURRENT_LIST_DIR}/source/scintilla/lexers/LexPascal.cxx"
    "${CMAKE_CURRENT_LIST_DIR}/source/scintilla/lexers/LexLaTeX.cxx"
    "${CMAKE_CURRENT_LIST_DIR}/source/scintilla/lexers/LexSQL.cxx"
)

add_library(scintilla OBJECT ${SCINTILLA_SRC})
add_library(scilexers OBJECT ${SCILEXERS_SRC})

function(set_warnings t)
    if (NOT ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC"))
        target_compile_options(${t} PRIVATE
            -Wall
        )
    else()
        target_compile_options(${t} PRIVATE
            /wd4244 # Possible data loss in type conversion
        )
    endif()
endfunction()

foreach(t scintilla scilexers)
    target_compile_features(${t} PUBLIC cxx_std_20)
    set_warnings(${t})
    target_include_directories(${t} PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/include/scintilla/include"
        "${CMAKE_CURRENT_LIST_DIR}/include/scintilla/lexlib"
        "${CMAKE_CURRENT_LIST_DIR}/include/scintilla/src"
    )
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        target_compile_options(${t} PRIVATE
            /wd4514 /wd4166 /wd4068 /wd4996 /wd4250 /wd4267
            /permissive- /Zc:__cplusplus
        )
    endif()
    target_compile_definitions(${t} PRIVATE
        CURSES
        SCI_LEXER
    )
endforeach()



# Dependencies

include(deps/CMakeLists.txt)

# Target 'text-editor-core'

file(GLOB_RECURSE TEXT_EDITOR_CORE_SRC "${CMAKE_CURRENT_LIST_DIR}/source/text-editor-core/*.cpp")
add_library(text-editor-core
    ${TEXT_EDITOR_CORE_SRC}
    $<TARGET_OBJECTS:scintilla>
    $<TARGET_OBJECTS:scilexers>
)

target_compile_features(text-editor-core PRIVATE cxx_std_20)
set_warnings(text-editor-core)
target_include_directories(text-editor-core PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>"
    "$<INSTALL_INTERFACE:include>"
)
target_include_directories(text-editor-core PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/source/text-editor-core"
    "${CMAKE_CURRENT_LIST_DIR}/source/text-editor-core/platform"
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
install(TARGETS text-editor RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})