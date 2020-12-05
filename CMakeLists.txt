cmake_minimum_required(VERSION 3.17)

project(ray_tracing_one_week C)

set(CMAKE_C_STANDARD 11)

include(CheckLibraryExists)

CHECK_LIBRARY_EXISTS(m cos "" HAVE_LIB_M)
if (HAVE_LIB_M)
    set(EXTRA_LIBS ${EXTRA_LIBS} m)
endif ()

add_executable(ray_tracing_one_week hittables/rt_hittable.c hittables/rt_sphere.c hittables/rt_hittable_list.c main.c rt_camera.c rt_colour.c materials/rt_material.c rt_hit.c materials/rt_material_diffuse.c materials/rt_material_metal.c materials/rt_material_dielectric.c hittables/rt_moving_sphere.c hittables/rt_moving_sphere.h rt_aabb.c rt_aabb.h hittables/rt_bvh.c hittables/rt_bvh.h)

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
endif()

target_include_directories(ray_tracing_one_week PRIVATE ./ materials hittables)
target_link_libraries(ray_tracing_one_week ${EXTRA_LIBS})