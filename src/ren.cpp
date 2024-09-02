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

extern "C" void ren_draw_frame(t_ren* ren) {
    vkWaitForFences(ren->device, 1, &ren->in_flight_fences[ren->current_frame], VK_TRUE, UINT64_MAX);
    vkResetFences(ren->device, 1, &ren->in_flight_fences[ren->current_frame]);

    uint32_t image_idx;
    vkAcquireNextImageKHR(
            ren->device, 
            ren->swap_chain, 
            UINT64_MAX, 
            ren->image_available_semaphores[ren->current_frame], 
            VK_NULL_HANDLE, 
            &image_idx);

    vkResetCommandBuffer(ren->command_buffers[ren->current_frame], 0);
    vk::record_command_buffer(ren, ren->command_buffers[ren->current_frame], image_idx);

    VkSubmitInfo submit_info{};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore wait_semaphores[] = { ren->image_available_semaphores[ren->current_frame] };
    VkPipelineStageFlags wait_stages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submit_info.waitSemaphoreCount = 1;
    submit_info.pWaitSemaphores = wait_semaphores;
    submit_info.pWaitDstStageMask = wait_stages;
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &ren->command_buffers[ren->current_frame];

    VkSemaphore signal_semaphores[] = { ren->render_finished_semaphores[ren->current_frame] };
    submit_info.signalSemaphoreCount = 1;
    submit_info.pSignalSemaphores = signal_semaphores;

    if (vkQueueSubmit(
                ren->graphics_queue, 
                1, 
                &submit_info, 
                ren->in_flight_fences[ren->current_frame]) != VK_SUCCESS
    ) {
        throw std::runtime_error("Failed to submit draw command buffer!");
    }

    VkPresentInfoKHR present_info{};
    present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    present_info.waitSemaphoreCount = 1;
    present_info.pWaitSemaphores = signal_semaphores;

    VkSwapchainKHR swap_chains[] = { ren->swap_chain };
    present_info.swapchainCount = 1;
    present_info.pSwapchains = swap_chains;
    present_info.pImageIndices = &image_idx;

    vkQueuePresentKHR(ren->present_queue, &present_info);
    ren->current_frame = (ren->current_frame + 1) % MAX_FRAMES_IN_FLIGHT;
}

extern "C" void ren_destroy(t_ren* ren) {
    // NOTE: waiting for all semaphores
    vkDeviceWaitIdle(ren->device);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroySemaphore(ren->device, ren->image_available_semaphores[i], nullptr);
        vkDestroySemaphore(ren->device, ren->render_finished_semaphores[i], nullptr);
        vkDestroyFence(ren->device, ren->in_flight_fences[i], nullptr);
    }

    vkDestroyCommandPool(ren->device, ren->command_pool, nullptr);

    for (size_t i = 0; i < ren->swap_chain_images_size; i++) {
        vkDestroyFramebuffer(ren->device, ren->swap_chain_framebuffers[i], nullptr);
        vkDestroyImageView(ren->device, ren->swap_chain_image_views[i], nullptr);
    }

    vkDestroyPipeline(ren->device, ren->graphics_pipeline, nullptr);
    vkDestroyPipelineLayout(ren->device, ren->pipeline_layout, nullptr);
    vkDestroyRenderPass(ren->device, ren->render_pass, nullptr); 

    vkDestroySwapchainKHR(ren->device, ren->swap_chain, nullptr);
    vkDestroyDevice(ren->device, nullptr);
    vkDestroySurfaceKHR(ren->instance,ren->surface, nullptr);
    vkDestroyInstance(ren->instance, nullptr);
    glfwDestroyWindow(ren->window);
    glfwTerminate();
}
