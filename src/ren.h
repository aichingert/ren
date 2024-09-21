#ifndef REN_H
#define REN_H

#include "rulkan/rulkan.h"
#include "core/math.h"

typedef struct {
    GLFWwindow *window;
    t_rulkan    rulkan;

    t_list      vertices;
    uint32_t    frame;
} t_ren;

extern "C" {

t_ren ren_init(uint32_t width, uint32_t height, const char *title);

void ren_draw_triangle(t_ren *ren, t_vec2 a, t_vec2 b, t_vec2 c);
void ren_draw_frame(t_ren *ren);

void ren_destroy(t_ren *ren);

}

#endif // REN_H
