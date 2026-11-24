#ifndef __NILE_SHADER_H
#define __NILE_SHADER_H

#include <stdint.h>

typedef struct nl_shader {
    uint32_t program;
} nl_shader;

nl_shader *nl_shader_from_file(const char* v_path, const char* f_path);
void nl_shader_free(nl_shader*);
void nl_shader_use(nl_shader*);

void nl_shader_uniform_f32(nl_shader*, const char *, float);
void nl_shader_uniform_u32(nl_shader*, const char *, uint32_t);
void nl_shader_uniform_i32(nl_shader*, const char *, int32_t);

#endif
