#include "device.cpp"
#include "instance.cpp"
#include "buffers.h"
#include "pipeline.cpp"

#include "command_buffer.h"
#include "sync.h"
#include "swap_chain.h"

namespace vk {

void init(t_ren* ren, const char* title) {
    init_instance(ren, title);
    init_surface(ren);
    init_device(ren);
    init_swap_chain(ren);
    init_image_views(ren);
    init_render_pass(ren);
    init_descriptor_set_layout(ren);
    init_graphics_pipeline(ren);
    init_framebuffers(ren);
    init_command_pool(ren);
    init_vertex_buffer(ren);
    init_index_buffer(ren);
    init_uniform_buffers(ren);
    init_descriptor_pool(ren);
    init_descriptor_sets(ren);
    init_command_buffer(ren);
    init_sync_objects(ren);
}

}
