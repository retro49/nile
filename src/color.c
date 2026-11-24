#include "color.h"

nl_color nl_color_rgba(float r, float g, float b, float a)
{
    return (nl_color){.r = r, .g = g, .b = b};
}
