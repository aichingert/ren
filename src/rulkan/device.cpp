#include <set>
#include <stdexcept>

#include "device.hpp"
#include "util.hpp"

#include "queue.hpp"
#include "swapchain.hpp"

namespace rulkan {

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

bool is_device_suitable(t_rulkan& rulkan, VkPhysicalDevice device) {
    t_queue_family_indices indices = find_queue_families(rulkan, device);

    bool extensions_supported = check_device_extension_support(device);
    bool swapchain_adequate = false;

    if (extensions_supported) {
        t_swapchain_support_details swapchain_support = query_swapchain_support(rulkan, device);
        swapchain_adequate = !swapchain_support.formats.empty() && !swapchain_support.present_modes.empty();
    }

    return indices.is_complete() && extensions_supported && swapchain_adequate;
}

void pick_physical_device(t_rulkan& rulkan) {
    rulkan.physical_device = VK_NULL_HANDLE;

    uint32_t device_count = 0;
    vkEnumeratePhysicalDevices(rulkan.instance, &device_count, nullptr);

    if (device_count == 0) throw std::runtime_error("Failed to find GPUs with Vulkan support!");

    std::vector<VkPhysicalDevice> devices(device_count);
    vkEnumeratePhysicalDevices(rulkan.instance, &device_count, devices.data());

    for (const VkPhysicalDevice& device : devices) {
        if (is_device_suitable(rulkan, device)) {
            rulkan.physical_device = device;
            break;
        }
    }

    if (rulkan.physical_device == VK_NULL_HANDLE) throw std::runtime_error("Failed to find a suitable GPU!");
}

void init_logical_device(t_rulkan& rulkan) {
    t_queue_family_indices indices = find_queue_families(rulkan, rulkan.physical_device);

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

    // This feature needs to be activated to use
    // topology types that do not fill shapes
    VkPhysicalDeviceFeatures device_features{};
    device_features.fillModeNonSolid = true;

    VkDeviceCreateInfo create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    create_info.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size());
    create_info.pQueueCreateInfos = queue_create_infos.data();
    create_info.pEnabledFeatures = &device_features;
    create_info.enabledExtensionCount = static_cast<uint32_t>(DEVICE_EXTENSIONS.size());
    create_info.ppEnabledExtensionNames = DEVICE_EXTENSIONS.data();

    if (ENABLE_VALIDATION_LAYERS) {
        create_info.enabledLayerCount = static_cast<uint32_t>(VALIDATION_LAYERS.size());
        create_info.ppEnabledLayerNames = VALIDATION_LAYERS.data();
    } else {
        create_info.enabledLayerCount = 0;
    }

    VkResult res = vkCreateDevice(rulkan.physical_device, &create_info, nullptr, &rulkan.device);
    VK_CHECK(res, "Failed to create logical device!");

    vkGetDeviceQueue(rulkan.device, indices.graphics_family.value(), 0, &rulkan.graphics_queue);
    vkGetDeviceQueue(rulkan.device, indices.present_family.value(), 0, &rulkan.present_queue);
}

void init_devices(t_rulkan& rulkan) {
    pick_physical_device(rulkan);
    init_logical_device(rulkan);
}


}
