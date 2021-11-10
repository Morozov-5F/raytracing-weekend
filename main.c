/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <stdio.h>

#include "hittables/rt_hittable_list.h"
#include "rt_camera.h"
#include "rt_skybox_simple.h"
#include <errno.h>
#include <string.h>
#include <scenes/rt_scenes.h>
#include <rt_thread_pool.h>
#include <assert.h>
#include <rt_sync.h>

typedef colour_t (*render_fn_t)(const ray_t *ray, const rt_hittable_list_t *list, const rt_skybox_t *skybox, int child_rays);

typedef struct worker_arg_s
{
    struct
    {
        vec3_t *pixels;
        int width;
        int height;
    } image;

    struct
    {
        int x_start;
        int y_start;

        int width;
        int height;
    } chunk;

    struct
    {
        int child_rays;
        long number_of_samples;
        render_fn_t render_function;
    } rendering;

    struct
    {
        const rt_hittable_list_t *world;
        const rt_skybox_t *skybox;
        const rt_camera_t *camera;
    } scene;

    struct
    {
        rt_mutex_t *process_mutex;
        int total_chunks;
        int *processed_chunks;
    } progress;
} worker_arg_t;

static void render_worker(void *args);
static void render_worker_complete(int status, void *args);
static void render_chunk(vec3_t *image, int image_width, int image_height, long number_of_samples, int child_rays,
                         int column_start, int width, int row_start, int height, const rt_hittable_list_t *world,
                         const rt_skybox_t *skybox, const rt_camera_t *camera, render_fn_t render_function);

static void show_usage(const char *program_name, int err);

static colour_t ray_colour(const ray_t *ray, const rt_hittable_list_t *list, const rt_skybox_t *skybox, int child_rays)
{
    if (child_rays <= 0)
    {
        return colour(0, 0, 0);
    }

    rt_hit_record_t record;
    if (!rt_hittable_list_hit_test(list, ray, 0.001, INFINITY, &record))
    {
        return rt_skybox_value(skybox, ray);
    }

    ray_t scattered;
    colour_t attenuation;
    colour_t emitted = rt_material_emit(record.material, record.u, record.v, &record.p);
    if (rt_material_scatter(record.material, ray, &record, &attenuation, &scattered))
    {
        return vec3_sum(emitted, vec3_multiply(attenuation, ray_colour(&scattered, list, skybox, child_rays - 1)));
    }
    return emitted;
}

static colour_t ray_color_iterative(const ray_t *ray, const rt_hittable_list_t *list, const rt_skybox_t *skybox, int child_rays)
{
    colour_t resulting_color = colour(0, 0, 0);
    colour_t global_attenuation = colour(1, 1, 1);

    ray_t current_ray = *ray;

    for (int ray_hop = 0; ray_hop < child_rays; ray_hop++)
    {
        rt_hit_record_t record;
        if (!rt_hittable_list_hit_test(list, &current_ray, 0.001, INFINITY, &record))
        {
            vec3_add(&resulting_color, vec3_multiply(rt_skybox_value(skybox, &current_ray), global_attenuation));
            break;
        }

        colour_t emitted = rt_material_emit(record.material, record.u, record.v, &record.p);
        vec3_add(&resulting_color, vec3_multiply(emitted, global_attenuation));

        ray_t scattered;
        colour_t attenuation;
        bool ray_scattered = rt_material_scatter(record.material, &current_ray, &record, &attenuation, &scattered);
        global_attenuation = vec3_multiply(attenuation, global_attenuation);
        if (!ray_scattered)
        {
            break;
        }
        current_ray = scattered;
    }
    return resulting_color;
}

