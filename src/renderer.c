#include "glad/glad.h"
#include "renderer.h"
#include "color.h"
#include "math.h"
#include "nl_math.h"
#include <GL/glext.h>
#include <math.h>
#include <stdlib.h>
#include <GL/gl.h>

static void nl_renderer_config_color(nl_renderer*, nl_color);

static void nl_renderer_config_vertex_points_1(nl_renderer*, vec3);
static void nl_renderer_config_vertex_points_2(nl_renderer*, vec3);
static void nl_renderer_config_vertex_points_3(nl_renderer*, vec3);
static void nl_renderer_config_vertex_points_4(nl_renderer*, vec3);

static void nl_renderer_config_texture_1(nl_renderer *r, vec2 v);
static void nl_renderer_config_texture_2(nl_renderer *r, vec2 v);
static void nl_renderer_config_texture_3(nl_renderer *r, vec2 v);
static void nl_renderer_config_texture_4(nl_renderer *r, vec2 v);

nl_renderer *nl_renderer_new()
{
    nl_renderer *r = NULL;
    r = malloc(sizeof(nl_renderer));
    if (r == NULL)
        return NULL;

    // EBO Layout
    // Always go for the left then bottom then right then finally top
    // LEFT - BOTTOM
    // LEFT - TOP
    // RIGHT - TOP
    // RIGHT - BOTTOM
    // 0, 0, // left bottom
    // 0, 1, // left top
    // 1, 1, // right top
    // 0, 1, // left bottom
    // 1, 0, // right bottom
    // 1, 0, // right top
    //
    // 0, 0, // left bottom
    // 0, 1, // left top
    // 1, 1, // right top
    // 1, 0, // right bottom

    r->renderingEBO[0] = 0;
    r->renderingEBO[1] = 1;
    r->renderingEBO[2] = 2;
    r->renderingEBO[3] = 0;
    r->renderingEBO[4] = 2;
    r->renderingEBO[5] = 3;

    nl_renderer_config_texture_1(r, (vec2){0.0f, 0.0f});
    nl_renderer_config_texture_2(r, (vec2){0.0f, 1.0f});
    nl_renderer_config_texture_3(r, (vec2){1.0f, 1.0f});
    nl_renderer_config_texture_4(r, (vec2){1.0f, 0.0f});

    // nl_renderer_config_texture_1(r, (vec2){1.0f, 1.0f});
    // nl_renderer_config_texture_2(r, (vec2){1.0f, 0.0f});
    // nl_renderer_config_texture_3(r, (vec2){0.0f, 0.0f});
    // nl_renderer_config_texture_4(r, (vec2){0.0f, 1.0f});

    glGenVertexArrays(1, &r->vao);
    glGenBuffers(1, &r->vbo);
    glGenBuffers(1, &r->ebo);

    glBindVertexArray(r->vao);
    glBindBuffer(GL_ARRAY_BUFFER, r->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(r->renderingVBO), &r->renderingVBO, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, r->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(r->renderingEBO), r->renderingEBO, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    return r;
}

void nl_renderer_free(nl_renderer* r)
{
    if (r == NULL)
        return;

    glDeleteVertexArrays(1, &r->vao);
    glDeleteBuffers(1, &r->vbo);
    glDeleteBuffers(1, &r->ebo);
}

void nl_renderer_fill_color(nl_renderer *r, nl_color c)
{
    nl_renderer_config_color(r, c);
}

void nl_renderer_draw_line(nl_renderer *r, vec3 p1, vec3 p2)
{
    nl_renderer_config_vertex_points_1(r, p1);
    nl_renderer_config_vertex_points_2(r, p2);

    glBindBuffer(GL_ARRAY_BUFFER, r->vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(r->renderingVBO), r->renderingVBO);
    glBindVertexArray(r->vao);
    glDrawArrays(GL_LINES, 0, 2);
    glBindVertexArray(0);
}

void nl_renderer_fill_triangle(nl_renderer *r, vec3 p1, vec3 p2, vec3 p3)
{
    nl_renderer_config_vertex_points_1(r, p1);
    nl_renderer_config_vertex_points_2(r, p2);
    nl_renderer_config_vertex_points_3(r, p3);

    glBindBuffer(GL_ARRAY_BUFFER, r->vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(r->renderingVBO), r->renderingVBO);
    glBindVertexArray(r->vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}

void nl_renderer_fill_rectangle(nl_renderer *r, vec3 p1, vec3 p2, vec3 p3, vec3 p4)
{
    nl_renderer_config_vertex_points_1(r, p1);
    nl_renderer_config_vertex_points_2(r, p2);
    nl_renderer_config_vertex_points_3(r, p3);
    nl_renderer_config_vertex_points_4(r, p4);

    glBindBuffer(GL_ARRAY_BUFFER, r->vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(r->renderingVBO), r->renderingVBO);
    glBindVertexArray(r->vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
    glBindVertexArray(0);
}

void nl_renderer_fill_circle(nl_renderer *r, vec3 p, float rad)
{
    const int step = 10;
    for (int i = 0; i < 360; i+=step) {
        float a1 = NL_MATH_TO_RAD(i);
        float a2 = NL_MATH_TO_RAD(i + step);

        double x1 = p.x + (cos(a1) * rad);
        double y1 = p.y + (sin(a1) * rad);

        double x2 = p.x + (cos(a2) * rad);
        double y2 = p.y + (sin(a2) * rad);

        nl_renderer_config_vertex_points_1(r, p);
        nl_renderer_config_vertex_points_2(r, (vec3){(float)x1, (float)y1, 0.0f});
        nl_renderer_config_vertex_points_3(r, (vec3){(float)x2, (float)y2, 0.0f});

        glBindBuffer(GL_ARRAY_BUFFER, r->vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(r->renderingVBO), r->renderingVBO);
        glBindVertexArray(r->vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
    }

}

void nl_renderer_clear_bg(nl_color c)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(c.r, c.g, c.b, 1.0f);
}

void nl_renderer_render_texture(nl_renderer *r, nl_texture *t, vec3 p1, vec3 p2, vec3 p3, vec3  p4)
{

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    nl_renderer_config_vertex_points_1(r, p1);
    nl_renderer_config_vertex_points_2(r, p2);
    nl_renderer_config_vertex_points_3(r, p3);
    nl_renderer_config_vertex_points_4(r, p4);

    glBindBuffer(GL_ARRAY_BUFFER, r->vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(r->renderingVBO), r->renderingVBO);
    glBindTexture(GL_TEXTURE_2D, t->texture);
    glBindVertexArray(r->vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

static void nl_renderer_config_color(nl_renderer *r, nl_color c)
{
    
    r->renderingVBO[3] = c.r;
    r->renderingVBO[4] = c.g;
    r->renderingVBO[5] = c.b;

    r->renderingVBO[11] = c.r;
    r->renderingVBO[12] = c.g;
    r->renderingVBO[13] = c.b;

    r->renderingVBO[19] = c.r;
    r->renderingVBO[20] = c.g;
    r->renderingVBO[21] = c.b;

    r->renderingVBO[27] = c.r;
    r->renderingVBO[28] = c.g;
    r->renderingVBO[29] = c.b;
}

static void nl_renderer_config_vertex_points_1(nl_renderer *r, vec3 v)
{
    r->renderingVBO[0] = v.x;
    r->renderingVBO[1] = v.y;
    r->renderingVBO[2] = v.z;
}

static void nl_renderer_config_vertex_points_2(nl_renderer *r, vec3 v)
{
    r->renderingVBO[8] = v.x;
    r->renderingVBO[9] = v.y;
    r->renderingVBO[10] = v.z;
}

static void nl_renderer_config_vertex_points_3(nl_renderer *r, vec3 v)
{
    r->renderingVBO[16] = v.x;
    r->renderingVBO[17] = v.y;
    r->renderingVBO[18] = v.z;
}

static void nl_renderer_config_vertex_points_4(nl_renderer *r, vec3 v)
{
    r->renderingVBO[24] = v.x;
    r->renderingVBO[25] = v.y;
    r->renderingVBO[26] = v.z;
}

static void nl_renderer_config_texture_1(nl_renderer *r, vec2 v)
{
    r->renderingVBO[6] = v.x; 
    r->renderingVBO[7] = v.y;
}

static void nl_renderer_config_texture_2(nl_renderer *r, vec2 v)
{
    r->renderingVBO[14] = v.x;
    r->renderingVBO[15] = v.y; 
}

static void nl_renderer_config_texture_3(nl_renderer *r, vec2 v)
{ 
    r->renderingVBO[22] = v.x; 
    r->renderingVBO[23] = v.y; 
}

static void nl_renderer_config_texture_4(nl_renderer *r, vec2 v) {
    r->renderingVBO[30] = v.x; 
    r->renderingVBO[31] = v.y; 
}
