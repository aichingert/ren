#ifndef UTIL_H
#define UTIL_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <vector>

const std::vector<const char*> VALIDATION_LAYERS = {
    "VK_LAYER_KHRONOS_validation",
};

#ifdef NDEBUG
    const bool ENABLE_VALIDATION_LAYERS = false;
#else
    const bool ENABLE_VALIDATION_LAYERS = true;
#endif

#define VK_CHECK(X, MSG)          \
	if (X != VK_SUCCESS) {        \
		std::runtime_error(MSG);  \
	}

#endif /* UTIL_H */
