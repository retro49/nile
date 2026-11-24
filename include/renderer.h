#ifndef __nl_RENDERER_H
#define __nl_RENDERER_H

#include "vec.h"
#include "color.h"
#include "texture.h"
#include <GL/gl.h>

#define NL_RENDERING_VERTICEX_BUFFER_OBJECT_SIZE 32
#define NL_RENDERING_ELEMENT_BUFFER_OBJECT_SIZE 6

typedef struct nl_renderer {
    GLuint vao;
    GLuint vbo;
    GLuint ebo;

    GLfloat renderingVBO[NL_RENDERING_VERTICEX_BUFFER_OBJECT_SIZE];
    GLuint renderingEBO[NL_RENDERING_ELEMENT_BUFFER_OBJECT_SIZE];
} nl_renderer;

nl_renderer *nl_renderer_new();
void nl_renderer_free(nl_renderer*);

void nl_renderer_clear_bg(nl_color);

void nl_renderer_fill_color(nl_renderer*, nl_color);
void nl_renderer_draw_line(nl_renderer*, vec3, vec3);
void nl_renderer_fill_triangle(nl_renderer*, vec3, vec3, vec3);
void nl_renderer_fill_rectangle(nl_renderer*, vec3, vec3, vec3, vec3);
void nl_renderer_fill_circle(nl_renderer*, vec3, float);

void nl_renderer_render_texture(nl_renderer*, nl_texture*, vec3, vec3, vec3, vec3);

#endif
