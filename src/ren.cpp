#include "ren.h"
#include "window/window.hpp"

#include <cstring>

extern "C" t_ren ren_init(uint32_t width, uint32_t height, const char* title) {
    auto window = window::init(width, height, title);
    auto rulkan = rulkan::init(window, title);

    return t_ren{
        .window = window,
        .rulkan = rulkan,
    };
}

extern "C" void ren_draw(t_ren *ren) {
    rulkan::draw(ren->rulkan, ren->window, ren->frame);
    ren->frame = (ren->frame + 1) % FRAME_OVERLAP;
}

extern "C" void ren_destroy(t_ren *ren) {
    window::destroy(ren);
    rulkan::destroy(ren->rulkan);
}
