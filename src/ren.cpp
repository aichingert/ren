#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include "ren.h"
#include "window.cpp"
#include "vulkan/instance.cpp"

extern "C" t_ren ren_init(int width, int height, const char* title) {
    t_ren ren = {0};

    window::init_glfw(&ren, width, height, title);
    vk::init_instance(&ren, title);

    return ren;
}

extern "C" void ren_destroy(t_ren* ren) {
    vkDestroyInstance(ren->instance, nullptr);
    glfwDestroyWindow(ren->window);
    glfwTerminate();
}
