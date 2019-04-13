# Defines the library 'ylog' for integration with other projects:
#
# add_subdirectory(path_to_lib/ylog)     # add 'tested' library to project
# target_link_libraries(mylib_test ylog) # adds dependency and include path
set(CUR_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

# collect sources
file(GLOB YLOG_SRC   RELATIVE ${CUR_DIR} "*.h" "impl/*.cpp" "impl/*.h")
list(SORT YLOG_SRC)

# build bathyscaphe static library
add_library(ylog STATIC ${YLOG_SRC})

target_include_directories(ylog PUBLIC ${CUR_DIR})
