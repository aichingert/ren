#include "sync.hpp"
#include "util.hpp"

namespace rulkan {

void init_sync_structures(t_rulkan& rulkan) {
    VkResult res = VK_SUCCESS;
    VkSemaphoreCreateInfo semaphore_info{};
    semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fence_info{};
    fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (size_t i = 0; i < FRAME_OVERLAP; i++) {     
        res = vkCreateFence(rulkan.device, &fence_info, nullptr, &rulkan.frames[i].render_fence);
        VK_CHECK(res, "Failed to create render fence!");

        res = vkCreateSemaphore(rulkan.device, &semaphore_info, nullptr, &rulkan.frames[i].present_sema);
        VK_CHECK(res, "Failed to create present semaphore!");

        res = vkCreateSemaphore(rulkan.device, &semaphore_info, nullptr, &rulkan.frames[i].render_sema);
        VK_CHECK(res, "Failed to create render semaphore!");
	}
}

}
