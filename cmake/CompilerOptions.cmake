set(MY_WARNING_FLAGS
    -Wall -Wextra -Wpedantic -Wshadow -Wnon-virtual-dtor
    -Wold-style-cast -Wcast-align -Wunused -Woverloaded-virtual
    -Wconversion -Wsign-conversion
)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    list(APPEND MY_WARNING_FLAGS -Wmisleading-indentation -Wduplicated-cond)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    list(APPEND MY_WARNING_FLAGS -Werror)
endif()