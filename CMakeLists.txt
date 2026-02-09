# NxCoreApi003CategoryMessage/CMakeLists.txt 


#[[
CMake reads this file to construct the project build system.

Run the following in a terminal window from the NxCoreApi003CategoryMessage directory to build and install:

# 1. Configure + generate build system (creates/updates the 'build' folder)
rm -rf build && mkdir build
cmake -S . -B build -G Ninja -DCMAKE_VERBOSE_MAKEFILE=ON     # Or skip -G if CMake defaults to Ninja

# Alternative if you want to force Debug/Release or other settings:
# cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_VERBOSE_MAKEFILE=ON

# 2. Build the project (parallel build using all cores)
cmake --build build --parallel

# Or more explicit (same effect):
# cmake --build build -j$(nproc)

# 3. Install (usually needs sudo if installing to /usr or /usr/local)
sudo cmake --install build                        # Installs to /usr/local by default (use -DCMAKE_INSTALL_PREFIX=/path during cmake .. to change)

ls -lR /usr/local/lib/extern                      # should show your nested structure now
sudo ldconfig                                     # To update library cache
ldconfig -p | grep "libnx.so"                     # To verify library cache

If you want a different prefix: cmake .. -DCMAKE_INSTALL_PREFIX=/opt/myproject
For local install (no sudo): cmake .. -DCMAKE_INSTALL_PREFIX=$PWD/install then "cmake --install ."

Expected output from the install step:
sudo cmake --install build
[sudo] password for crymoney: 
-- Install configuration: ""
-- Installing: /usr/local/bin/nxcore-cat-message
-- Set non-toolchain portion of runtime path of "/usr/local/bin/nxcore-cat-message" to ""
-- Installing: /usr/local/lib/libNxCoreApi003CategoryMessage.a
-- Up-to-date: /usr/local/lib/libnx.so
-- Installing: /usr/local/lib/cmake/NxCoreApi003CategoryMessage/NxCoreApi003CategoryMessageTargets.cmake
-- Installing: /usr/local/lib/cmake/NxCoreApi003CategoryMessage/NxCoreApi003CategoryMessageTargets-noconfig.cmake
-- Installing: /usr/local/lib/cmake/NxCoreApi003CategoryMessage/NxCoreApi003CategoryMessageConfigVersion.cmake
crymoney@rick-Precision-3510-A:~/Crymoney/CrymoneyCodeLinux/NxCoreApi003CategoryMessage$ nxcore-cat-message

Executable application is in the build target directory:  build/apps/nxcore-cat-message/nxcore-cat-message
Executable application is installed as:  nxcore-cat-message
]]

cmake_minimum_required(VERSION 3.25)

project(NxCoreApi003CategoryMessage
    VERSION 1.0.0
    DESCRIPTION "C++17 NxCore Category Message Processor (Linux)"
    LANGUAGES CXX
)

# ── Options ────────────────────────────────────────────────────────────────
option(BUILD_SHARED_LIBS        "Build libraries as shared instead of static"   OFF)
option(BUILD_LIBRARY            "Build the core processing library"              ON)
option(BUILD_APPS               "Build applications (nxcore-cat-message etc.)"   ON)
option(BUILD_TESTS              "Build unit tests"                              OFF)
option(ENABLE_NXCORE            "Enable NxCore proprietary support"              ON)

# ── Global settings ────────────────────────────────────────────────────────
set(CMAKE_CXX_STANDARD          17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(GNUInstallDirs)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(FATAL_ERROR "In-source builds disallowed. Use out-of-source build dir.")
endif()

# ── Include custom cmake helpers ───────────────────────────────────────────
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
include(CompilerOptions)

# ── NxCore (proprietary) ───────────────────────────────────────────────────
if(ENABLE_NXCORE)
    set(NXCORE_LIB "${CMAKE_CURRENT_SOURCE_DIR}/extern/nxcore/libnx.so")
    if(NOT EXISTS "${NXCORE_LIB}")
        message(FATAL_ERROR "libnx.so not found at ${NXCORE_LIB}. Place it there or set -DENABLE_NXCORE=OFF.")
    endif()

    add_library(NxCore::nx SHARED IMPORTED GLOBAL)
    set_target_properties(NxCore::nx PROPERTIES
        IMPORTED_LOCATION               "${NXCORE_LIB}"
        INTERFACE_INCLUDE_DIRECTORIES   "${CMAKE_CURRENT_SOURCE_DIR}/extern/nxcore/include"
    )
else()
    add_library(NxCore::nx INTERFACE IMPORTED)
    message(STATUS "NxCore disabled → dummy interface target created.")
endif()

# ── Core library (processing logic) ────────────────────────────────────────
if(BUILD_LIBRARY)
    add_library(${PROJECT_NAME} STATIC)  # or SHARED via option
    add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

    target_sources(${PROJECT_NAME}
        PRIVATE
            src/executableUtils.cpp
            src/nxcaExceptions.cpp
            src/nxcoreCallback.cpp
            src/nxcore_global.cpp
            src/categoryMessageDump.cpp
            src/processNxCoreCategoryMessage.cpp
            src/processNxCoreStatusMessage.cpp
            src/processNxCoreSymbolSpinMessage.cpp
    )

    target_include_directories(${PROJECT_NAME}
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src
            ${CMAKE_CURRENT_SOURCE_DIR}/src/srcinc
    )

    target_link_libraries(${PROJECT_NAME} PRIVATE NxCore::nx)
    target_compile_options(${PROJECT_NAME} PRIVATE ${MY_WARNING_FLAGS})
    target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:NXCORE_DEBUG>)
endif()

# ── Applications ───────────────────────────────────────────────────────────
if(BUILD_APPS)
    set(apps_dir "${CMAKE_CURRENT_SOURCE_DIR}/apps")

    if(NOT EXISTS "${apps_dir}")
        message(STATUS "Creating missing 'apps/' directory")
        file(MAKE_DIRECTORY "${apps_dir}")
        if(NOT EXISTS "${apps_dir}")
            message(WARNING "Failed to create 'apps/' directory — build will likely fail later")
        endif()
    else()
        message(STATUS "Directory 'apps/' already exists")
    endif()

    # Now try to add it (will error if no CMakeLists.txt inside → that's expected)
    add_subdirectory(apps)
endif()

# ── Tests ──────────────────────────────────────────────────────────────────
if(BUILD_TESTS)
    enable_testing()
    message(STATUS "Tests enabled – implement in tests/ when ready.")
endif()

# ── Install ────────────────────────────────────────────────────────────────
if(BUILD_LIBRARY)
    install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}Targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
endif()

if(ENABLE_NXCORE)
    install(IMPORTED_RUNTIME_ARTIFACTS NxCore::nx
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
endif()

install(DIRECTORY include/nxcorecategorymessage/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/nxcorecategorymessage
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)

# Package config
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(EXPORT ${PROJECT_NAME}Targets
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)