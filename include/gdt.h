#ifndef _GDT_H
# define _GDT_H

#include "stdint.h"

#define SEG_DESCTYPE(x)  ((x) << 0x04) // 0 = system, 1 = code/data
#define SEG_PRES(x)      ((x) << 0x07) // Present
#define SEG_SAVL(x)      ((x) << 0x0C) // Available
#define SEG_LONG(x)      ((x) << 0x0D) // Long mode (64-bit)
#define SEG_SIZE(x)      ((x) << 0x0E) // 0 = 16-bit, 1 = 32-bit
#define SEG_GRAN(x)      ((x) << 0x0F) // 0 = byte, 1 = 4 KiB
#define SEG_PRIV(x)      (((x) & 0x03) << 0x05) // Privilege: 0-3

#define SEG_DATA_RD        0x00
#define SEG_DATA_RDA       0x01
#define SEG_DATA_RDWR      0x02
#define SEG_DATA_RDWRA     0x03
#define SEG_DATA_RDEXPD    0x04
#define SEG_DATA_RDEXPDA   0x05
#define SEG_DATA_RDWREXPD  0x06
#define SEG_DATA_RDWREXPDA 0x07

#define SEG_CODE_EX        0x08
#define SEG_CODE_EXA       0x09
#define SEG_CODE_EXRD      0x0A
#define SEG_CODE_EXRDA     0x0B
#define SEG_CODE_EXC       0x0C
#define SEG_CODE_EXCA      0x0D
#define SEG_CODE_EXRDC     0x0E
#define SEG_CODE_EXRDCA    0x0F

#define GDT_CODE_PL0 \
    SEG_DESCTYPE(1) | \
    SEG_PRES(1)     | \
    SEG_SAVL(0)     | \
    SEG_LONG(0)     | \
    SEG_SIZE(1)     | \
    SEG_GRAN(1)     | \
    SEG_PRIV(0)     | \
    SEG_CODE_EXRD

#define GDT_DATA_PL0 \
    SEG_DESCTYPE(1) | \
    SEG_PRES(1)     | \
    SEG_SAVL(0)     | \
    SEG_LONG(0)     | \
    SEG_SIZE(1)     | \
    SEG_GRAN(1)     | \
    SEG_PRIV(0)     | \
    SEG_DATA_RDWR

#define GDT_CODE_PL3 \
    SEG_DESCTYPE(1) | \
    SEG_PRES(1)     | \
    SEG_SAVL(0)     | \
    SEG_LONG(0)     | \
    SEG_SIZE(1)     | \
    SEG_GRAN(1)     | \
    SEG_PRIV(3)     | \
    SEG_CODE_EXRD

#define GDT_DATA_PL3 \
    SEG_DESCTYPE(1) | \
    SEG_PRES(1)     | \
    SEG_SAVL(0)     | \
    SEG_LONG(0)     | \
    SEG_SIZE(1)     | \
    SEG_GRAN(1)     | \
    SEG_PRIV(3)     | \
    SEG_DATA_RDWR



typedef struct s_layout
{
    uint32_t    base;
    uint32_t    limit;
    uint16_t    flags;
}   t_layout;

typedef struct s_gdt
{
    t_layout    null;
    t_layout    kcode;
    t_layout    kdata;
    t_layout    kstack;
    t_layout    ucode;
    t_layout    udata;
    t_layout    ustack;
}   t_gdt;

typedef struct gdtr
{
    uint16_t limit;
    uint32_t base;
}__attribute__((packed)) t_gdtr;

#define GDT_ENTRIES 7

static uint64_t gdt_table[GDT_ENTRIES];
 __attribute__((section(".gdt")));

void	gdt_flush(struct gdtr *gdtr);
t_gdt	*gdt_init(void);


#endif
