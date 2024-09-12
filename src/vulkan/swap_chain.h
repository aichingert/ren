#ifndef SWAPCHAIN_H
#define SWAPCHAIN_H

#include "vulkan.h"

struct swap_chain_support_details_t {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> present_modes;
};

namespace vk {

swap_chain_support_details_t query_swap_chain_support(t_ren* ren, VkPhysicalDevice device);
VkSurfaceFormatKHR choose_swap_surface_format(const std::vector<VkSurfaceFormatKHR>& available_formats);
VkPresentModeKHR choose_swap_present_mode(const std::vector<VkPresentModeKHR>& available_present_modes);
VkExtent2D choose_swap_extent(t_ren* ren, const VkSurfaceCapabilitiesKHR& capabilities);

void init_framebuffers(t_ren* ren);
void init_image_views(t_ren* ren);
void init_swap_chain(t_ren* ren);
void init_surface(t_ren* ren);

void destroy_swap_chain(t_ren* ren);
void recreate_swap_chain(t_ren* ren);

}

#endif /* SWAPCHAIN_H */
