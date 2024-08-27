#include "ren.h"

namespace window {

void init(t_ren* ren, int width, int height, const char* title) {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    ren->window = glfwCreateWindow(width, height, title, nullptr, nullptr);
}

}
