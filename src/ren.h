#ifndef REN_H
#define REN_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

typedef struct {
    GLFWwindow* window;

    VkInstance          instance;
    VkSurfaceKHR        surface;

    VkDevice            device;
    VkPhysicalDevice    physical_device;

    VkQueue             present_queue;
    VkQueue             graphics_queue;

    VkSwapchainKHR      swap_chain;
    VkExtent2D          swap_chain_extent;
    VkImage*            swap_chain_images;
    VkImageView*        swap_chain_image_views;
    size_t              swap_chain_images_size;
    VkFormat            swap_chain_image_format;

    VkRenderPass        render_pass;
    VkPipelineLayout    pipeline_layout;
    VkPipeline          graphics_pipeline;
} t_ren;

extern "C" {

t_ren ren_init(int width, int height, const char* title);
void ren_destroy(t_ren* ren);

}

#endif // REN_H
