#include "swapchain.hpp"
#include "queue.hpp"
#include "util.hpp"

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

VkSurfaceFormatKHR choose_swap_surface_format(const std::vector<VkSurfaceFormatKHR>& formats) {
    for (const auto& available : formats) {
        if (available.format     == VK_FORMAT_B8G8R8A8_SRGB &&  
            available.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            return available;
    }

    return formats[0];
}

VkPresentModeKHR choose_swap_present_mode(const std::vector<VkPresentModeKHR> &modes) {
    for (const auto& available: modes) {
        if (available == VK_PRESENT_MODE_MAILBOX_KHR)
            return VK_PRESENT_MODE_MAILBOX_KHR;
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D choose_swap_extent(t_rulkan& rulkan, VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow *window) {
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
        return capabilities.currentExtent;

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    VkExtent2D actual_extent = { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };

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

void init_framebuffers(t_rulkan& rulkan, GLFWwindow *window) {
    size_t size = sizeof(VkFramebuffer) * rulkan.swapchain.size;
    rulkan.swapchain.framebuffers = static_cast<VkFramebuffer*>(malloc(size));

    for (size_t i = 0; i < rulkan.swapchain.size; i++) {
        VkImageView attachments[] = {rulkan.swapchain.image_views[i]};

        VkFramebufferCreateInfo framebuffer_info{};
        framebuffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebuffer_info.renderPass = rulkan.render_pass;
        framebuffer_info.attachmentCount = 1;
        framebuffer_info.pAttachments = attachments;
        framebuffer_info.width = rulkan.swapchain.extent.width;
        framebuffer_info.height = rulkan.swapchain.extent.height;
        framebuffer_info.layers = 1;

        VkResult res = vkCreateFramebuffer(
                rulkan.device,
                &framebuffer_info,
                nullptr,
                &rulkan.swapchain.framebuffers[i]);

        VK_CHECK(res, "Failed to create framebuffer!");
    }
}
void init_image_views(t_rulkan& rulkan) {
    size_t size = sizeof(VkImageView) * rulkan.swapchain.size;
    rulkan.swapchain.image_views = static_cast<VkImageView*>(malloc(size));

    for(size_t i = 0; i < rulkan.swapchain.size; i++) {
        VkImageViewCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        create_info.image = rulkan.swapchain.images[i];
        create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
        create_info.format = rulkan.swapchain.format;
        create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

        create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        create_info.subresourceRange.baseMipLevel = 0;
        create_info.subresourceRange.levelCount = 1;
        create_info.subresourceRange.baseArrayLayer = 0;
        create_info.subresourceRange.layerCount = 1;

        VkResult res = vkCreateImageView(
                rulkan.device, 
                &create_info, 
                nullptr, 
                &rulkan.swapchain.image_views[i]);
        VK_CHECK(res, "Failed to create image view!");
    }
}
void init_swapchain(t_rulkan& rulkan, GLFWwindow *window) {
    t_swapchain_support_details swapchain_support = query_swapchain_support(rulkan, rulkan.physical_device);
    VkSurfaceFormatKHR surface_format = choose_swap_surface_format(swapchain_support.formats);
    VkPresentModeKHR present_mode = choose_swap_present_mode(swapchain_support.present_modes);
    VkExtent2D extent = choose_swap_extent(rulkan, swapchain_support.capabilities, window);

    uint32_t image_count = swapchain_support.capabilities.minImageCount + 1;

    if (swapchain_support.capabilities.maxImageCount > 0
        && image_count > swapchain_support.capabilities.maxImageCount) {
        image_count = swapchain_support.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    create_info.surface = rulkan.surface;
    create_info.minImageCount = image_count;
    create_info.imageFormat = surface_format.format;
    create_info.imageColorSpace = surface_format.colorSpace;
    create_info.imageExtent = extent;
    create_info.imageArrayLayers = 1;
    create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    t_queue_family_indices indices = find_queue_families(rulkan, rulkan.physical_device);
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

    create_info.preTransform = swapchain_support.capabilities.currentTransform;
    create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    create_info.presentMode = present_mode;
    create_info.clipped = VK_TRUE;
    create_info.oldSwapchain = VK_NULL_HANDLE;

    VkResult res = vkCreateSwapchainKHR(rulkan.device, &create_info, nullptr, &rulkan.swapchain.self);
    VK_CHECK(res, "Failed to create swap chain!");

    vkGetSwapchainImagesKHR(rulkan.device, rulkan.swapchain.self, &image_count, nullptr);
    rulkan.swapchain.size = image_count;
    rulkan.swapchain.images = static_cast<VkImage*>(malloc(sizeof(VkImage) * image_count));
    vkGetSwapchainImagesKHR(rulkan.device, rulkan.swapchain.self, &image_count, rulkan.swapchain.images);

    rulkan.swapchain.format = surface_format.format;
    rulkan.swapchain.extent = extent;
}

void destroy_swapchain(t_rulkan& rulkan) {
    for (size_t i = 0; i < rulkan.swapchain.size; i++) {
        vkDestroyFramebuffer(rulkan.device, rulkan.swapchain.framebuffers[i], nullptr);
        vkDestroyImageView(rulkan.device, rulkan.swapchain.image_views[i], nullptr);
    }

    vkDestroySwapchainKHR(rulkan.device, rulkan.swapchain.self, nullptr);

    free(rulkan.swapchain.images);
    free(rulkan.swapchain.image_views);
    free(rulkan.swapchain.framebuffers);
}

void recreate_swapchain(t_rulkan& rulkan, GLFWwindow *window) {
    int width = 0;
    int height = 0;

    glfwGetFramebufferSize(window, &width, &height);

    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(window, &width, &height);
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(rulkan.device);
    destroy_swapchain(rulkan);

    init_swapchain(rulkan, window);
    init_image_views(rulkan);
    init_framebuffers(rulkan, window);
}


}
