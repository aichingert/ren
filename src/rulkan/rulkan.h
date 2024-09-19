#ifndef RULKAN_H
#define RULKAN_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

const size_t FRAME_OVERLAP = 2;

struct t_frame_data {
    VkFence         render_fence;
    VkSemaphore     present_sema, render_sema;

    VkCommandPool   command_pool;
    VkCommandBuffer command_buffer;
};

struct t_rulkan {
    VkInstance instance;
    VkSurfaceKHR surface;

    VkDevice         device;
    VkPhysicalDevice physical_device;

    VkQueue present_queue;
    VkQueue graphics_queue;


    t_frame_data frames[FRAME_OVERLAP];
};

namespace rulkan {

t_rulkan init(GLFWwindow *window, const char *title);
void destroy(const t_rulkan&);

}

#endif /* RULKAN_H */
