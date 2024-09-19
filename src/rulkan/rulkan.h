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
    VkInstance  instance;

    t_frame_data frames[FRAME_OVERLAP];
};

namespace rulkan {

t_rulkan init();
void destroy(const t_rulkan&);

}

#endif /* RULKAN_H */
