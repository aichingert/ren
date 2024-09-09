#include <stdexcept>
#include <cstring>

#include "vulkan.h"
#include "vertex.h"

namespace vk {

uint32_t find_memory_type(t_ren* ren, uint32_t type_filter, VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties mem_properties;
    vkGetPhysicalDeviceMemoryProperties(ren->physical_device, &mem_properties);

    for (uint32_t i = 0; i < mem_properties.memoryTypeCount; i++) {
        uint32_t check = mem_properties.memoryTypes[i].propertyFlags & properties;

        if (type_filter & (1 << i) && check == properties) {
            return i;
        }
    }

    throw std::runtime_error("Failed to find suitable memory type!");
}

void create_buffer(
        t_ren* ren,
        VkDeviceSize size, 
        VkBufferUsageFlags usage, 
        VkMemoryPropertyFlags properties, 
        VkBuffer &buffer, 
        VkDeviceMemory& buffer_memory
) {
    VkBufferCreateInfo buffer_info{};
    buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buffer_info.size = size;
    buffer_info.usage = usage;
    buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(ren->device, &buffer_info, nullptr, &buffer) != VK_SUCCESS) {
        std::runtime_error("Failed to create buffer!");
    }

    VkMemoryRequirements mem_reqs;
    vkGetBufferMemoryRequirements(ren->device, buffer, &mem_reqs);

    VkMemoryAllocateInfo alloc_info{};
    alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    alloc_info.allocationSize = mem_reqs.size;
    alloc_info.memoryTypeIndex = find_memory_type(ren, mem_reqs.memoryTypeBits, properties);

    if (vkAllocateMemory(ren->device, &alloc_info, nullptr, &buffer_memory) != VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate buffer memory!");
    }

    vkBindBufferMemory(ren->device, buffer, buffer_memory, 0);
}

void copy_buffer(t_ren* ren, VkBuffer src, VkBuffer dst, VkDeviceSize size) {
    VkCommandBufferAllocateInfo alloc_info{};
    alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    
    // TODO: create another command pool with VK_COMMAND_POOL_CREATE_TRANSIENT_BIT flag for perf.
    alloc_info.commandPool = ren->command_pool;
    alloc_info.commandBufferCount = 1;

    VkCommandBuffer command_buffer;
    vkAllocateCommandBuffers(ren->device, &alloc_info, &command_buffer);

    VkCommandBufferBeginInfo begin_info{};
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(command_buffer, &begin_info);

    VkBufferCopy copy_region{};
    copy_region.size = size;
    vkCmdCopyBuffer(command_buffer, src, dst, 1, &copy_region);
    vkEndCommandBuffer(command_buffer);

    VkSubmitInfo submit_info{};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &command_buffer;

    vkQueueSubmit(ren->graphics_queue, 1, &submit_info, VK_NULL_HANDLE);
    vkQueueWaitIdle(ren->graphics_queue);
    vkFreeCommandBuffers(ren->device, ren->command_pool, 1, &command_buffer);
}

void init_vertex_buffer(t_ren* ren) {
    VkDeviceSize buffer_size = sizeof(VERTICES[0]) * VERTICES.size();
    VkBuffer staging_buffer;
    VkDeviceMemory staging_buffer_memory;

    create_buffer(
            ren,
            buffer_size, 
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
            staging_buffer, 
            staging_buffer_memory);

    void* data;
    vkMapMemory(ren->device, staging_buffer_memory, 0, buffer_size, 0, &data);
    memcpy(data, VERTICES.data(), (size_t)buffer_size);
    vkUnmapMemory(ren->device, staging_buffer_memory);

    create_buffer(
            ren, 
            buffer_size, 
            VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, 
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            ren->vertex_buffer, 
            ren->vertex_buffer_memory);

    copy_buffer(ren, staging_buffer, ren->vertex_buffer, buffer_size);

    vkDestroyBuffer(ren->device, staging_buffer, nullptr);
    vkFreeMemory(ren->device, staging_buffer_memory, nullptr);
}

void init_index_buffer(t_ren* ren) {
    VkDeviceSize buffer_size = sizeof(INDICES[0]) * INDICES.size();

    VkBuffer staging_buffer;
    VkDeviceMemory staging_buffer_memory;
    create_buffer(
            ren,
            buffer_size, 
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
            staging_buffer, 
            staging_buffer_memory);

    void* data;
    vkMapMemory(ren->device, staging_buffer_memory, 0, buffer_size, 0, &data);
    memcpy(data, INDICES.data(), (size_t) buffer_size);
    vkUnmapMemory(ren->device, staging_buffer_memory);

    create_buffer(
            ren,
            buffer_size, 
            VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, 
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
            ren->index_buffer, 
            ren->index_buffer_memory);

    copy_buffer(ren, staging_buffer, ren->index_buffer, buffer_size);

    vkDestroyBuffer(ren->device, staging_buffer, nullptr);
    vkFreeMemory(ren->device, staging_buffer_memory, nullptr);
}

void init_uniform_buffers(t_ren* ren) {
    VkDeviceSize buffer_size = sizeof(UniformBufferObject);

    ren->uniform_buffers = static_cast<VkBuffer*>(malloc(sizeof(VkBuffer) * MAX_FRAMES_IN_FLIGHT));
    ren->uniform_buffers_mapped = static_cast<void**>(malloc(sizeof(void*) * MAX_FRAMES_IN_FLIGHT));
    ren->uniform_buffers_memory = static_cast<VkDeviceMemory*>(
        malloc(sizeof(VkDeviceMemory) * MAX_FRAMES_IN_FLIGHT)
    );

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        create_buffer(
                ren,
                buffer_size, 
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                ren->uniform_buffers[i], 
                ren->uniform_buffers_memory[i]);

        vkMapMemory(
                ren->device, 
                ren->uniform_buffers_memory[i], 
                0, 
                buffer_size, 
                0, 
                &ren->uniform_buffers_mapped[i]);
    }
}

}
