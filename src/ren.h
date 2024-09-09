#ifndef REN_H
#define REN_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

const size_t MAX_FRAMES_IN_FLIGHT = 2;

typedef struct {
    GLFWwindow*             window;

    VkInstance              instance;
    VkSurfaceKHR            surface;

    VkDevice                device;
    VkPhysicalDevice        physical_device;

    VkQueue                 present_queue;
    VkQueue                 graphics_queue;

    VkSwapchainKHR          swap_chain;
    VkExtent2D              swap_chain_extent;
    VkImage*                swap_chain_images;
    VkImageView*            swap_chain_image_views;
    size_t                  swap_chain_images_size;
    VkFormat                swap_chain_image_format;
    VkFramebuffer*          swap_chain_framebuffers;

    VkRenderPass            render_pass;
    VkDescriptorSetLayout   descriptor_set_layout;
    VkDescriptorPool        descriptor_pool;
    VkDescriptorSet*        descriptor_sets;

    VkPipelineLayout        pipeline_layout;
    VkPipeline              graphics_pipeline;

    VkCommandPool           command_pool;
    VkCommandBuffer*        command_buffers;

    VkBuffer                vertex_buffer;
    VkDeviceMemory          vertex_buffer_memory;
    VkBuffer                index_buffer;
    VkDeviceMemory          index_buffer_memory;

    VkBuffer*               uniform_buffers;
    VkDeviceMemory*         uniform_buffers_memory;
    void**                  uniform_buffers_mapped;

    VkFence*                in_flight_fences;
    VkSemaphore*            image_available_semaphores;
    VkSemaphore*            render_finished_semaphores;

    uint32_t                current_frame;
} t_ren;

extern "C" {

t_ren ren_init(uint32_t width, uint32_t height, const char* title);
void ren_destroy(t_ren* ren);

void ren_draw_frame(t_ren* ren);

}

#endif // REN_H
