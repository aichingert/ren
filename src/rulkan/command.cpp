#include <vector>
#include <cstring>
#include <iostream>

#include "command.hpp"
#include "vertex.hpp"
#include "queue.hpp"
#include "util.hpp"

namespace rulkan {

void record_command_buffer(t_rulkan& rulkan, VkCommandBuffer cmd, uint32_t frame, uint32_t image) {
    std::cout << "joined" << std::endl;

    auto next = VERTICES[frame];

    void* data;
    vkMapMemory(rulkan.device, rulkan.frames[frame].vb.memory, 0, sizeof(next[0]) * next.size(), 0, &data);
    memcpy(data, next.data(), sizeof(next[0]) * next.size());
    vkUnmapMemory(rulkan.device, rulkan.frames[frame].vb.memory);

    VkCommandBufferBeginInfo begin_info {};
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    VK_CHECK(vkBeginCommandBuffer(cmd, &begin_info), "Failed to record command buffer!");

    VkRenderPassBeginInfo render_pass_info{};
    render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    render_pass_info.renderPass = rulkan.render_pass;
    render_pass_info.framebuffer = rulkan.swapchain.framebuffers[image];
    render_pass_info.renderArea.offset = {0, 0};
    render_pass_info.renderArea.extent = rulkan.swapchain.extent;

    VkClearValue clear_color = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
    render_pass_info.clearValueCount = 1;
    render_pass_info.pClearValues = &clear_color;

    vkCmdBeginRenderPass(cmd, &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);
    vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, rulkan.graphics_pipeline);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(rulkan.swapchain.extent.width);
    viewport.height = static_cast<float>(rulkan.swapchain.extent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(cmd, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = rulkan.swapchain.extent;
    vkCmdSetScissor(cmd, 0, 1, &scissor);

    VkBuffer vertex_buffers[] = {rulkan.frames[frame].vb.buffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(cmd, 0, 1, vertex_buffers, offsets);

    vkCmdDraw(cmd, static_cast<uint32_t>(next.size()), 1, 0, 0);

    vkCmdEndRenderPass(cmd);

    VK_CHECK(vkEndCommandBuffer(cmd), "Failed to record command buffer!");
}

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
    for (size_t i = 0; i < FRAME_OVERLAP; i++) {
        VkCommandBufferAllocateInfo alloc_info{};
        alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        alloc_info.commandBufferCount = 1;
        alloc_info.commandPool = rulkan.frames[i].command_pool;

        VkResult res = vkAllocateCommandBuffers(rulkan.device, &alloc_info, &rulkan.frames[i].command_buffer);
        VK_CHECK(res, "Failed to create frame command buffers!");
    }
}

}
