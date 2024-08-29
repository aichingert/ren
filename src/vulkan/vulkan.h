#ifndef LIB_VULKAN_H
#define LIB_VULKAN_H

#include <optional>

#include "../ren.h"

struct QueueFamilyIndices {
    std::optional<uint32_t> graphics_family;
};

#endif /* LIB_VULKAN_H */
