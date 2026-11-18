# Build setup adapted from NVIDIA's vk_mini_path_tracer (Apache-2.0).
cmake_minimum_required(VERSION 3.5)
project(gpu_tracer)

if(NOT BASE_DIRECTORY)
  find_path(BASE_DIRECTORY
    NAMES nvpro_core/cmake/setup.cmake
    PATHS ${CMAKE_CURRENT_SOURCE_DIR}/external ${CMAKE_CURRENT_SOURCE_DIR}/.. ${CMAKE_CURRENT_SOURCE_DIR}/../.. ${CMAKE_CURRENT_SOURCE_DIR}/../../nvpro-samples
    REQUIRED
    DOC "Directory containing nvpro_core"
    )
endif()
set(OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin CACHE PATH "Directory where outputs will be stored")
include(${BASE_DIRECTORY}/nvpro_core/cmake/setup.cmake)

_add_package_VulkanSDK()
_add_nvpro_core_lib()

set(PROJNAME gpu_tracer)
_add_project_definitions(${PROJNAME})

file(GLOB SOURCE_FILES *.cpp *.hpp *.h)

file(GLOB SHADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.glsl)
set(SHADER_ENTRY ${CMAKE_CURRENT_SOURCE_DIR}/shaders/raytrace.comp.glsl)
set(SHADER_OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/shaders/raytrace.comp.glsl.spv)
add_custom_command(
  OUTPUT ${SHADER_OUTPUT}
  COMMAND ${Vulkan_GLSLANG_VALIDATOR_EXECUTABLE} --target-env vulkan1.2 -g -o ${SHADER_OUTPUT} ${SHADER_ENTRY}
  DEPENDS ${SHADER_FILES}
  )

add_executable(${PROJNAME} ${SOURCE_FILES} ${COMMON_SOURCE_FILES} ${SHADER_OUTPUT})
target_link_libraries(${PROJNAME} ${PLATFORM_LIBRARIES} nvpro_core)
_finalize_target(${PROJNAME})
