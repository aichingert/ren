#ifndef VERTEX_H
#define VERTEX_H

#include <array>
#include <vector>
#include <glm/glm.hpp>

#include "rulkan.h"

struct t_vertex {
    glm::vec2 pos;
    glm::vec3 color;

    static VkVertexInputBindingDescription get_binding_description() {
        VkVertexInputBindingDescription binding_description{};
        binding_description.binding = 0;
        binding_description.stride = sizeof(t_vertex);
        binding_description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return binding_description;
    }

    static std::array<VkVertexInputAttributeDescription, 2> get_attribute_descriptions() {
        std::array<VkVertexInputAttributeDescription, 2> attribute_descriptions{};

        attribute_descriptions[0].binding = 0;
        attribute_descriptions[0].location = 0;
        attribute_descriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
        attribute_descriptions[0].offset = offsetof(t_vertex, pos);

        attribute_descriptions[1].binding = 0;
        attribute_descriptions[1].location = 1;
        attribute_descriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attribute_descriptions[1].offset = offsetof(t_vertex, color);

        return attribute_descriptions;
    }
};

const std::vector<t_vertex> TRI = {
    {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
    {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
};

const std::vector<t_vertex> QUAD = {
    {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}},
    {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
    {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
};

const std::vector<t_vertex> VERTICES[2] = {
    TRI,
    QUAD
};

namespace rulkan {
}

#endif /* VERTEX_H */
