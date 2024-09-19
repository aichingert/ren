#include "command.hpp"
#include "queue.hpp"
#include "util.hpp"

namespace rulkan {

void init_command_pools(t_rulkan& rulkan) {
    VkResult res = VK_SUCCESS;
    t_queue_family_indices queue_family_indices = find_queue_families(rulkan, rulkan.physical_device);

    VkCommandPoolCreateInfo pool_info{};
    pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    pool_info.queueFamilyIndex = queue_family_indices.graphics_family.value();

    for(size_t i = 0; i < FRAME_OVERLAP; i++) {
        res = vkCreateCommandPool(rulkan.device, &pool_info, nullptr, &rulkan.frames[i].command_pool);
        VK_CHECK(res, "Failed to create frame command pool!");
    }
}

void init_command_buffers(t_rulkan& rulkan) {
    VkResult res = VK_SUCCESS;
    VkCommandBufferAllocateInfo alloc_info{};
    alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    alloc_info.commandBufferCount = 1;

    for (size_t i = 0; i < FRAME_OVERLAP; i++) {
        alloc_info.commandPool = rulkan.frames[i].command_pool;
        res = vkAllocateCommandBuffers(rulkan.device, &alloc_info, &rulkan.frames[i].command_buffer);
        VK_CHECK(res, "Failed to create frame command buffers!");
    }
}

}
