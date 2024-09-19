#include "ren.h"
#include "window/window.h"

#include <cstring>

extern "C" t_ren ren_init(uint32_t width, uint32_t height, const char* title) {
    return t_ren{
        .window = window::init(width, height, title),
        .rulkan = rulkan::init(),
    };
}

extern "C" void ren_destroy(t_ren *ren) {
    window::destroy(ren);
    rulkan::destroy(ren->rulkan);
}
