cmake_minimum_required(VERSION 4.0)
set(CMAKE_CXX_STANDARD 14)

set(
    PROJECT
    DATA_STRUCTURES_AND_ALGORITHM_ANALYSIS
)

project (${PROJECT})

# determine architecture
if(NOT ${CUSTOM_ARCH} MATCHES "x32" AND NOT ${CUSTOM_ARCH} MATCHES "x64")
    set(
        CUSTOM_ARCH
        x32
    )
endif()

# Ouput - path
# All binaries are located in the bin - folder
set(
    EXECUTABLE_OUTPUT_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/bin
)
set(
    LIBRARY_OUTPUT_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/bin
)

include_directories(
    # project directories
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

link_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/bin
)

set(
    HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/concept/chronium/logging.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/book/data-structures-and-algorithm-analysis/chapter1/class-template.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/book/data-structures-and-algorithm-analysis/chapter1fFunction-object.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/book/data-structures-and-algorithm-analysis/chapter1/function-template.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/book/data-structures-and-algorithm-analysis/chapter1/int-cell.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/book/data-structures-and-algorithm-analysis/chapter1/matrix.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/book/data-structures-and-algorithm-analysis/chapter1/recursion.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/book/data-structures-and-algorithm-analysis/chapter1/square.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/book/data-structures-and-algorithm-analysis/chapter2/binary-search.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/book/data-structures-and-algorithm-analysis/chapter2/exponentiation.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/book/data-structures-and-algorithm-analysis/chapter2/greatest-common-divisor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/book/data-structures-and-algorithm-analysis/chapter2/maximum-subsequence.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/book/data-structures-and-algorithm-analysis/chapter3/container.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/book/data-structures-and-algorithm-analysis/chapter3/linked-list-stack.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/book/data-structures-and-algorithm-analysis/chapter3/list.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/book/data-structures-and-algorithm-analysis/chapter3/vector.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/book/data-structures-and-algorithm-analysis/chapter3/vector-stack.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/book/effective-modern-cpp/chapter1/template-type-deduction.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/experiment/switch0.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/experiment/token-pasting-operator.h
)

set(
    SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/book/data-structures-and-algorithm-analysis/chapter1/int-cell.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/book/data-structures-and-algorithm-analysis/chapter1/recursion.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/book/data-structures-and-algorithm-analysis/chapter1/square.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/book/data-structures-and-algorithm-analysis/chapter2/maximum-subsequence.cpp
)

add_executable(
    ${PROJECT}
    ${SOURCES}
)

target_link_libraries(
    ${PROJECT}
)

add_dependencies(
    ${PROJECT}
)

set(
    SHAREDSUFFIX
    so
)
if(${CMAKE_SYSTEM} MATCHES "Darwin")
    set(
        SHAREDSUFFIX
        dylib
    )
endif()

if(${CMAKE_SYSTEM} MATCHES "Linux")
    SET(CMAKE_CXX_FLAGS "--std=c++11 ${CMAKE_CXX_FLAGS}")
    set_target_properties(
        ${PROJECT}
        PROPERTIES
            OUTPUT_NAME 
                "test"
            LINK_FLAGS
                -Wl,-rpath,.
    )
elseif(${CMAKE_SYSTEM} MATCHES "Darwin")
    SET(CMAKE_CXX_FLAGS "--stdlib=libc++ --std=c++11 ${CMAKE_CXX_FLAGS}")
    set_target_properties(
        ${PROJECT}
        PROPERTIES
            OUTPUT_NAME 
                "test"
            BUILD_WITH_INSTALL_RPATH
                1
            LINK_FLAGS
                -Wl,-rpath,@loader_path/
    )

    add_custom_command(
        TARGET
            ${PROJECT}
        WORKING_DIRECTORY
            ${EXECUTABLE_OUTPUT_PATH}
        VERBATIM
    )
endif()