int main(int argc, char const *argv[])
{
    const char *number_of_samples_str = NULL;
    const char *scene_id_str = NULL;
    const char *file_name = NULL;
    const char *number_of_threads_str = NULL;
    const char *image_width_str = NULL;
    const char *image_height_str = NULL;
    bool verbose = false;
    bool render_recursive = true;

    // Parse console arguments
    for (int i = 1; i < argc; ++i)
    {
        if (0 == strcmp(argv[i], "-s") || 0 == strcmp(argv[i], "--samples"))
        {
            if (i + 1 >= argc)
            {
                fprintf(stderr, "Fatal error: Argument '%s' doesn't have a value\n", argv[i]);
                show_usage(argv[0], EXIT_FAILURE);
            }
            number_of_samples_str = argv[++i];
            continue;
        }
        else if (0 == strcmp(argv[i], "--scene"))
        {
            if (i + 1 >= argc)
            {
                fprintf(stderr, "Fatal error: Argument '%s' doesn't have a value\n", argv[i]);
                show_usage(argv[0], EXIT_FAILURE);
            }
            scene_id_str = argv[++i];
            continue;
        }
        else if (0 == strcmp(argv[i], "-v") || 0 == strcmp(argv[i], "--verbose"))
        {
            verbose = true;
            continue;
        }
        else if (0 == strcmp(argv[i], "-t") || 0 == strcmp(argv[i], "--threads"))
        {
            if (i + 1 >= argc)
            {
                fprintf(stderr, "Fatal error: Argument '%s' doesn't have a value\n", argv[i]);
                show_usage(argv[0], EXIT_FAILURE);
            }
            number_of_threads_str = argv[++i];
            continue;
        }
        else if (0 == strcmp(argv[i], "--width"))
        {
            if (i + 1 >= argc)
            {
                fprintf(stderr, "Fatal error: Argument '%s' doesn't have a value\n", argv[i]);
                show_usage(argv[0], EXIT_FAILURE);
            }
            image_width_str = argv[++i];
            continue;
        }
        else if (0 == strcmp(argv[i], "--height"))
        {
            if (i + 1 >= argc)
            {
                fprintf(stderr, "Fatal error: Argument '%s' doesn't have a value\n", argv[i]);
                show_usage(argv[0], EXIT_FAILURE);
            }
            image_height_str = argv[++i];
            continue;
        }
        else if (0 == strcmp(argv[i], "-h"))
        {
            show_usage(argv[0], EXIT_SUCCESS);
        }
        else if (0 == strcmp(argv[i], "-i"))
        {
            render_recursive = false;
            continue;
        }
        else if ('-' == *argv[i])
        {
            fprintf(stderr, "Fatal error: Unknown argument '%s'\n", argv[i]);
            show_usage(argv[0], EXIT_FAILURE);
        }
        else if (i + 1 < argc)
        {
            fprintf(stderr, "Fatal error: Too many positional arguments (1 expected)\n");
            show_usage(argv[0], EXIT_FAILURE);
        }
        file_name = argv[i];
    }

    if (verbose)
    {
        fprintf(stderr, "Non-parsed parameters:\n");
        fprintf(stderr, "\t- number of samples: %s\n", number_of_samples_str);
        fprintf(stderr, "\t- scene ID:          %s\n", scene_id_str);
        fprintf(stderr, "\t- number of threads: %s\n", number_of_threads_str);
        fprintf(stderr, "\t- image width:       %s\n", image_width_str);
        fprintf(stderr, "\t- image height:      %s\n", image_width_str);
        fprintf(stderr, "\t- file_name:         %s\n", file_name);
        fprintf(stderr, "\t- renderer:          %s\n", render_recursive ? "recursive" : "iterative");
    }

    // Parse resulting parameters
    long number_of_samples = 1000;
    if (NULL != number_of_samples_str)
    {
        char *end_ptr = NULL;
        number_of_samples = strtol(number_of_samples_str, &end_ptr, 10);
        if (*end_ptr != '\0')
        {
            fprintf(stderr, "Fatal error: Value of 'samples' is not a correct number\n");
            show_usage(argv[0], EXIT_FAILURE);
        }
    }
    rt_scene_id_t scene_id = RT_SCENE_SHOWCASE;
    if (NULL != scene_id_str)
    {
        scene_id = rt_scene_get_id_by_name(scene_id_str);
        if (RT_SCENE_NONE == scene_id)
        {
            fprintf(stderr, "Fatal error: Invalid scene identifier\n");
            show_usage(argv[0], EXIT_FAILURE);
        }
    }
    long number_of_threads = rt_sync_get_number_of_cores();
    if (NULL != number_of_threads_str)
    {
        char *end_ptr = NULL;
        number_of_threads = strtol(number_of_threads_str, &end_ptr, 10);
        if (*end_ptr != '\0')
        {
            fprintf(stderr, "Fatal error: Value of 'threads' is not a correct number\n");
            show_usage(argv[0], EXIT_FAILURE);
        }
    }
    int image_width = 320;
    if (NULL != image_width_str)
    {
        if (NULL == image_height_str)
        {
            fprintf(stderr, "Missing --height parameter. Image width should always be specified with height\n");
            show_usage(argv[0], EXIT_FAILURE);
        }
        char *end_ptr = NULL;
        image_width = (int)strtol(image_width_str, &end_ptr, 10);
        if (*end_ptr != '\0')
        {
            fprintf(stderr, "Fatal error: Value of 'width' is not a correct number\n");
            show_usage(argv[0], EXIT_FAILURE);
        }
    }
    int image_height = 320;
    if (NULL != image_height_str)
    {
        if (NULL == image_width_str)
        {
            fprintf(stderr, "Missing --width parameter. Image height should always be specified with width\n");
            show_usage(argv[0], EXIT_FAILURE);
        }
        char *end_ptr = NULL;
        image_height = (int)strtol(image_height_str, &end_ptr, 10);
        if (*end_ptr != '\0')
        {
            fprintf(stderr, "Fatal error: Value of 'height' is not a correct number\n");
            show_usage(argv[0], EXIT_FAILURE);
        }
    }
    render_fn_t render_function = ray_colour;
    if (!render_recursive)
    {
        render_function = ray_color_iterative;
    }

    if (verbose)
    {
        fprintf(stderr, "Parsed parameters:\n");
        fprintf(stderr, "\t- number of samples: %ld\n", number_of_samples);
        fprintf(stderr, "\t- scene ID:          %d\n", scene_id);
        fprintf(stderr, "\t- number of threads: %ld\n", number_of_threads);
        fprintf(stderr, "\t- image width:       %d\n", image_width);
        fprintf(stderr, "\t- image height:      %d\n", image_height);
        fprintf(stderr, "\t- file_name:         %s\n", file_name);
        fprintf(stderr, "\t- render function:   %s\n", render_function == ray_colour ? "ray_colour" : "ray_colou_iterative");
    }

    // Image parameters
    double ASPECT_RATIO = (double)image_width / image_height;
    const int CHILD_RAYS = 50;
    const int CHUNK_SIZE = 32;

    // Declare Camera parameters
    point3_t look_from, look_at;
    vec3_t up = point3(0, 1, 0);
    double focus_distance = 10.0, aperture = 0.0, vertical_fov = 40.0;

    // World
    rt_hittable_list_t *world = NULL;
    rt_skybox_t *skybox = NULL;

    // Select a scene from a pre-defined one
    switch (scene_id)
    {
        case RT_SCENE_RANDOM:
            look_from = point3(13, 2, 3);
            look_at = point3(0, 0, 0);
            aperture = 0.1;
            vertical_fov = 20.0;

            skybox = rt_skybox_new_gradient(colour(1, 1, 1), colour(0.5, 0.7, 1));
            world = rt_scene_random();
            break;

        case RT_SCENE_TWO_SPHERES:
            look_from = point3(13, 2, 3);
            look_at = point3(0, 0, 0);
            vertical_fov = 20.0;

            skybox = rt_skybox_new_gradient(colour(1, 1, 1), colour(0.5, 0.7, 1));
            world = rt_scene_two_spheres();
            break;

        case RT_SCENE_TWO_PERLIN_SPHERES:
            look_from = point3(13, 2, 3);
            look_at = point3(0, 0, 0);
            vertical_fov = 20.0;

            skybox = rt_skybox_new_gradient(colour(1, 1, 1), colour(0.5, 0.7, 1));
            world = rt_scene_two_perlin_spheres();
            break;

        case RT_SCENE_EARTH:
            look_from = point3(13, 2, 3);
            look_at = point3(0, 0, 0);
            vertical_fov = 20.0;

            skybox = rt_skybox_new_gradient(colour(1, 1, 1), colour(0.5, 0.7, 1));
            world = rt_scene_earth();
            break;

        case RT_SCENE_LIGHT_SAMPLE:
            look_from = point3(26, 3, 6);
            look_at = point3(0, 2, 0);
            vertical_fov = 20.0;

            skybox = rt_skybox_new_background(colour(0, 0, 0));
            world = rt_scene_light_sample();
            break;

        case RT_SCENE_CORNELL_BOX:
            look_from = point3(278, 278, -800);
            look_at = point3(278, 278, 0);

            skybox = rt_skybox_new_background(colour(0, 0, 0));
            world = rt_scene_cornell_box();
            break;

        case RT_SCENE_INSTANCE_TEST:
            look_from = point3(0, 5, -20);
            look_at = point3(0, 0, 0);
            vertical_fov = 20.0;

            skybox = rt_skybox_new_gradient(colour(1, 1, 1), colour(0.5, 0.7, 1));
            world = rt_scene_instance_test();
            break;

        case RT_SCENE_CORNELL_SMOKE:
            look_from = point3(278, 278, -800);
            look_at = point3(278, 278, 0);

            skybox = rt_skybox_new_background(colour(0, 0, 0));
            world = rt_scene_cornell_box_smoke_boxes();
            break;

        case RT_SCENE_SHOWCASE:
            look_from = point3(478, 278, -600);
            look_at = point3(278, 278, 0);

            skybox = rt_skybox_new_background(colour(0, 0, 0));
            world = rt_scene_showcase();
            break;

        case RT_SCENE_METAL_TEST:
            look_from = point3(0, 5, -10);
            look_at = point3(0, 2, 0);
            vertical_fov = 20.0;

            skybox = rt_skybox_new_gradient(colour(1, 1, 1), colour(0.5, 0.7, 1));
            world = rt_scene_metal_test();
            break;
        case RT_SCENE_NONE:
            fprintf(stderr, "Fatal error: scene id is undefined after parsing the parameters\n");
            return EXIT_FAILURE;
    }

    rt_camera_t *camera =
        rt_camera_new(look_from, look_at, up, vertical_fov, ASPECT_RATIO, aperture, focus_distance, 0.0, 1.0);

    vec3_t *image = calloc(image_height * image_width, sizeof(vec3_t));
    assert(NULL != image);

    FILE *out_file = stdout;
    if (NULL != file_name)
    {
        out_file = fopen(file_name, "w");
        if (NULL == out_file)
        {
            fprintf(stderr, "Fatal error: Unable to open file %s: %s", file_name, strerror(errno));
            goto cleanup;
        }
    }

    // Prepare a thread pool
    rt_thread_pool_t *thread_pool = rt_tp_init(number_of_threads);
    assert(NULL != thread_pool);

    rt_mutex_t *progress_mutex = rt_mutex_init();
    assert(NULL != progress_mutex);

    if (verbose)
    {
        fprintf(stderr, "Rendering started\n");
        fflush(stderr);
    }

    // Distribute workers
    int processed_chunks = 0;
    int number_of_chunks = (int)ceil(image_height / (double)CHUNK_SIZE) * (int)ceil(image_width / (double)CHUNK_SIZE);

    fprintf(stderr, "\rProgress: %d/%d chunks (%3d%%)", 0, number_of_chunks, 0);
    fflush(stderr);

    for (int i = 0; i < ceil(image_height / (double)CHUNK_SIZE); ++i)
    {
        for (int j = 0; j < ceil(image_width / (double)CHUNK_SIZE); ++j)
        {
            worker_arg_t *arg = malloc(sizeof(worker_arg_t));
            assert(NULL != arg);

            arg->scene.camera = camera;
            arg->scene.skybox = skybox;
            arg->scene.world = world;

            arg->rendering.number_of_samples = number_of_samples;
            arg->rendering.child_rays = CHILD_RAYS;
            arg->rendering.render_function = render_function;

            arg->chunk.x_start = j * CHUNK_SIZE;
            arg->chunk.y_start = i * CHUNK_SIZE;
            arg->chunk.width = image_width - j * CHUNK_SIZE < CHUNK_SIZE ? image_width - j * CHUNK_SIZE : CHUNK_SIZE;
            arg->chunk.height = image_height - i * CHUNK_SIZE < CHUNK_SIZE ? image_height - i * CHUNK_SIZE : CHUNK_SIZE;

            arg->image.pixels = image;
            arg->image.width = image_width;
            arg->image.height = image_height;

            arg->progress.process_mutex = progress_mutex;
            arg->progress.total_chunks = number_of_chunks;
            arg->progress.processed_chunks = &processed_chunks;

            rt_tp_schedule_work(thread_pool, render_worker, arg, render_worker_complete);
        }
    }

    // Wait until all the workers finish
    rt_tp_deinit(thread_pool);
    rt_mutex_deinit(progress_mutex);

    if (verbose)
    {
        fprintf(stderr, "\nRendering done, starting output to file\n");
        fflush(stderr);
    }

    // Output image to file
    fprintf(out_file, "P3\n%d %d\n255\n", image_width, image_height);
    for (int j = image_height - 1; j >= 0; --j)
    {
        for (int i = 0; i < image_width; ++i)
        {
            rt_write_colour(out_file, image[j * image_width + i], number_of_samples);
        }
    }
    fprintf(stderr, "\nDone\n");
    fflush(stderr);

cleanup:
    // Cleanup
    rt_hittable_list_deinit(world);
    rt_camera_delete(camera);
    rt_skybox_delete(skybox);
    free(image);

    return EXIT_SUCCESS;
}

