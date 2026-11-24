#include "glad/glad.h"
#include "window.h"
#include <stdlib.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>

static void nl_window_frame_buffersize_callback(GLFWwindow*, int, int);

nl_window *nl_window_new(int32_t width, int32_t height, const char* title)
{
    nl_window *win;
    win = malloc(sizeof(nl_window));
    if (win == NULL)
        return NULL;

    win->width = width;
    win->height = height;
    win->title = title;
    if (!glfwInit()) {
        free(win);
        return NULL;
    }

    win->window = glfwCreateWindow(win->width, win->height, win->title, NULL, NULL);
    if (win->window == NULL) {
        if (win != NULL)
            free(win);
        glfwTerminate();
        return NULL;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,  3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwMakeContextCurrent(win->window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        if (win->window != NULL)
            glfwDestroyWindow(win->window);
        if (win != NULL)
            free(win);
        glfwTerminate();
        return NULL;
    }

    glfwSetFramebufferSizeCallback(win->window, nl_window_frame_buffersize_callback);
    win->running = true;
    return win;
}

void nl_window_free(nl_window* win)
{
    if (win == NULL)
        return;

    if (win->window != NULL)
        glfwDestroyWindow(win->window);

    glfwTerminate();
    free(win);
}

void nl_window_stop(nl_window *w)
{
    w->running = false;
}

GLFWwindow* nl_window_get_glfw_window(nl_window *w) 
{
    if (w != NULL)
        return w->window;
    return NULL;
}

double nl_window_time(nl_window* w)
{
    return glfwGetTime();
}

bool nl_window_is_running(nl_window* w)
{
    return w->running == true; 
}

void nl_window_update(nl_window *w)
{
        glfwSwapBuffers(w->window);
        glfwPollEvents();
}

static void nl_window_frame_buffersize_callback(GLFWwindow* w, int width, int height)
{
    glViewport(0, 0, width, height);
}

