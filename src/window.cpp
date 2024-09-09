#include "ren.h"

namespace window {

void init(t_ren* ren, uint32_t width, uint32_t height, const char* title) {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    ren->window = glfwCreateWindow(width, height, title, nullptr, nullptr);
}

}