static void show_usage(const char *program_name, int err)
{
    // clang-format off
    fprintf(stderr, "Usage:\n");
    fprintf(stderr, "%s [-s|--samples N] [--scene SCENE] [-v|--verbose] [-t|--threads THREADS] [--width WIDTH] [--height HEIGHT] [-i] [output_file_name]\n", program_name);
    fprintf(stderr, "Options:\n");
    fprintf(stderr, "\t-s | --samples      <int>       Number of rays to cast for each pixel\n");
    fprintf(stderr, "\t--scene             <string>    ID of the scene to render. List of available scenes is printed below.\n");
    fprintf(stderr, "\t-t | --threads      <int>       Number of threads to utilize during rendering.\n");
    fprintf(stderr, "\t--width             <int>       Image width in pixels. Should be used with --height option.\n");
    fprintf(stderr, "\t--height            <int>       Image height in pixels. Should be used with --width option.\n");
    fprintf(stderr, "\t-v | --verbose                  Enable verbose output\n");
    fprintf(stderr, "\t-h                              Show this message and exit\n");
    fprintf(stderr, "\t-i                              Use iterative rendering loop instead of recursive\n");
    fprintf(stderr, "Positional arguments:\n");
    fprintf(stderr, "\toutput_file_name                Name of the output file. Outputs image to console if not specified.\n");
    fprintf(stderr, "Available scenes:\n");
    rt_scene_print_scenes_info(stderr);
    // clang-format on

    exit(err);
}

