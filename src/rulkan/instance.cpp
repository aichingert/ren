#include "instance.hpp"
#include "util.hpp"
#include <cstring>

namespace rulkan {

bool are_validation_layers_supported() {
    uint32_t layer_count;
    vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

    std::vector<VkLayerProperties> available_layers(layer_count);
    vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

    for (const char *layer_name : VALIDATION_LAYERS) {
        bool layer_found = false;

        for (const auto& layer_props : available_layers)
            if (std::strcmp(layer_name, layer_props.layerName) == 0) {
                layer_found = true;
                break;
            }

        if (!layer_found) return false;
    }

    return true;
}

void init_instance(t_rulkan& rulkan, const char *title) {
    if (ENABLE_VALIDATION_LAYERS && !are_validation_layers_supported()) {
        VK_CHECK(VK_NOT_READY, "validation layers requested, but not available");
    }

    uint32_t glfw_extension_count = 0;
    const char **glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

    VkApplicationInfo app_info{};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName = title;
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.pEngineName = "No Engine";
    app_info.engineVersion = VK_MAKE_VERSION(0, 0, 1);
    app_info.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create_info.pApplicationInfo = &app_info; 

    create_info.enabledExtensionCount = glfw_extension_count;
    create_info.ppEnabledExtensionNames = glfw_extensions;

    if (ENABLE_VALIDATION_LAYERS) {
        create_info.enabledLayerCount = static_cast<uint32_t>(VALIDATION_LAYERS.size());
        create_info.ppEnabledLayerNames = VALIDATION_LAYERS.data();
    } else {
        create_info.enabledLayerCount = 0;
    }

    VK_CHECK(vkCreateInstance(&create_info, nullptr, &rulkan.instance), "Failed to create instance");
}


}
