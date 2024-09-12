#include <stdexcept>

#include "queue.h"
#include "buffers.h"

namespace vk {

void record_command_buffer(t_ren* ren, VkCommandBuffer command_buffer, uint32_t image_idx) {
    VkCommandBufferBeginInfo begin_info{};

    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(command_buffer, &begin_info) != VK_SUCCESS) {
        throw std::runtime_error("Failed to begin recording command buffer!");
    }

    VkRenderPassBeginInfo render_pass_info{};
    render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    render_pass_info.renderPass = ren->render_pass;
    render_pass_info.framebuffer = ren->swap_chain_framebuffers[image_idx];
    render_pass_info.renderArea.offset = {0, 0};
    render_pass_info.renderArea.extent = ren->swap_chain_extent;

    VkClearValue clear_color = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
    render_pass_info.clearValueCount = 1;
    render_pass_info.pClearValues = &clear_color;

    vkCmdBeginRenderPass(command_buffer, &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);
    vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ren->graphics_pipeline);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(ren->swap_chain_extent.width);
    viewport.height = static_cast<float>(ren->swap_chain_extent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(command_buffer, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = ren->swap_chain_extent;
    vkCmdSetScissor(command_buffer, 0, 1, &scissor);

    VkBuffer vertex_buffers[] = {ren->vertex_buffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(command_buffer, 0, 1, vertex_buffers, offsets);

    vkCmdBindIndexBuffer(command_buffer, ren->index_buffer, 0, VK_INDEX_TYPE_UINT16);

    vkCmdBindDescriptorSets(
            command_buffer,
            VK_PIPELINE_BIND_POINT_GRAPHICS,
            ren->pipeline_layout,
            0,
            1,
            &ren->descriptor_sets[ren->current_frame],
            0,
            nullptr);
    vkCmdDrawIndexed(command_buffer, static_cast<uint32_t>(INDICES.size()), 1, 0, 0, 0);
    vkCmdEndRenderPass(command_buffer);

    if (vkEndCommandBuffer(command_buffer) != VK_SUCCESS) {
        throw std::runtime_error("Failed to record command buffer!");
    }
}

void init_command_pool(t_ren* ren) {
    t_queue_family_indices queue_family_indices = find_queue_families(ren, ren->physical_device);

    VkCommandPoolCreateInfo pool_info{};
    pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    pool_info.queueFamilyIndex = queue_family_indices.graphics_family.value();

    if (vkCreateCommandPool(ren->device, &pool_info, nullptr, &ren->command_pool) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create command pool!");
    }
}

void init_command_buffer(t_ren* ren) {
    VkCommandBufferAllocateInfo alloc_info{};
    alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    alloc_info.commandPool = ren->command_pool;
    alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    alloc_info.commandBufferCount = MAX_FRAMES_IN_FLIGHT;

    size_t size = MAX_FRAMES_IN_FLIGHT * sizeof(VkCommandBuffer);
    ren->command_buffers = static_cast<VkCommandBuffer*>(malloc(size));

    if (vkAllocateCommandBuffers(ren->device, &alloc_info, ren->command_buffers) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create command buffers!");
    }
}

}
