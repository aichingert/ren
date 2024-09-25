#include <cstring>
#include <cstdlib>
#include <stdexcept>

#include "ren.h"
#include "window/window.hpp"

extern "C" t_ren ren_init(uint32_t width, uint32_t height, const char* title) {
    auto window = window::init(width, height, title);
    auto rulkan = rulkan::init(window, title);

    return t_ren{
        .window = window,
        .rulkan = rulkan,

        .vertices = {0},
        .frame = 0,
    };
}

void resize_list(t_ren *ren, size_t size) {
    if (ren->vertices.data == nullptr) {
        ren->vertices.data = (vertex*)calloc(12, sizeof(vertex));
        ren->vertices.cap = 12;
        return;
    }

    void* res = realloc(ren->vertices.data, size * sizeof(vertex));

    if (res == nullptr) {
        std::runtime_error("unable to allocate more");
    }
}

extern "C" void ren_draw_triangle(t_ren *ren, t_vec2 a, t_vec2 b, t_vec2 c) {
    size_t& pos = ren->vertices.size;

    if (pos + 3 > ren->vertices.cap) {
        ren->vertices.cap *= 12;
        resize_list(ren, ren->vertices.cap);
    }

    ren->vertices.data[pos++] = { .x = a.x, .y = a.y, .r = 0.5, .g = 0.5, .b = 0.5 };
    ren->vertices.data[pos++] = { .x = b.x, .y = b.y, .r = 0.5, .g = 0.5, .b = 0.5 };
    ren->vertices.data[pos++] = { .x = c.x, .y = c.y, .r = 0.5, .g = 0.5, .b = 0.5 };
}

extern "C" void ren_draw_frame(t_ren *ren) {
    glfwPollEvents();
    rulkan::draw(ren->rulkan, ren->vertices, ren->window, ren->frame);
    ren->frame = (ren->frame + 1) % FRAME_OVERLAP;
}

extern "C" void ren_destroy(t_ren *ren) {
    window::destroy(ren);
    //rulkan::destroy(ren->rulkan);
}
