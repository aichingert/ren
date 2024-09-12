#ifndef QUEUE_H
#define QUEUE_H

#include <vector>
#include <optional>

#include "vulkan.h"

struct t_queue_family_indices {
    std::optional<uint32_t> present_family;
    std::optional<uint32_t> graphics_family;

    bool is_complete() {
        return present_family.has_value() && graphics_family.has_value();
    }
};

namespace vk {

t_queue_family_indices find_queue_families(t_ren* ren, VkPhysicalDevice device);

}

#endif /* QUEUE_H */
