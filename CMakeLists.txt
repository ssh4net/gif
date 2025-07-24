cmake_minimum_required(VERSION 3.15)
project(GIFLIB VERSION 5.2.1 LANGUAGES C)

# Options
option(BUILD_SHARED_LIBS "Build shared libraries" ON)
option(BUILD_UTILS "Build utility programs" ON)

# Set MSVC specific flags and runtime
if(MSVC)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W4")
    
    # Set MSVC runtime
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
    
    # Ensure proper DLL export/import
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

# Set debug postfix
set(CMAKE_DEBUG_POSTFIX "d")

# Set output directories
if(MSVC)
    # For MSVC, we want Debug and Release in separate directories
    foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
        string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/bin/${OUTPUTCONFIG})
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/lib/${OUTPUTCONFIG})
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/lib/${OUTPUTCONFIG})
    endforeach()
else()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
endif()

# Core library source files
set(GIFLIB_SOURCES
    dgif_lib.c
    egif_lib.c
    gif_err.c
    gif_hash.c
    gifalloc.c
    openbsd-reallocarray.c
    quantize.c
)

# Create the main library
add_library(gif ${GIFLIB_SOURCES})
add_library(GIFLIB::gif ALIAS gif)

if(MSVC AND BUILD_SHARED_LIBS)
    # Add export definitions for Windows DLL
    target_compile_definitions(gif
        PRIVATE -DGIFLIB_EXPORTS
        PUBLIC -DGIFLIB_DLL
    )
endif()

# Set library properties
set_target_properties(gif PROPERTIES
    DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}"
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    OUTPUT_NAME "gif"
)

target_include_directories(gif
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)

# Utility programs common source files
set(UTILS_COMMON_SOURCES
    getarg.c
    getopt.c
    qprintf.c
    gif_font.c
)

# Define utility programs
set(UTILS
    gif2rgb
    gifbuild
    gifbg
    gifclrmp
    gifcolor
    gifecho
    giffilter
    giffix
    gifhisto
    gifinto
    gifsponge
    giftext
    giftool
    gifwedge
)

# Build utilities if enabled
if(BUILD_UTILS)
    foreach(UTIL ${UTILS})
        add_executable(${UTIL} ${UTIL}.c ${UTILS_COMMON_SOURCES})
        target_link_libraries(${UTIL} PRIVATE gif)
        
        # Set utility output properties
        set_target_properties(${UTIL} PROPERTIES
            DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}"
            RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}"
            RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}"
            PDB_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
        )
    endforeach()
endif()

# Installation
include(GNUInstallDirs)

install(TARGETS gif
    EXPORT GIFLIBTargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Install headers
install(FILES
    gif_lib.h
    gif_hash.h
    gif_lib_private.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Install utilities
if(BUILD_UTILS)
    install(TARGETS ${UTILS}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
endif()

# Export targets
install(EXPORT GIFLIBTargets
    FILE GIFLIBTargets.cmake
    NAMESPACE GIFLIB::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/GIFLIB
)

# Create and install config files
include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/GIFLIBConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/GIFLIBConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/GIFLIBConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/GIFLIB
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/GIFLIBConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/GIFLIBConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/GIFLIB
) 