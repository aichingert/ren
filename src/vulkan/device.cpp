#include <vector>

#include "../ren.h"

namespace vk {

// TODO: rate device suitability instead of just picking the first one
bool is_device_suitable(VkPhysicalDevice device) {
    VkPhysicalDeviceProperties device_properties;
    VkPhysicalDeviceFeatures device_features;

    vkGetPhysicalDeviceProperties(device, &device_properties);
    vkGetPhysicalDeviceFeatures(device, &device_features);

    return device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
           device_features.geometryShader;
}

void pick_physical_device(t_ren* ren) {
    ren->device = VK_NULL_HANDLE;

    uint32_t device_count = 0;
    vkEnumeratePhysicalDevices(ren->instance, &device_count, nullptr);

    if (device_count == 0) {
        throw std::runtime_error("Failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(device_count);
    vkEnumeratePhysicalDevices(ren->instance, &device_count, devices.data());

    for (const VkPhysicalDevice& device : devices) {
        if (is_device_suitable(device)) {
            ren->device = device;
            break;
        }
    }

    if (ren->device == VK_NULL_HANDLE){
        throw std::runtime_error("Failed to find a suitable GPU!");
    }
}

}
