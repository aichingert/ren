#include "rulkan.h"

#include "util.hpp"
#include "instance.hpp"
#include "../window/window.hpp"
#include "device.hpp"
#include "swapchain.hpp"
#include "pipeline.hpp"
#include "command.hpp"
#include "sync.hpp"

namespace rulkan {

t_rulkan init(GLFWwindow *window, const char *title) {
    t_rulkan rulkan = {};

    init_instance(rulkan, title);
    window::init_surface(window, &rulkan);
    init_devices(rulkan);
    init_swapchain(rulkan, window);
    init_image_views(rulkan);
    init_render_pass(rulkan);
    init_framebuffers(rulkan, window);
    init_command_pools(rulkan);
    init_command_buffers(rulkan);
    init_sync_structures(rulkan);

    return rulkan;
}

void destroy(const t_rulkan& rulkan) {
    for (size_t i = 0; i < FRAME_OVERLAP; i++) {
        //VkDestroyFence(rulkan.device, rulkan.frames[i].render_fence, nullptr);
        //VkDestroySemaphore(rulkan.device, rulkan.frames[i].render_sema, nullptr);
        //VkDestroySemaphore(rulkan.device, rulkan.frames[i].present_sema, nullptr);
    }
}

}
