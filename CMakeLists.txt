cmake_minimum_required(VERSION 3.17)

project(ray_tracing_one_week C)

set(CMAKE_C_STANDARD 11)

include(CheckLibraryExists)
include(CheckIncludeFile)
include(CheckSymbolExists)

option(ENABLE_GUI "Enable Native OS GUI support" ON)

CHECK_LIBRARY_EXISTS(m ceil "" HAVE_LIB_M)
if (HAVE_LIB_M)
    set(EXTRA_LIBS ${EXTRA_LIBS} m)
endif ()

set(THREADING_IMPLEMENTATION "None")

find_package(Threads REQUIRED)
if (CMAKE_USE_PTHREADS_INIT)
    set(THREADING_IMPLEMENTATION threads/rt_sync_unix.c threads/rt_thread_unix.c)
elseif (CMAKE_USE_WIN32_THREADS_INIT)
    set(THREADING_IMPLEMENTATION threads/rt_sync_win32.c threads/rt_thread_win32.c)
else ()
    message(FATAL_ERROR "The System does not contain the supported threading library (pthread or windows threads)")
endif ()

set(OS_WINDOW_IMPLEMENTATION "window/rt_window_noop.c")

if (ENABLE_GUI)
    if (WIN32)
        # TODO: properly verify that there is actually a windows library available
        set(OS_WINDOW_IMPLEMENTATION "window/rt_window_win32.c")
    else()
        # TODO: windows implementation for other platforms
        message(WARNING "GUI is not available for this platform yet")
    endif()
endif()

set(OS_MAIN_IMPLEMENTATION "main/rt_main_${MAIN_IMPLEMENTATION}.c")

add_executable(ray_tracing_one_week ${OS_APP_TYPE} rt_camera.c rt_colour.c rt_aabb.c rt_perlin.c rt_skybox_simple.c
               # Materials
               materials/rt_material.c rt_hit.c materials/rt_material_diffuse.c materials/rt_material_metal.c
               materials/rt_material_dielectric.c materials/rt_material_diffuse_light.c materials/rt_material_isotropic.c
               # Hittables
               hittables/rt_hittable.c hittables/rt_sphere.c hittables/rt_hittable_list.c main.c hittables/rt_moving_sphere.c
               hittables/rt_bvh.c hittables/rt_aa_rect.c hittables/rt_box.c hittables/rt_instance.c hittables/rt_const_medium.c
               # Textures
               textures/rt_texture.c textures/rt_texture_solid_colour.c textures/rt_texture_checker_pattern.c
               textures/rt_texture_noise.c textures/rt_texture_image.c
               # Scenes
               scenes/rt_scenes.c
               # Random number generation
               random/rt_random.c random/rt_random.h
               # Threading
               ${THREADING_IMPLEMENTATION} threads/rt_thread_pool.c
               # Window management
               ${OS_WINDOW_IMPLEMENTATION})

target_compile_options(ray_tracing_one_week PRIVATE $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
                       -Wall -Wextra -Winline>
                       $<$<CXX_COMPILER_ID:MSVC>:
                       /WX /W4>)

if (WIN32)
    target_compile_definitions(ray_tracing_one_week PRIVATE WIN32_LEAN_AND_MEAN)
endif ()

if(MSVC)
    target_compile_options(ray_tracing_one_week PRIVATE /experimental:c11atomics)
endif()

if (CMAKE_BUILD_TYPE EQUAL "DEBUG")
    # Enable inline optimization under debug configurations.
    target_compile_options(ray_tracing_one_week PRIVATE $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
                           -finline-functions>
                           $<$<CXX_COMPILER_ID:MSVC>:
                           /Ob>)
endif ()

# Copy assets into a build directory
add_custom_command(TARGET ray_tracing_one_week PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:ray_tracing_one_week>/assets)

target_include_directories(ray_tracing_one_week PRIVATE ./ materials hittables textures deps threads window main)
target_link_libraries(ray_tracing_one_week ${EXTRA_LIBS} Threads::Threads)

set_target_properties(ray_tracing_one_week
                      PROPERTIES
                      ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
                      LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
                      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# Define the install targets
install(TARGETS ray_tracing_one_week RUNTIME DESTINATION bin)
install(DIRECTORY assets DESTINATION bin)