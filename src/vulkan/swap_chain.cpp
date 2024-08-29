#include <stdexcept>
#include <vector>
#include <limits>
#include <cstdint>
#include <algorithm>

#include "vulkan.h"
#include "queue.h"

namespace vk {

VkSurfaceFormatKHR choose_swap_surface_format(
        const std::vector<VkSurfaceFormatKHR>& available_formats
) {
    for (const auto& available_format : available_formats) {
        if (available_format.format == VK_FORMAT_B8G8R8A8_SRGB
            && available_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
        ) {
            return available_format;
        }
    }

    return available_formats[0];
}

VkPresentModeKHR choose_swap_present_mode(
        const std::vector<VkPresentModeKHR>& available_present_modes
) {
    for (const auto& available_present_mode : available_present_modes) {
        if (available_present_mode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return VK_PRESENT_MODE_MAILBOX_KHR;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D choose_swap_extent(t_ren* ren, const VkSurfaceCapabilitiesKHR& capabilities) {
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    }

    int width, height;
    glfwGetFramebufferSize(ren->window, &width, &height);

    VkExtent2D actual_extent = {
        static_cast<uint32_t>(width),
        static_cast<uint32_t>(height),
    };

    actual_extent.width = std::clamp(
            actual_extent.width,
            capabilities.minImageExtent.width,
            capabilities.maxImageExtent.width);
    actual_extent.height = std::clamp(
            actual_extent.height,
            capabilities.minImageExtent.height,
            capabilities.maxImageExtent.height);

    return actual_extent;
}

void init_swap_chain(t_ren* ren) {
    SwapChainSupportDetails swap_chain_support = query_swap_chain_support(ren, ren->physical_device);
    VkSurfaceFormatKHR surface_format = choose_swap_surface_format(swap_chain_support.formats);
    VkPresentModeKHR present_mode = choose_swap_present_mode(swap_chain_support.present_modes);
    VkExtent2D extent = choose_swap_extent(ren, swap_chain_support.capabilities);

    uint32_t image_count = swap_chain_support.capabilities.minImageCount + 1;

    if (swap_chain_support.capabilities.maxImageCount > 0
            && image_count > swap_chain_support.capabilities.maxImageCount) {
        image_count = swap_chain_support.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    create_info.surface = ren->surface;
    create_info.minImageCount = image_count;
    create_info.imageFormat = surface_format.format;
    create_info.imageColorSpace = surface_format.colorSpace;
    create_info.imageExtent = extent;
    create_info.imageArrayLayers = 1;
    create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices = find_queue_families(ren, ren->physical_device);
    uint32_t queue_family_indices[] = {
        indices.graphics_family.value(),
        indices.present_family.value()
    };

    if (indices.graphics_family != indices.present_family) {
        create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        create_info.queueFamilyIndexCount = 2;
        create_info.pQueueFamilyIndices = queue_family_indices;
    } else {
        create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    create_info.preTransform = swap_chain_support.capabilities.currentTransform;
    create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    create_info.presentMode = present_mode;
    create_info.clipped = VK_TRUE;
    create_info.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(ren->device, &create_info, nullptr, &ren->swap_chain) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create swap chain!");
    }

    vkGetSwapchainImagesKHR(ren->device, ren->swap_chain, &image_count, nullptr);
    std::vector<VkImage> swap_chain_images(image_count);
    vkGetSwapchainImagesKHR(ren->device, ren->swap_chain, &image_count, swap_chain_images.data());

    ren->swap_chain_images = swap_chain_images.data();
    ren->swap_chain_images_size = swap_chain_images.size();
    ren->swap_chain_format = surface_format.format;
    ren->swap_chain_extent = extent;
}

void init_surface(t_ren* ren) {
    if (glfwCreateWindowSurface(ren->instance, ren->window, nullptr, &ren->surface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface!");
    }
}

}
