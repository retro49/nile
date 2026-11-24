#include "input.h"
#include <stdbool.h>

bool nl_is_key_pressed(nl_window *w, int key)
{
    return glfwGetKey(nl_window_get_glfw_window(w), key) == GLFW_PRESS;
}
