# apps/CMakeLists.txt
# Thin forwarding file — allows add_subdirectory(apps) from root

if(BUILD_APPS)
    add_subdirectory(catmsg)
endif()