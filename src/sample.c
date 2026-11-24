#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <math.h>
#include "nl_math.h"
#include "window.h"
#include "renderer.h"
#include "input.h"
#include "shader.h"
#include "texture.h"

#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500
#define WINDOW_TITLE "nl Sample"

#define NL_SAMPLE_LOAD_SHADER(SH, VERT, FRAG) \
    SH = nl_shader_from_file(VERT, FRAG); \
    if (SH == NULL) {\
        printf("ERROR::MAIN unable to load shader with vertex: %s, fragment: %s\n", VERT, FRAG); \
        nl_renderer_free(r);\
        nl_window_free(win);\
        return 1;\
    }

int main() {
    nl_window *win = nl_window_new(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
    if (win == NULL) {
        printf("ERROR::MAIN unable to initialize window");
        return 1;
    }

    nl_renderer *r = nl_renderer_new();
    if (r == NULL) {
        if (win != NULL)
            nl_window_free(win);
        printf("ERROR::MAIN unable to initialize renderer");
        return 1;
    }

    nl_shader *sh = NULL;
    nl_shader *shader_left_right = NULL;
    nl_shader *shader_up_down = NULL;
    nl_shader *texture_shader = NULL;

    NL_SAMPLE_LOAD_SHADER(sh, "assets/shaders/default.vert", "assets/shaders/default.frag");
    NL_SAMPLE_LOAD_SHADER(shader_left_right, "assets/shaders/left_right.vert", "assets/shaders/left_right.frag");
    NL_SAMPLE_LOAD_SHADER(shader_up_down, "assets/shaders/updown.vert", "assets/shaders/updown.frag");
    NL_SAMPLE_LOAD_SHADER(texture_shader, "assets/shaders/texture.vert", "assets/shaders/texture.frag");

    nl_texture *t1 = NULL;
    nl_texture *t2 = NULL;
    t1 = nl_texture_new("assets/textures/shuttle.png");
    t2 = nl_texture_new("assets/textures/city.jpg");

    while(nl_window_is_running(win)) {
        if (nl_is_key_pressed(win, GLFW_KEY_ESCAPE)) {
            nl_window_stop(win);
        }

        double time = nl_window_time(win);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        nl_renderer_clear_bg((nl_color){0.1f, 0.2f, 0.2f});

        nl_shader_use(shader_left_right);
        nl_shader_uniform_f32(shader_left_right, "u_time", (float)time);
        nl_renderer_fill_color(r, (color){1.0f, 0.0f, 0.0f});
        nl_renderer_draw_line(r, (vec3){0.0f, 0.0f, 0.0f}, (vec3){0.0f, 0.5f, 0.0});
        nl_renderer_draw_line(r, (vec3){0.0f, 0.0f, 0.0f}, (vec3){0.0f, -0.5f, 0.0});


        nl_shader_use(shader_up_down);
        nl_shader_uniform_f32(shader_up_down, "u_time", (float)time);
        nl_renderer_fill_color(r, (color){0.0f, 1.0f, 0.0f});
        nl_renderer_draw_line(r, (vec3){0.0f, 0.0f, 0.0f}, (vec3){0.5f, 0.0f, 0.0});
        nl_renderer_draw_line(r, (vec3){0.0f, 0.0f, 0.0f}, (vec3){-0.5f, 0.0f, 0.0});

        float speed = NL_MATH_TO_RAD(time * 10.0f) * 3.0f;
        speed = speed * 10;

        nl_shader_use(sh);
        nl_renderer_fill_color(r, (color){0.5f, 0.99f, 0.3f});
        nl_renderer_draw_line(r, (vec3){0.0f, 0.0f, 0.0f}, (vec3){0.5f * cos(speed), 0.5 * sin(speed), 0.0f});
        nl_renderer_draw_line(r, (vec3){0.0f, 0.0f, 0.0f}, (vec3){0.5f * -cos(speed), 0.5 * -sin(speed), 0.0f});
        nl_renderer_draw_line(r, (vec3){0.0f, 0.0f, 0.0f}, (vec3){0.5f * -sin(speed), 0.5 * cos(speed), 0.0f});
        nl_renderer_draw_line(r, (vec3){0.0f, 0.0f, 0.0f}, (vec3){0.5f * sin(speed), 0.5 * -cos(speed), 0.0f});

        nl_renderer_fill_color(r, (color){1.0f, 0.90f, 0.3f});
        nl_renderer_fill_rectangle(r, 
                (vec3){0.5f * -cos(speed * 10), 0.5 * -sin(speed * 10), 0.0f},
                (vec3){0.5f * -sin(speed * 10), 0.5 * cos(speed * 10), 0.0f},
                (vec3){0.5f * cos(speed * 10), 0.5 * sin(speed * 10), 0.0f},
                (vec3){0.5f * sin(speed * 10), 0.5 * -cos(speed * 10), 0.0f});

        nl_renderer_fill_color(r, (color){1.0f, 1.0f, 1.0f});
        nl_renderer_draw_line(r, (vec3){0.0f, 0.0f, 0.0f}, (vec3){0.0f, 1.0f, 0.0f});
        nl_renderer_draw_line(r, (vec3){0.0f, 0.0f, 0.0f}, (vec3){0.0f, -1.0f, 0.0f});

        nl_renderer_fill_color(r, (color){1.0f, 1.0f, 1.0f});
        nl_renderer_draw_line(r, (vec3){0.0f, 0.0f, 0.0f}, (vec3){1.0f, 0.0f, 0.0f});
        nl_renderer_draw_line(r, (vec3){0.0f, 0.0f, 0.0f}, (vec3){-1.0f, 0.0f, 0.0f});

        nl_renderer_fill_color(r, (color){0.0f, 1.0f, 1.0f});
        nl_renderer_fill_triangle(r, (vec3){0.0f, 0.0f, 0.0f}, (vec3){0.5f, 0.5f, 0.0f}, (vec3){0.5f, 0.0f, 0.0f});

        nl_renderer_fill_color(r, (color){0.0f, 0.0f, 1.0f});
        nl_renderer_fill_triangle(r, (vec3){0.0f, 0.0f, 0.0f}, (vec3){-0.5f, 0.5f, 0.0f}, (vec3){-0.5f, 0.0f, 0.0f});

        nl_renderer_fill_color(r, (color){1.0f, 0.0f, 0.0f});
        nl_renderer_fill_triangle(r, (vec3){0.0f, 0.0f, 0.0f}, (vec3){-0.5f, -0.5f, 0.0f}, (vec3){-0.5f, 0.0f, 0.0f});

        nl_renderer_fill_color(r, (color){0.5f, 1.0f, 0.0f});
        nl_renderer_fill_triangle(r, (vec3){0.0f, 0.0f, 0.0f}, (vec3){0.5f, 0.0f, 0.0f}, (vec3){0.5f, -0.5f, 0.0f});

        nl_renderer_fill_color(r, (nl_color){0.10f, 0.3f, 0.8f});
        nl_renderer_fill_circle(r, (vec3){0.0f, 0.0f, 0.0f}, 0.1 * sin(time));

        nl_renderer_fill_color(r, (color){0.5f, 0.5f, 1.0f});
        nl_renderer_fill_rectangle(r, (vec3){-0.1f, -0.1f, 0.0f}, (vec3){-0.1f, 0.1f, 0.0f}, (vec3){0.1f, 0.1f, 0.0f}, (vec3){0.1f, -0.1f, 0.0f});

        // TODO: ITS JUST A MESS
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        nl_shader_use(texture_shader);
        nl_shader_uniform_i32(texture_shader, "texture1", 0);
        nl_shader_uniform_i32(texture_shader, "texture2", 1);
        nl_renderer_fill_color(r, (color){0.0f, 1.0f, 0.0f});

        nl_renderer_render_texture(
                r,
                t1,
                (vec3){-1.0f, -1.0f}, 
                (vec3){-1.0f, 0.0f}, 
                (vec3){0.0f, 0.0f}, 
                (vec3){-0.0f, -1.0f});
        nl_renderer_render_texture(r, t2, (vec3){-0.5f, -0.5f}, (vec3){-0.5f, 0.5f}, (vec3){0.5f, 0.5f}, (vec3){0.5f, -0.5f});

        nl_window_update(win);
    }

    nl_shader_free(sh);
    nl_shader_free(shader_left_right);
    nl_shader_free(shader_up_down);

    nl_texture_free(t1);
    nl_texture_free(t2);

    nl_window_free(win);
    nl_renderer_free(r);
}
