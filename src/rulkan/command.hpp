#ifndef COMMAND_H
#define COMMAND_H

#include "rulkan.h"

namespace rulkan {

void init_command_pools(t_rulkan& rulkan);
void init_command_buffers(t_rulkan& rulkan);

void record_command_buffer(t_rulkan& rulkan, t_list& vec, VkCommandBuffer cmd, uint32_t frame, uint32_t image);

}

#endif /* COMMAND_H */
