#pragma once

#include "core.h"

void plugin_retrace(struct plugin_api* api);
void plugin_set_rdram_size(uint32_t size);
uint32_t** plugin_get_vi_registers(void);
