#include "ren.h"
#include "window.cpp"
#include "vulkan/vertex.h"
#include "vulkan/vulkan.cpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>
#include <cstring>

extern "C" t_ren ren_init(uint32_t width, uint32_t height, const char* title) {
    t_ren ren = {};

    window::init(&ren, width, height, title);
    vk::init(&ren, title);

    return ren;
}

void update_uniform_buffer(t_ren* ren) {
    static auto start = std::chrono::high_resolution_clock::now();
    auto current = std::chrono::high_resolution_clock::now();

    float time = std::chrono::duration<float, std::chrono::seconds::period>(current - start).count();

    UniformBufferObject ubo{};

    ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.view = glm::lookAt(
            glm::vec3(2.0f, 2.0f, 2.0f),
            glm::vec3(0.0f, 0.0f, 0.0f), 
            glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.proj = glm::perspective(
            glm::radians(45.0f), 
            ren->swap_chain_extent.width / (float) ren->swap_chain_extent.height, 
            0.1f, 
            10.0f);
    ubo.proj[1][1] *= -1;

    memcpy(ren->uniform_buffers_mapped[ren->current_frame], &ubo, sizeof(ubo));
}

extern "C" void ren_draw_frame(t_ren* ren) {
    vkWaitForFences(ren->device, 1, &ren->in_flight_fences[ren->current_frame], VK_TRUE, UINT64_MAX);

    uint32_t image_idx;
    VkResult result = vkAcquireNextImageKHR(
        ren->device, 
        ren->swap_chain, 
        UINT64_MAX, 
        ren->image_available_semaphores[ren->current_frame], 
        VK_NULL_HANDLE, 
        &image_idx);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        return vk::recreate_swap_chain(ren);
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("Failed to aquire swap chain image!");
    }

    update_uniform_buffer(ren);

    vkResetFences(ren->device, 1, &ren->in_flight_fences[ren->current_frame]);

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

    result = vkQueuePresentKHR(ren->present_queue, &present_info);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
        vk::recreate_swap_chain(ren);
    } else if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to present swap chain image!");
    }

    ren->current_frame = (ren->current_frame + 1) % MAX_FRAMES_IN_FLIGHT;
}

extern "C" void ren_destroy(t_ren* ren) {
    vkDeviceWaitIdle(ren->device);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroyBuffer(ren->device, ren->uniform_buffers[i], nullptr);
        vkFreeMemory(ren->device, ren->uniform_buffers_memory[i], nullptr);
    }

    vkDestroyDescriptorPool(ren->device, ren->descriptor_pool, nullptr);
    vkDestroyDescriptorSetLayout(ren->device, ren->descriptor_set_layout, nullptr);

    vk::destroy_swap_chain(ren); 

    vkDestroyBuffer(ren->device, ren->index_buffer, nullptr);
    vkFreeMemory(ren->device, ren->index_buffer_memory, nullptr);

    vkDestroyBuffer(ren->device, ren->vertex_buffer, nullptr);
    vkFreeMemory(ren->device, ren->vertex_buffer_memory, nullptr);

    vkDestroyPipeline(ren->device, ren->graphics_pipeline, nullptr);
    vkDestroyPipelineLayout(ren->device, ren->pipeline_layout, nullptr);

    vkDestroyRenderPass(ren->device, ren->render_pass, nullptr); 

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroySemaphore(ren->device, ren->image_available_semaphores[i], nullptr);
        vkDestroySemaphore(ren->device, ren->render_finished_semaphores[i], nullptr);
        vkDestroyFence(ren->device, ren->in_flight_fences[i], nullptr);
    }

    vkDestroyCommandPool(ren->device, ren->command_pool, nullptr);
    vkDestroyDevice(ren->device, nullptr);

    vkDestroySurfaceKHR(ren->instance,ren->surface, nullptr);
    vkDestroyInstance(ren->instance, nullptr);

    glfwDestroyWindow(ren->window);

    glfwTerminate();
}
