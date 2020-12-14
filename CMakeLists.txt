cmake_minimum_required(VERSION 3.17)

project(ray_tracing_one_week C)

set(CMAKE_C_STANDARD 11)

include(CheckLibraryExists)

CHECK_LIBRARY_EXISTS(m cos "" HAVE_LIB_M)
if (HAVE_LIB_M)
    set(EXTRA_LIBS ${EXTRA_LIBS} m)
endif ()

add_executable(ray_tracing_one_week rt_camera.c rt_colour.c rt_aabb.c rt_perlin.c rt_skybox_simple.c
               # Materials
               materials/rt_material.c rt_hit.c materials/rt_material_diffuse.c materials/rt_material_metal.c
               materials/rt_material_dielectric.c materials/rt_material_diffuse_light.c materials/rt_material_isotropic.c
               # Hittables
               hittables/rt_hittable.c hittables/rt_sphere.c hittables/rt_hittable_list.c main.c hittables/rt_moving_sphere.c
               hittables/rt_bvh.c hittables/rt_aa_rect.c hittables/rt_box.c hittables/rt_instance.c hittables/rt_const_medium.c
               # Textures
               textures/rt_texture.c textures/rt_texture_solid_colour.c textures/rt_texture_checker_pattern.c
               textures/rt_texture_noise.c textures/rt_texture_image.c
               # Scenets
               scenes/rt_scenes.c)

target_compile_options(ray_tracing_one_week PRIVATE $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
                       -Wall -Wextra -Winline>
                       $<$<CXX_COMPILER_ID:MSVC>:
                       /WX /W4>)

if (CMAKE_BUILD_TYPE EQUAL "DEBUG")
    # Enable inline optimization under debug configurations.
    target_compile_options(ray_tracing_one_week PRIVATE $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
                           -finline-funcitons>
                           $<$<CXX_COMPILER_ID:MSVC>:
                           /Ob>)
endif ()

# Copy assets into a build directory
add_custom_command(TARGET ray_tracing_one_week PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:ray_tracing_one_week>/assets)


target_include_directories(ray_tracing_one_week PRIVATE ./ materials hittables textures deps)
target_link_libraries(ray_tracing_one_week ${EXTRA_LIBS})