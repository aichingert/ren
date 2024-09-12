#include <stdexcept>
#include <set>
#include <vector>

#include "vulkan.h"
#include "swap_chain.h"
#include "queue.h"

namespace vk {

const std::vector<const char*> DEVICE_EXTENSIONS = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
};

bool check_device_extension_support(VkPhysicalDevice device) {
    uint32_t extension_count = 0;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, nullptr);

    std::vector<VkExtensionProperties> available_extensions(extension_count);
    vkEnumerateDeviceExtensionProperties(
            device,
            nullptr,
            &extension_count,
            available_extensions.data());

    std::set<std::string> required_extensions(DEVICE_EXTENSIONS.begin(), DEVICE_EXTENSIONS.end());

    for (const auto& extension : available_extensions) {
        required_extensions.erase(extension.extensionName);
    }

    return required_extensions.empty();
}

bool is_device_suitable(t_ren* ren, VkPhysicalDevice device) {
    t_queue_family_indices indices = find_queue_families(ren, device);

    bool extensions_supported = check_device_extension_support(device);
    bool swap_chain_adequate = false;

    if (extensions_supported) {
        swap_chain_support_details_t swap_chain_support = query_swap_chain_support(ren, device);
        swap_chain_adequate = !swap_chain_support.formats.empty()
            && !swap_chain_support.present_modes.empty();
    }

    return indices.is_complete() && extensions_supported && swap_chain_adequate;
}

void pick_physical_device(t_ren* ren) {
    ren->physical_device = VK_NULL_HANDLE;

    uint32_t device_count = 0;
    vkEnumeratePhysicalDevices(ren->instance, &device_count, nullptr);

    if (device_count == 0) {
        throw std::runtime_error("Failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(device_count);
    vkEnumeratePhysicalDevices(ren->instance, &device_count, devices.data());

    for (const VkPhysicalDevice& device : devices) {
        if (is_device_suitable(ren, device)) {
            ren->physical_device = device;
            break;
        }
    }

    if (ren->physical_device == VK_NULL_HANDLE){
        throw std::runtime_error("Failed to find a suitable GPU!");
    }
}

void create_logical_device(t_ren* ren) {
    t_queue_family_indices indices = find_queue_families(ren, ren->physical_device);

    std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
    std::set<uint32_t> unique_queue_families = {
        indices.graphics_family.value(),
        indices.present_family.value()
    };

    float queue_priority = 1.0f;

    for (uint32_t queue_family : unique_queue_families) {
        VkDeviceQueueCreateInfo queue_create_info{};
        queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_create_info.queueFamilyIndex = queue_family;
        queue_create_info.queueCount = 1;
        queue_create_info.pQueuePriorities = &queue_priority;
        queue_create_infos.push_back(queue_create_info);
    }

    VkPhysicalDeviceFeatures device_features{
        .fillModeNonSolid = true,
    };

    VkDeviceCreateInfo create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    create_info.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size());
    create_info.pQueueCreateInfos = queue_create_infos.data();
    create_info.pEnabledFeatures = &device_features;
    create_info.enabledExtensionCount = static_cast<uint32_t>(DEVICE_EXTENSIONS.size());
    create_info.ppEnabledExtensionNames = DEVICE_EXTENSIONS.data();

    // HERE
    if (ENABLE_VALIDATION_LAYERS) {
        create_info.enabledLayerCount = static_cast<uint32_t>(VALIDATION_LAYERS.size());
        create_info.ppEnabledLayerNames = VALIDATION_LAYERS.data();
    } else {
        create_info.enabledLayerCount = 0;
    }


    if (vkCreateDevice(ren->physical_device, &create_info, nullptr, &ren->device) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device!");
    }

    vkGetDeviceQueue(ren->device, indices.graphics_family.value(), 0, &ren->graphics_queue);
    vkGetDeviceQueue(ren->device, indices.present_family.value(), 0, &ren->present_queue);
}

void init_device(t_ren* ren) {
    pick_physical_device(ren);
    create_logical_device(ren);
}

}
