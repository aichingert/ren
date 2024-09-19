#ifndef DEVICE_H
#define DEVICE_H

#include "rulkan.h"
#include <vector>

const std::vector<const char*> DEVICE_EXTENSIONS = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
};

namespace rulkan {

void ckeck_device_extension_support(VkPhysicalDevice device);
bool is_device_suitable(t_rulkan& rulkan, VkPhysicalDevice device);
void pick_physical_device(t_rulkan& rulkan);
void init_logical_device(t_rulkan& rulkan);
void init_devices(t_rulkan& rulkan);

}

#endif /* DEVICE_H */
