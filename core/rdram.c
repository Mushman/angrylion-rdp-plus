//
// rdram.c: RDRAM memory interface
//

#include "rdram.h"
#include "core.h"

#define RDRAM_MASK 0x00ffffff

static struct plugin_api* plugin;

// pointer indexing limits for aliasing RDRAM reads and writes
static uint32_t idxlim8;
static uint32_t idxlim16;
static uint32_t idxlim32;

static uint32_t* rdram32;
static uint16_t* rdram16;
static uint8_t* rdram8;
static uint8_t* rdram_hidden;

void rdram_init(struct plugin_api* _plugin)
{
    plugin = _plugin;

    idxlim8 = plugin->get_rdram_size() - 1;
    idxlim16 = (idxlim8 >> 1) & 0xffffffu;
    idxlim32 = (idxlim8 >> 2) & 0xffffffu;

    rdram32 = (uint32_t*)plugin->get_rdram();
    rdram16 = (uint16_t*)plugin->get_rdram();
    rdram8 = plugin->get_rdram();
    rdram_hidden = plugin->get_rdram_hidden();
}

bool rdram_valid_idx8(uint32_t in)
{
    return in <= idxlim8;
}

bool rdram_valid_idx16(uint32_t in)
{
    return in <= idxlim16;
}

bool rdram_valid_idx32(uint32_t in)
{
    return in <= idxlim32;
}

uint8_t rdram_read_idx8(uint32_t in)
{
    in &= RDRAM_MASK;
    return rdram_valid_idx8(in) ? rdram8[in ^ BYTE_ADDR_XOR] : 0;
}

uint16_t rdram_read_idx16(uint32_t in)
{
    in &= RDRAM_MASK >> 1;
    return rdram_valid_idx16(in) ? rdram16[in ^ WORD_ADDR_XOR] : 0;
}

uint32_t rdram_read_idx32(uint32_t in)
{
    in &= RDRAM_MASK >> 2;
    return rdram_valid_idx32(in) ? rdram32[in] : 0;
}

void rdram_write_idx8(uint32_t in, uint8_t val)
{
    in &= RDRAM_MASK;
    if (in <= idxlim8) {
        rdram8[in ^ BYTE_ADDR_XOR] = val;
    }
}

void rdram_write_idx16(uint32_t in, uint16_t val)
{
    in &= RDRAM_MASK >> 1;
    if (in <= idxlim16) {
        rdram16[in ^ WORD_ADDR_XOR] = val;
    }
}

void rdram_write_idx32(uint32_t in, uint32_t val)
{
    in &= RDRAM_MASK >> 2;
    if (rdram_valid_idx32(in)) {
        rdram32[in] = val;
    }
}

void rdram_read_pair16(uint16_t* rdst, uint8_t* hdst, uint32_t in)
{
    in &= RDRAM_MASK >> 1;
    if (rdram_valid_idx16(in)) {
        *rdst = rdram16[in ^ WORD_ADDR_XOR];
        *hdst = rdram_hidden[in];
    } else {
        *rdst = *hdst = 0;
    }
}

void rdram_write_pair8(uint32_t in, uint8_t rval, uint8_t hval)
{
    in &= RDRAM_MASK;
    if (rdram_valid_idx8(in)) {
        rdram8[in ^ BYTE_ADDR_XOR] = rval;
        if (in & 1) {
            rdram_hidden[in >> 1] = hval;
        }
    }
}

void rdram_write_pair16(uint32_t in, uint16_t rval, uint8_t hval)
{
    in &= RDRAM_MASK >> 1;
    if (rdram_valid_idx16(in)) {
        rdram16[in ^ WORD_ADDR_XOR] = rval;
        rdram_hidden[in] = hval;
    }
}

void rdram_write_pair32(uint32_t in, uint32_t rval, uint8_t hval0, uint8_t hval1)
{
    in &= RDRAM_MASK >> 2;
    if (rdram_valid_idx32(in)) {
        rdram32[in] = rval;
        rdram_hidden[in << 1] = hval0;
        rdram_hidden[(in << 1) + 1] = hval1;
    }
}
