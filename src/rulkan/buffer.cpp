#include <stdexcept>

#include "buffer.hpp"
#include "util.hpp"

namespace rulkan {

uint32_t find_memory_type(const t_rulkan& rulkan, uint32_t type_filter, VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties mem_properties;
    vkGetPhysicalDeviceMemoryProperties(rulkan.physical_device, &mem_properties);
    for (uint32_t i = 0; i < mem_properties.memoryTypeCount; i++) {
        uint32_t check = mem_properties.memoryTypes[i].propertyFlags & properties;

        if (type_filter & (1 << i) && check == properties) {
            return i;
        }
    }

    throw std::runtime_error("Failed to find suitable memory type!");
}

t_vertex_buffer init_vertex_buffer(const t_rulkan& rulkan) {
    VkDeviceSize buffer_size = sizeof(t_vertex) * 1024 * 1024 * 16; 
    t_vertex_buffer vb{};

    VkBufferCreateInfo buffer_info{};
    buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buffer_info.size = buffer_size;
    buffer_info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
    buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VkResult res = vkCreateBuffer(rulkan.device, &buffer_info, nullptr, &vb.buffer);
    VK_CHECK(res, "Failed to create vertex buffer!");

    VkMemoryRequirements mem_reqs;
    vkGetBufferMemoryRequirements(rulkan.device, vb.buffer, &mem_reqs);

    VkMemoryAllocateInfo alloc_info{};
    alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    alloc_info.allocationSize = mem_reqs.size;
    alloc_info.memoryTypeIndex = find_memory_type(
        rulkan, 
        mem_reqs.memoryTypeBits, 
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

    res = vkAllocateMemory(rulkan.device, &alloc_info, nullptr, &vb.memory);
    VK_CHECK(res, "Failed to create vertex buffer memory!");

    vkBindBufferMemory(rulkan.device, vb.buffer, vb.memory, 0);
    return vb;
}

}
