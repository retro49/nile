#include "shader.h"
#include "glad/glad.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <string.h>

// returns a null terminated buffer with the file content
static char *nl_shader_read_file(const char *path)
{
    if (path == NULL)
        return NULL;

    uint32_t file_size = 0;

#if defined(__linux__)
#include <sys/stat.h>
    struct stat s;
    if (stat(path, &s) != 0) {
        printf("ERROR::SHADER unable to retrive file information for file at path: %s\n", path);
        return NULL;
    }
    file_size = (uint32_t)s.st_size;
#elif defined(_WIN32)
#error "WINDOWS IMPLEMENTATION IS NOT DEFINED YET!"
#elif defined(__APPLE__)
#error "APPLE IMPLEMENTATION IS NOT DEFINED YET!"
#else
#error "OPERATING SYSTEM IS NOT DETECTED"
#endif
    if (file_size == 0) {
        printf("ERROR::SHADER unable to read empty file\n");
        return NULL;
    }

    char *buffer = NULL;
    FILE* file = NULL;

    file = fopen(path, "r");
    if (file == NULL) {
        printf("ERROR::SHADER unable to read file\n");
        return NULL;
    }

    buffer = malloc(sizeof(char) * (file_size + 1));
    if (buffer == NULL) {
        printf("ERROR::SHADER unable to allocate file buffer\n");
        fclose(file);
        return NULL;
    }

    fread(buffer, sizeof(char), file_size, file);
    fclose(file);
    *(buffer + file_size) = 0;
    return buffer;
}

#define NL_SHADER_ERROR_BUFFER_SIZE 512
static char nl_error_buffer[NL_SHADER_ERROR_BUFFER_SIZE];

// returns true if an error occured during compilation
static bool nl_shader_check_compilation_error(GLuint shader)
{
    int error = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &error);
    if (error != 0)
        return false;
    memset(nl_error_buffer, 0, sizeof(char) * NL_SHADER_ERROR_BUFFER_SIZE);
    glGetShaderInfoLog(shader, NL_SHADER_ERROR_BUFFER_SIZE, 0, nl_error_buffer);
    printf("ERROR::SHADER shader compilation error due to: %s\n", nl_error_buffer);
    return true;
}

static bool nl_shader_check_link_error(GLuint program)
{
    int error = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &error);
    if (error != 0)
        return false;

    memset(nl_error_buffer, 0, sizeof(char) * NL_SHADER_ERROR_BUFFER_SIZE);
    glGetProgramInfoLog(program, NL_SHADER_ERROR_BUFFER_SIZE, 0, nl_error_buffer);
    printf("ERROR::SHADER shader linking error due to: %s\n", nl_error_buffer);
    return true;
}

nl_shader *nl_shader_from_file(const char* v_path, const char* f_path)
{
    nl_shader *s = NULL;
    char *v_buff = NULL;
    char *f_buff = NULL;

    v_buff = nl_shader_read_file(v_path);
    if (v_buff == NULL) {
        printf("ERROR::SHADER unable to read vertx shader source\n");
        return NULL;
    }

    f_buff = nl_shader_read_file(f_path);
    if (f_buff == NULL) {
        printf("ERROR::SHADER unable to read fragment shader source\n");
        goto __failure;
    }

    s = malloc(sizeof(nl_shader));
    if (s == NULL) {
        printf("ERROR::SHADER unable to allocate shader object");
        goto __failure;
    }

    GLuint vert_shader = 0;
    GLuint frag_shader = 0;

    vert_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_shader, 1, (const char**)&v_buff, NULL);
    glCompileShader(vert_shader);
    // check error
    if (nl_shader_check_compilation_error(vert_shader)) {
        printf("unable to compile vertex shader\n");
        goto __failure;
    }

    frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, (const char**)&f_buff, NULL);
    glCompileShader(frag_shader);
    if (nl_shader_check_compilation_error(frag_shader)) {
        goto __failure;
    }
    // link 
    GLuint program = 0;
    program = glCreateProgram();

    glAttachShader(program, vert_shader);
    glAttachShader(program, frag_shader);
    glLinkProgram(program);
    if (nl_shader_check_link_error(program)) {
        printf("ERROR::SHADER unable to link program to final stage\n");
        goto __failure;
    }

    if (v_buff != NULL)
        free(v_buff);
    if (f_buff != NULL)
        free(f_buff);

    if (vert_shader != 0)
        glDeleteShader(vert_shader);

    if (frag_shader != 0)
        glDeleteShader(frag_shader);

    s->program = program;

    return s;
__failure:
    if (v_buff != NULL)
        free(v_buff);

    if (f_buff != NULL)
        free(f_buff);


    if (vert_shader != 0)
        glDeleteShader(vert_shader);

    if (frag_shader != 0)
        glDeleteShader(frag_shader);

    if (program != 0)
        glDeleteProgram(program);

    if (s != NULL)
        free(s);

    return NULL;
}

void nl_shader_free(nl_shader* s)
{
    if (s == NULL)
        return;
    if (s->program != 0)
        glDeleteProgram(s->program);

    free(s);
}

void nl_shader_use(nl_shader *s)
{
    glUseProgram(s->program);
}

void nl_shader_uniform_f32(nl_shader *s, const char *name, float f)
{
    glUniform1f(glGetUniformLocation(s->program, name), f);
}

void nl_shader_uniform_u32(nl_shader *s, const char *name, uint32_t n)
{
    glUniform1ui(glGetUniformLocation(s->program, name), n);
}

void nl_shader_uniform_i32(nl_shader *s, const char *name, int32_t n)
{
    glUniform1i(glGetUniformLocation(s->program, name), n);
}
