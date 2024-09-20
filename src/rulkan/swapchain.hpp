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
VkSurfaceFormatKHR choose_swap_surface_format(const std::vector<VkSurfaceFormatKHR>& formats);
VkPresentModeKHR choose_swap_present_mode(const std::vector<VkPresentModeKHR>& modes);
VkExtent2D choose_swap_extent(t_rulkan& rulkan, VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow *window);
void init_framebuffers(t_rulkan& rulkan, GLFWwindow *window);
void init_image_views(t_rulkan& rulkan);
void init_swapchain(t_rulkan& rulkan, GLFWwindow *window);

void destroy_swapchain(t_rulkan& rulkan);
void recreate_swapchain(t_rulkan& rulkan, GLFWwindow *window);

}

#endif /* SWAPCHAIN_H */
