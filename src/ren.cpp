//#define GLM_FORCE_DEPTH_ZERO_TO_ONE
//#include <glm/vec4.hpp>
//#include <glm/mat4x4.hpp>

#include "ren.h"
#include "window.cpp"
#include "vulkan/vulkan.cpp"

#include <iostream>

extern "C" t_ren ren_init(int width, int height, const char* title) {
    t_ren ren = {0};

    window::init(&ren, width, height, title);
    vk::init(&ren, title);

    return ren;
}

extern "C" void ren_destroy(t_ren* ren) {
    vkDestroyPipeline(ren->device, ren->graphics_pipeline, nullptr);
    vkDestroyPipelineLayout(ren->device, ren->pipeline_layout, nullptr);
    vkDestroyRenderPass(ren->device, ren->render_pass, nullptr);

    for (size_t i = 0; i < ren->swap_chain_images_size; i++) {
        vkDestroyImageView(ren->device, ren->swap_chain_image_views[i], nullptr);
    }

    vkDestroySwapchainKHR(ren->device, ren->swap_chain, nullptr);
    vkDestroyDevice(ren->device, nullptr);
    vkDestroySurfaceKHR(ren->instance,ren->surface, nullptr);
    vkDestroyInstance(ren->instance, nullptr);
    glfwDestroyWindow(ren->window);
    glfwTerminate();
}
