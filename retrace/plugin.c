#include "plugin.h"

#include "core/rdp.h"

#include <memory.h>

#define RDRAM_MAX_SIZE 0x800000

static uint8_t rdram[RDRAM_MAX_SIZE];
static uint8_t rdram_hidden_bits[RDRAM_MAX_SIZE / 2];
static uint8_t dmem[0x1000];

static uint32_t dp_reg[DP_NUM_REG];
static uint32_t vi_reg[VI_NUM_REG];

static uint32_t* dp_reg_ptr[DP_NUM_REG];
static uint32_t* vi_reg_ptr[VI_NUM_REG];

static uint32_t rdram_size;

void plugin_init(void)
{
    for (int i = 0; i < DP_NUM_REG; i++) {
        dp_reg_ptr[i] = &dp_reg[i];
    }
    for (int i = 0; i < VI_NUM_REG; i++) {
        vi_reg_ptr[i] = &vi_reg[i];
    }
}

void plugin_sync_dp(void)
{
}

uint32_t** plugin_get_dp_registers(void)
{
    return dp_reg_ptr;
}

uint32_t** plugin_get_vi_registers(void)
{
    return vi_reg_ptr;
}

uint8_t* plugin_get_rdram(void)
{
    return rdram;
}

uint8_t* plugin_get_rdram_hidden(void)
{
    return rdram_hidden_bits;
}

uint32_t plugin_get_rdram_size(void)
{
    return rdram_size;
}

uint8_t* plugin_get_dmem(void)
{
    return dmem;
}

void plugin_close(void)
{
}

uint8_t* plugin_get_rom_header(void)
{
    return NULL;
}

void plugin_set_rdram_size(uint32_t size)
{
    rdram_size = size;
}
