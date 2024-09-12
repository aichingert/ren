#ifndef LIB_VULKAN_H
#define LIB_VULKAN_H

#include <optional>
#include <vector>

#include "../ren.h"

const std::vector<const char*> VALIDATION_LAYERS = {
    "VK_LAYER_KHRONOS_validation",
};

#ifdef NDEBUG
    const bool ENABLE_VALIDATION_LAYERS = false;
#else
    const bool ENABLE_VALIDATION_LAYERS = true;
#endif

namespace vk {

void init(t_ren* ren, const char* title);

}

#endif /* LIB_VULKAN_H */
