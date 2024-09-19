#include "swapchain.hpp"

namespace rulkan {

t_swapchain_support_details query_swapchain_support(t_rulkan& rulkan, VkPhysicalDevice device) {
    t_swapchain_support_details details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, rulkan.surface, &details.capabilities);

    uint32_t format_count;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, rulkan.surface, &format_count, nullptr);

    if (format_count != 0) {
        details.formats.resize(format_count);
        vkGetPhysicalDeviceSurfaceFormatsKHR(
                device,
                rulkan.surface,
                &format_count, details.formats.data());
    }

    uint32_t present_mode_count;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, rulkan.surface, &present_mode_count, nullptr);

    if (present_mode_count != 0) {
        details.present_modes.resize(present_mode_count);
        vkGetPhysicalDeviceSurfacePresentModesKHR(
                device,
                rulkan.surface,
                &present_mode_count,
                details.present_modes.data());
    }

    return details;
}

}
