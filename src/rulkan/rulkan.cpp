#include <iostream>

#include "rulkan.h"

#include "util.hpp"
#include "instance.hpp"
#include "../window/window.hpp"
#include "device.hpp"
#include "swapchain.hpp"
#include "pipeline.hpp"
#include "buffer.hpp"
#include "command.hpp"
#include "sync.hpp"

namespace rulkan {

t_rulkan init(GLFWwindow *window, const char *title) {
    t_rulkan rulkan = {};

    init_instance(rulkan, title);
    window::init_surface(window, &rulkan);
    init_devices(rulkan);
    init_swapchain(rulkan, window);
    init_image_views(rulkan);
    init_render_pass(rulkan);
    init_graphics_pipeline(rulkan);
    init_framebuffers(rulkan);
    init_command_pools(rulkan);

    for(size_t i = 0; i < FRAME_OVERLAP; i++) {
        rulkan.frames[i].vb = init_vertex_buffer(rulkan);
    }

    init_command_buffers(rulkan);
    init_sync_structures(rulkan);

    return rulkan;
}

void draw(t_rulkan& rulkan, GLFWwindow *window, uint32_t frame) {
    vkWaitForFences(rulkan.device, 1, &rulkan.frames[frame].render_fence, VK_TRUE, UINT64_MAX);

    uint32_t image_idx;
    VkResult res = vkAcquireNextImageKHR(
        rulkan.device,
        rulkan.swapchain.self,
        UINT64_MAX,
        rulkan.frames[frame].present_sema,
        VK_NULL_HANDLE,
        &image_idx);

    if (res == VK_ERROR_OUT_OF_DATE_KHR) {
        return recreate_swapchain(rulkan, window);
    } else if (res != VK_SUCCESS && res != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("Failed to aquire swap chain image!");
    }


    vkResetFences(rulkan.device, 1, &rulkan.frames[frame].render_fence);
    vkResetCommandBuffer(rulkan.frames[frame].command_buffer, 0);


    record_command_buffer(rulkan, rulkan.frames[frame].command_buffer, frame, image_idx);

    VkSubmitInfo submit_info{};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore wait_semaphores[] = { rulkan.frames[frame].present_sema };

    VkPipelineStageFlags wait_stages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submit_info.waitSemaphoreCount = 1;
    submit_info.pWaitSemaphores = wait_semaphores;
    submit_info.pWaitDstStageMask = wait_stages;
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &rulkan.frames[frame].command_buffer;

    VkSemaphore signal_semaphores[] = { rulkan.frames[frame].render_sema };
    submit_info.signalSemaphoreCount = 1;
    submit_info.pSignalSemaphores = signal_semaphores;

    res = vkQueueSubmit(rulkan.graphics_queue, 1, &submit_info, rulkan.frames[frame].render_fence);
    VK_CHECK(res, "Failed to submit draw command buffer!");

    VkPresentInfoKHR present_info{};
    present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    present_info.waitSemaphoreCount = 1;
    present_info.pWaitSemaphores = signal_semaphores;

    VkSwapchainKHR swapchains[] = { rulkan.swapchain.self };
    present_info.swapchainCount = 1;
    present_info.pSwapchains = swapchains;
    present_info.pImageIndices = &image_idx;
    res = vkQueuePresentKHR(rulkan.present_queue, &present_info);

    if (res == VK_ERROR_OUT_OF_DATE_KHR || res == VK_SUBOPTIMAL_KHR) {
        recreate_swapchain(rulkan, window);
    } else if (res != VK_SUCCESS) {
        throw std::runtime_error("Failed to present swap chain image!");
    }
}

void destroy(t_rulkan& rulkan) {
    destroy_swapchain(rulkan);

    for (size_t i = 0; i < FRAME_OVERLAP; i++) {
        //VkDestroyFence(rulkan.device, rulkan.frames[i].render_fence, nullptr);
        //VkDestroySemaphore(rulkan.device, rulkan.frames[i].render_sema, nullptr);
        //VkDestroySemaphore(rulkan.device, rulkan.frames[i].present_sema, nullptr);
    }

}

}
