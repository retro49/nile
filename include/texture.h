#ifndef __NILE_TEXTURE_H
#define __NILE_TEXTURE_H

#include <stdint.h>
#include <GL/gl.h>

typedef struct nl_texture {
    int32_t width;
    int32_t height;
    int32_t channels;

    GLuint texture;

} nl_texture;

nl_texture *nl_texture_new(const char*);
void nl_texture_free(nl_texture*);

#endif
