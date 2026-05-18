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
    ${CMAKE_CURRENT_SOURCE_DIR}/src/DataStructuresAndAlgorithmAnalysis/chapter1/ClassTemplate.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/DataStructuresAndAlgorithmAnalysis/chapter1/FunctionObject.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/DataStructuresAndAlgorithmAnalysis/chapter1/FunctionTemplate.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/DataStructuresAndAlgorithmAnalysis/chapter1/IntCell.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/DataStructuresAndAlgorithmAnalysis/chapter1/Matrix.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/DataStructuresAndAlgorithmAnalysis/chapter1/Recursion.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/DataStructuresAndAlgorithmAnalysis/chapter1/Square.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/DataStructuresAndAlgorithmAnalysis/chapter2/BinarySearch.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/DataStructuresAndAlgorithmAnalysis/chapter2/Exponentiation.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/DataStructuresAndAlgorithmAnalysis/chapter2/GreatestCommonDivisor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/DataStructuresAndAlgorithmAnalysis/chapter2/MaximumSubsequence.h
)

set(
    SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/DataStructuresAndAlgorithmAnalysis/chapter1/IntCell.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/DataStructuresAndAlgorithmAnalysis/chapter1/Recursion.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/DataStructuresAndAlgorithmAnalysis/chapter1/Square.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/DataStructuresAndAlgorithmAnalysis/chapter2/MaximumSubsequence.cpp
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
