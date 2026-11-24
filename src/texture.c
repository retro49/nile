#include "glad/glad.h"
#include "texture.h"
#include "stb_image.h"
#include <GL/gl.h>
#include <stdio.h>
#include <stdlib.h>

nl_texture *nl_texture_new(const char* path)
{
    if (path == NULL)
        return NULL;

    unsigned char *data = NULL;
    int width;
    int height;
    int channels;
    data = stbi_load(path, &width, &height, &channels, 0);
    if (data == NULL) {
        printf("ERROR::TEXTURE unable to load texture from path: %s\n", path);
        return NULL;
    }

    nl_texture *t = NULL;
    t = malloc(sizeof(nl_texture));
    if (t == NULL) {
        if (data != NULL)
            stbi_image_free(data);
        return NULL;
    }

    t->channels = channels;
    t->width = width;
    t->height = height;

    glGenTextures(1, &t->texture);
    glBindTexture(GL_TEXTURE_2D, t->texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    switch(t->channels) {
        case 3:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, t->width, t->height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            break;
        case 4:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t->width, t->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            break;
        default:
            {
                // TODO: panic right here...
                // Other channels are not support for now.
            }
    }

    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
    data = NULL;

    return t;
}

void nl_texture_free(nl_texture *t)
{
    if (t == NULL)
        return;
    glDeleteTextures(1, &t->texture);
    free(t);
}
