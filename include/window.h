#ifndef __nl_WINDOW_H
#define __nl_WINDOW_H

#include <stdint.h>
#include <stdbool.h>
#include <GLFW/glfw3.h>

typedef struct nl_window {
    uint32_t width;
    uint32_t height;
    const char *title;
    bool running;

    GLFWwindow *window;
} nl_window;

nl_window *nl_window_new(int32_t widht, int32_t height, const char *title);
void nl_window_free(nl_window*);
void nl_window_stop(nl_window*);
bool nl_window_is_running(nl_window*);
GLFWwindow* nl_window_get_glfw_window(nl_window*);
double nl_window_time(nl_window*);

void nl_window_update(nl_window*);

#endif
