#ifndef UTIL_H
#define UTIL_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>

#define VK_CHECK(X, MSG)          \
	if (X != VK_SUCCESS) {        \
		std::runtime_error(MSG);  \
	}

#endif /* UTIL_H */
