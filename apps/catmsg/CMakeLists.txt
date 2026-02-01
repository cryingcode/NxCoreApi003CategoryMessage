add_executable(catmsg main.cpp)

target_include_directories(catmsg
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/src/srcinc     # or just src/srcinc if you prefer
)

if(BUILD_LIBRARY)
    target_link_libraries(catmsg PRIVATE NxCoreApi003CategoryMessage::NxCoreApi003CategoryMessage)
else()
    target_sources(catmsg PRIVATE
        ${CMAKE_SOURCE_DIR}/src/executableUtils.cpp
        # ... list remaining .cpp files ...
    )
endif()

if(ENABLE_NXCORE)
    target_link_libraries(catmsg PRIVATE NxCore::nx)
endif()

target_compile_options(catmsg PRIVATE ${MY_WARNING_FLAGS})

set_target_properties(catmsg PROPERTIES
    BUILD_RPATH   "${CMAKE_SOURCE_DIR}/extern/nxcore"
    INSTALL_RPATH "$ORIGIN/../${CMAKE_INSTALL_LIBDIR}"
    INSTALL_RPATH_USE_LINK_PATH ON
)

install(TARGETS catmsg
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)