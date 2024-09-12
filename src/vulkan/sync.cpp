#include <stdexcept>
#include "sync.h"

namespace vk {

void init_sync_objects(t_ren* ren)  {
    VkSemaphoreCreateInfo semaphore_info{};
    semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fence_info{};
    fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    size_t fences_size = sizeof(VkFence) * MAX_FRAMES_IN_FLIGHT;
    size_t semaphores_size = sizeof(VkSemaphore) * MAX_FRAMES_IN_FLIGHT;

    ren->in_flight_fences = static_cast<VkFence*>(malloc(fences_size));
    ren->image_available_semaphores = static_cast<VkSemaphore*>(malloc(semaphores_size));
    ren->render_finished_semaphores = static_cast<VkSemaphore*>(malloc(semaphores_size));

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        if (vkCreateSemaphore(
                ren->device,
                &semaphore_info,
                nullptr,
                &ren->image_available_semaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(
                ren->device,
                &semaphore_info,
                nullptr,
                &ren->render_finished_semaphores[i]) != VK_SUCCESS ||
            vkCreateFence(
                ren->device,
                &fence_info,
                nullptr,
                &ren->in_flight_fences[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create synchronization objects for a frame!");
        }
    }
}

}
