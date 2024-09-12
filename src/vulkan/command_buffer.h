#ifndef COMMAND_BUFFER_H
#define COMMAND_BUFFER_H

#include <stdexcept>
#include "vulkan.h"

namespace vk {

void record_command_buffer(t_ren* ren, VkCommandBuffer command_buffer, uint32_t image_idx);
void init_command_pool(t_ren* ren);
void init_command_buffer(t_ren* ren);

}

#endif /* COMMAND_BUFFER_H */