static void render_chunk(vec3_t *image, int image_width, int image_height, long number_of_samples, int child_rays,
                         int column_start, int width, int row_start, int height, const rt_hittable_list_t *world,
                         const rt_skybox_t *skybox, const rt_camera_t *camera, render_fn_t render_function)
{
    for (int j = row_start + height - 1; j >= row_start; --j)
    {
        for (int i = column_start; i < column_start + width; ++i)
        {
            colour_t pixel = colour(0, 0, 0);
            for (int s = 0; s < number_of_samples; ++s)
            {
                double u = (double)(i + rt_random_double(0, 1)) / (image_width - 1);
                double v = (double)(j + rt_random_double(0, 1)) / (image_height - 1);

                ray_t ray = rt_camera_get_ray(camera, u, v);
                vec3_add(&pixel, render_function(&ray, world, skybox, child_rays));
            }
            image[j * image_width + i] = pixel;
        }
    }
}

static void render_worker(void *args)
{
    worker_arg_t *worker_arg = args;
    assert(NULL != worker_arg);

    render_chunk(worker_arg->image.pixels, worker_arg->image.width, worker_arg->image.height,
                 worker_arg->rendering.number_of_samples, worker_arg->rendering.child_rays, worker_arg->chunk.x_start,
                 worker_arg->chunk.width, worker_arg->chunk.y_start, worker_arg->chunk.height, worker_arg->scene.world,
                 worker_arg->scene.skybox, worker_arg->scene.camera, worker_arg->rendering.render_function);
}

static void render_worker_complete(int status, void *args)
{
    worker_arg_t *worker_arg = args;

    rt_mutex_lock(worker_arg->progress.process_mutex);

    int last_processed_chunks = *(worker_arg->progress.processed_chunks);
    *worker_arg->progress.processed_chunks += 1;
    int current_processed_chunks = *(worker_arg->progress.processed_chunks);
    int old_percentage = 100 * last_processed_chunks / worker_arg->progress.total_chunks;
    int current_percentage = 100 * current_processed_chunks / worker_arg->progress.total_chunks;

    if (current_percentage != old_percentage)
    {
        fprintf(stderr, "\rProgress: %d/%d chunks (%3d%%)", current_processed_chunks, worker_arg->progress.total_chunks,
                current_percentage);
        fflush(stderr);
    }

    rt_mutex_unlock(worker_arg->progress.process_mutex);

    free(worker_arg);
}
