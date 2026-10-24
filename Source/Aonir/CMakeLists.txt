add_subdirectory(Core)
add_subdirectory(Platform)

if(BUILD_VULKAN_RENDERER)
    add_subdirectory(Vulkan)
endif()
