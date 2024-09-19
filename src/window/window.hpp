#ifndef WINDOW_H
#define WINDOW_H

#include "../ren.h"

namespace window {

GLFWwindow* init(uint32_t width, uint32_t height, const char *title);
void init_surface(GLFWwindow *window, t_rulkan *rulkan);

void destroy(t_ren *ren);

}

#endif /* WINDOW_H */
