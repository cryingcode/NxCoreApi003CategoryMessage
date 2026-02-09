# apps/nxcore-cat-message/CMakeLists.txt

add_executable(nxcore-cat-message
    main.cpp
)

target_link_libraries(nxcore-cat-message
    PRIVATE
        NxCoreApi003CategoryMessage
        NxCore::nx
)

target_include_directories(nxcore-cat-message
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/src/srcinc
        ${CMAKE_SOURCE_DIR}/include
)

target_compile_features(nxcore-cat-message PRIVATE cxx_std_17)

install(TARGETS nxcore-cat-message
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)