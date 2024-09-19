#include "rulkan.h"
#include "util.hpp"

namespace rulkan {

t_rulkan init() {
    VkResult result = VK_SUCCESS;

    VK_CHECK(result, "failed");
    return t_rulkan{};
}

void destroy(const t_rulkan& rulkan) {
    for (size_t i = 0; i < FRAME_OVERLAP; i++) {
        //VkDestroyFence(rulkan.device, rulkan.frames[i].render_fence, nullptr);
        //VkDestroySemaphore(rulkan.device, rulkan.frames[i].render_sema, nullptr);
        //VkDestroySemaphore(rulkan.device, rulkan.frames[i].present_sema, nullptr);
    }
}

}
