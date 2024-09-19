#ifndef SWAPCHAIN_H
#define SWAPCHAIN_H

#include <vector>

#include "rulkan.h"

struct t_swapchain_support_details {
    VkSurfaceCapabilitiesKHR        capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR>   present_modes;
};

namespace rulkan {

t_swapchain_support_details query_swapchain_support(t_rulkan& rulkan, VkPhysicalDevice device);

}

#endif /* SWAPCHAIN_H */
