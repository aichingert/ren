#ifndef RULKAN_H
#define RULKAN_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

const size_t FRAME_OVERLAP = 2;

/* TODO: vertex and t_list have to be renamed and
 * should probably go into the core folder where
 * they are properly implemented
 */

struct vertex {
    float x;
    float y;
    float r;
    float g;
    float b;
};

typedef struct {
    vertex *data;

    size_t cap;
    size_t size;
} t_list;

struct t_vertex_buffer {
    VkBuffer buffer;
    VkDeviceMemory memory;
};

struct t_frame_data {
    VkFence         render_fence;
    VkSemaphore     present_sema, render_sema;

    t_vertex_buffer vb;

    VkCommandPool   command_pool;
    VkCommandBuffer command_buffer;
};

struct t_swapchain {
    VkSwapchainKHR self;
    VkExtent2D extent;
    VkFormat format;

    VkImage     *images;
    VkImageView *image_views;
    VkFramebuffer *framebuffers;

    size_t size;
};

struct t_rulkan {
    VkInstance instance;
    VkSurfaceKHR surface;

    VkDevice         device;
    VkPhysicalDevice physical_device;

    VkQueue present_queue;
    VkQueue graphics_queue;

    t_swapchain swapchain;
    VkRenderPass render_pass;

    VkPipeline graphics_pipeline;
    VkPipelineLayout pipeline_layout;

    t_frame_data frames[FRAME_OVERLAP];
};

namespace rulkan {

t_rulkan init(GLFWwindow *window, const char *title);

void draw(t_rulkan& rulkan, t_list& vertices, GLFWwindow *window, uint32_t frame);

void destroy(t_rulkan& rulkan);

}

#endif /* RULKAN_H */
