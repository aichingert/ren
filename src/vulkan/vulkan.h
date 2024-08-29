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

struct QueueFamilyIndices {
    std::optional<uint32_t> present_family;
    std::optional<uint32_t> graphics_family;

    bool is_complete() {
        return present_family.has_value() && graphics_family.has_value();
    }
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> present_modes;
};

// TODO: find a better place for this
SwapChainSupportDetails query_swap_chain_support(t_ren* ren, VkPhysicalDevice device) {
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, ren->surface, &details.capabilities);

    uint32_t format_count;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, ren->surface, &format_count, nullptr);

    if (format_count != 0) {
        details.formats.resize(format_count);
        vkGetPhysicalDeviceSurfaceFormatsKHR(
                device,
                ren->surface,
                &format_count, details.formats.data());
    }

    uint32_t present_mode_count;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, ren->surface, &present_mode_count, nullptr);

    if (present_mode_count != 0) {
        details.present_modes.resize(present_mode_count);
        vkGetPhysicalDeviceSurfacePresentModesKHR(
                device,
                ren->surface,
                &present_mode_count,
                details.present_modes.data());
    }

    return details;
}

#endif /* LIB_VULKAN_H */
