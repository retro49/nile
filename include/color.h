#ifndef __nl_COLOR_H
#define __nl_COLOR_H

typedef struct nl_color {
    float r;
    float g;
    float b;
} nl_color, color;

nl_color nl_color_rgba(float, float, float, float);

#endif
