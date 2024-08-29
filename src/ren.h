#ifndef REN_H
#define REN_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

typedef struct {
    GLFWwindow* window;

    VkDevice            device;
    VkInstance          instance;
    VkQueue             graphics_queue;
    VkPhysicalDevice    physical_device;
} t_ren;

extern "C" {

t_ren ren_init(int width, int height, const char* title);
void ren_destroy(t_ren* ren);

}

#endif // REN_H
