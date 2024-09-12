
#ifndef BUFFERS_H
#define BUFFERS_H

#include <array>
#include <vector>
#include <glm/glm.hpp>

#include "vulkan.h"

// TODO: fix naming convention

struct vertex_t {
    glm::vec2 pos;
    glm::vec3 color;

    static VkVertexInputBindingDescription get_binding_description() {
        VkVertexInputBindingDescription binding_description{};
        binding_description.binding = 0;
        binding_description.stride = sizeof(vertex_t);
        binding_description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return binding_description;
    }

    static std::array<VkVertexInputAttributeDescription, 2> get_attribute_descriptions() {
        std::array<VkVertexInputAttributeDescription, 2> attribute_descriptions{};

        attribute_descriptions[0].binding = 0;
        attribute_descriptions[0].location = 0;
        attribute_descriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
        attribute_descriptions[0].offset = offsetof(vertex_t, pos);

        attribute_descriptions[1].binding = 0;
        attribute_descriptions[1].location = 1;
        attribute_descriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attribute_descriptions[1].offset = offsetof(vertex_t, color);

        return attribute_descriptions;
    }
};

struct uniform_buffer_object_t {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};

const std::vector<vertex_t> VERTICES = {
    {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
    {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
    {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
};

const std::vector<uint16_t> INDICES = {
    0, 1, 2, 2, 3, 0
};

namespace vk {

uint32_t find_memory_type(t_ren* ren, uint32_t type_filter, VkMemoryPropertyFlags properties);
void init_buffer(t_ren* ren, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &buffer, VkDeviceMemory& buffer_memory);

void copy_buffer(t_ren* ren, VkBuffer src, VkBuffer dst, VkDeviceSize size);
void init_vertex_buffer(t_ren* ren);
void init_index_buffer(t_ren* ren);
void init_uniform_buffers(t_ren* ren);


}

#endif /* BUFFERS_H */
