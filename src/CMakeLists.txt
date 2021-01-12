cmake_minimum_required(VERSION 3.16.0)

project(libcpnet VERSION 1.0.1 LANGUAGES C)

# Define the library
add_library(cpnet SHARED
    cpnet-export.h
    cpnet-network.h
    cpnet-network.c
)

# Mark all public headers
file(GLOB_RECURSE ALL_HEADER_FILES "*.h")

set_target_properties(cpnet PROPERTIES 
    C_STANDARD 11
    DEBUG_POSTFIX "_d"
    PUBLIC_HEADER "${ALL_HEADER_FILES}"
)

target_include_directories(cpnet
    PUBLIC
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

if(WIN32)
target_link_libraries(cpnet
    PUBLIC wsock32 ws2_32
)
endif()

# Installation

include(GNUInstallDirs)

export(TARGETS cpnet FILE "${PROJECT_NAME}Targets.cmake")

install(TARGETS cpnet
    EXPORT "${PROJECT_NAME}Targets"
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/cpnet
)
