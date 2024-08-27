#include "../ren.h"

#include "instance.cpp"
#include "device.cpp"

namespace vk {

void init(t_ren* ren, const char* title) {
    vk::init_instance(ren, title);
    vk::pick_physical_device(ren);
}

}
