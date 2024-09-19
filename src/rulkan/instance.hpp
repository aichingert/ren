#ifndef INSTANCE_H
#define INSTANCE_H

#include "rulkan.h"

namespace rulkan {

bool are_validation_layers_supported();
void init_instance(t_rulkan& rulkan, const char *title);

}

#endif /* INSTANCE_H */
