#include <iostream>
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

void init_framebuffers(t_ren* ren) {
    size_t size = sizeof(VkFramebuffer) * ren->swap_chain_images_size;
    ren->swap_chain_framebuffers = static_cast<VkFramebuffer*>(malloc(size));

    for (size_t i = 0; i < ren->swap_chain_images_size; i++) {
        VkImageView attachments[] = {ren->swap_chain_image_views[i]};

        VkFramebufferCreateInfo framebuffer_info{};
        framebuffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebuffer_info.renderPass = ren->render_pass;
        framebuffer_info.attachmentCount = 1;
        framebuffer_info.pAttachments = attachments;
        framebuffer_info.width = ren->swap_chain_extent.width;
        framebuffer_info.height = ren->swap_chain_extent.height;
        framebuffer_info.layers = 1;

        VkResult result = vkCreateFramebuffer(
                ren->device, 
                &framebuffer_info, 
                nullptr, 
                &ren->swap_chain_framebuffers[i]);

        if (result != VK_SUCCESS) {
            throw std::runtime_error("Failed to create framebuffer!");
        }
    }
}

void init_image_views(t_ren* ren) {
    size_t size = sizeof(VkImageView) * ren->swap_chain_images_size;
    ren->swap_chain_image_views = static_cast<VkImageView*>(malloc(size));

    for (size_t i = 0; i < ren->swap_chain_images_size; i++) {
        VkImageViewCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        create_info.image = ren->swap_chain_images[i];
        create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
        create_info.format = ren->swap_chain_image_format;
        create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

        create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        create_info.subresourceRange.baseMipLevel = 0;
        create_info.subresourceRange.levelCount = 1;
        create_info.subresourceRange.baseArrayLayer = 0;
        create_info.subresourceRange.layerCount = 1;

        if (vkCreateImageView(ren->device, &create_info, nullptr, &ren->swap_chain_image_views[i]) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create image view!");
        }
    }
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

    t_queue_family_indices indices = find_queue_families(ren, ren->physical_device);
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
    ren->swap_chain_images_size = image_count;
    ren->swap_chain_images = static_cast<VkImage*>(malloc(sizeof(VkImage) * image_count));
    vkGetSwapchainImagesKHR(ren->device, ren->swap_chain, &image_count, ren->swap_chain_images);

    ren->swap_chain_image_format = surface_format.format;
    ren->swap_chain_extent = extent;
}

void init_surface(t_ren* ren) {
    if (glfwCreateWindowSurface(ren->instance, ren->window, nullptr, &ren->surface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface!");
    }
}

void destroy_swap_chain(t_ren* ren) {
    for (size_t i = 0; i < ren->swap_chain_images_size; i++) {
        vkDestroyFramebuffer(ren->device, ren->swap_chain_framebuffers[i], nullptr);
        vkDestroyImageView(ren->device, ren->swap_chain_image_views[i], nullptr);
    }

    vkDestroySwapchainKHR(ren->device, ren->swap_chain, nullptr);
}

void recreate_swap_chain(t_ren* ren) {
    int width = 0;
    int height = 0;

    glfwGetFramebufferSize(ren->window, &width, &height);

    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(ren->window, &width, &height);
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(ren->device);

    destroy_swap_chain(ren);
    
    init_swap_chain(ren);
    init_image_views(ren);
    init_framebuffers(ren);
}

}
