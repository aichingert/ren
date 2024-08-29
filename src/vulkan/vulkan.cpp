#include "../ren.h"

#include "device.cpp"
#include "instance.cpp"
#include "swap_chain.cpp"
#include "pipeline.cpp"

namespace vk {

void init(t_ren* ren, const char* title) {
    init_instance(ren, title);
    init_surface(ren);
    init_device(ren);
    init_swap_chain(ren);
    init_render_pass(ren);
    init_graphics_pipeline(ren);
}

}
