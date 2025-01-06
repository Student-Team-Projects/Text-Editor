set(TV_OPTIMIZE_BUILD ON)
set(TV_BUILD_EXAMPLES OFF)
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/tvision" EXCLUDE_FROM_ALL)

set(tvisoin_DIR "${CMAKE_INSTALL_PREFIX}/tvision" CACHE PATH "Path to tvision installation")

set(BUILD_SHARED_LIBS OFF)
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/efsw" EXCLUDE_FROM_ALL)

set(TVTERM_USE_SYSTEM_TVISION OFF CACHE BOOL "use project tvision" FORCE)
set(TVTERM_USE_SYSTEM_LIBVTERM ON CACHE BOOL "use pacman libvterm" FORCE)
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/tvterm" EXCLUDE_FROM_ALL)
