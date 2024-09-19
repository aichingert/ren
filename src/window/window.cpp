#include "window.h"

namespace window {

GLFWwindow* init(uint32_t width, uint32_t height, const char *title) {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    return glfwCreateWindow(width, height, title, nullptr, nullptr);
}

void destroy(t_ren *ren) {
    glfwDestroyWindow(ren->window);
    glfwTerminate();
}

}
