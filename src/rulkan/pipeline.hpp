#ifndef PIPELINE_H
#define PIPELINE_H

#include <vector>

#include "rulkan.h"

namespace rulkan {

void init_render_pass(t_rulkan& rulkan);
VkShaderModule init_shader_module(t_rulkan& rulkan, const std::vector<char>& code);
void init_graphics_pipeline(t_rulkan& rulkan);

}

#endif /* PIPELINE_H */

