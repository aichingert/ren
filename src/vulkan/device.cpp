#include <stdexcept>
#include <vector>

#include "vulkan.h"
#include "queue.cpp"

namespace vk {

// TODO: rate device suitability instead of just picking the first one
bool is_device_suitable(VkPhysicalDevice device) {
    QueueFamilyIndices indices = find_queue_families(device);

    return indices.graphics_family.has_value();
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
        if (is_device_suitable(device)) {
            ren->physical_device = device;
            break;
        }
    }

    if (ren->physical_device == VK_NULL_HANDLE){
        throw std::runtime_error("Failed to find a suitable GPU!");
    }
}

void create_logical_device(t_ren* ren) {
    QueueFamilyIndices indices = find_queue_families(ren->physical_device);
    VkDeviceQueueCreateInfo queue_create_info{};
    queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_create_info.queueFamilyIndex = indices.graphics_family.value();
    queue_create_info.queueCount = 1;

    float queue_priority = 1.0f;

    queue_create_info.pQueuePriorities = &queue_priority;

    VkPhysicalDeviceFeatures device_features{};

    VkDeviceCreateInfo create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    create_info.pQueueCreateInfos = &queue_create_info;
    create_info.queueCreateInfoCount = 1;
    create_info.pEnabledFeatures = &device_features;

    if (vkCreateDevice(ren->physical_device, &create_info, nullptr, &ren->device) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device!");
    }

    vkGetDeviceQueue(ren->device, indices.graphics_family.value(), 0, &ren->graphics_queue);
}

void init_device(t_ren* ren) {
    pick_physical_device(ren);
    create_logical_device(ren);
}

}
