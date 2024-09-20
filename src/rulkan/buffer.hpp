#ifndef BUFFER_H
#define BUFFER_H

#include "vertex.hpp"

namespace rulkan {


uint32_t find_memory_type(const t_rulkan& ruklan, uint32_t type_filter, VkMemoryPropertyFlags properties);
t_vertex_buffer init_vertex_buffer(const t_rulkan& rulkan);

}


#endif /* BUFFER_H */
