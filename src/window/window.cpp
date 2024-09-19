#include "window.hpp"
#include "../rulkan/util.hpp"

namespace window {

GLFWwindow* init(uint32_t width, uint32_t height, const char *title) {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    return glfwCreateWindow(width, height, title, nullptr, nullptr);
}

void init_surface(GLFWwindow *window, t_rulkan *rulkan) {
    VkResult res = glfwCreateWindowSurface(rulkan->instance, window, nullptr, &rulkan->surface);
    VK_CHECK(res, "Failed to create window surface!");
}

void destroy(t_ren *ren) {
    glfwDestroyWindow(ren->window);
    glfwTerminate();
}

}
