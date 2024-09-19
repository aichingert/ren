#ifndef REN_H
#define REN_H

#include "rulkan/rulkan.h"

typedef struct {
    GLFWwindow *window;
    t_rulkan    rulkan;
} t_ren;

extern "C" {

t_ren ren_init(uint32_t width, uint32_t height, const char *title);
void ren_destroy(t_ren *ren);

}

#endif // REN_H
