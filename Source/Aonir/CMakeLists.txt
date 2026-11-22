add_subdirectory(Core)
add_subdirectory(Platform/${AONIR_PLATFORM})

if(BUILD_VULKAN_RENDERER)
    add_subdirectory(Vulkan)
endif()
