#include "gdt.h"

static uint64_t gdt_table[GDT_ENTRIES];


static uint64_t
create_descriptor(uint32_t base, uint32_t limit, uint16_t flag)
{
    uint64_t descriptor;

    /* High 32 bits */
    descriptor  =  limit       & 0x000F0000;
    descriptor |= (flag << 8)  & 0x00F0FF00;
    descriptor |= (base >> 16) & 0x000000FF;
    descriptor |=  base         & 0xFF000000;

    descriptor <<= 32;

    /* Low 32 bits */
    descriptor |= base  << 16;
    descriptor |= limit & 0x0000FFFF;

    return (descriptor);
}

t_gdt *
gdt_init(void)
{
    static t_gdt gdt;

    gdt.null.base = 0;
    gdt.null.limit = 0;
    gdt.null.flags = 0;

    gdt.kcode.base = 0;
    gdt.kcode.limit = 0x000FFFFF;
    gdt.kcode.flags = GDT_CODE_PL0;

    gdt.kdata.base = 0;
    gdt.kdata.limit = 0x000FFFFF;
    gdt.kdata.flags = GDT_DATA_PL0;

    gdt.kstack.base = 0;
    gdt.kstack.limit = 0x000FFFFF;
    gdt.kstack.flags = GDT_DATA_PL0;

    gdt.ucode.base = 0;
    gdt.ucode.limit = 0x000FFFFF;
    gdt.ucode.flags = GDT_CODE_PL3;

    gdt.udata.base = 0;
    gdt.udata.limit = 0x000FFFFF;
    gdt.udata.flags = GDT_DATA_PL3;

    gdt.ustack.base = 0;
    gdt.ustack.limit = 0x000FFFFF;
    gdt.ustack.flags = GDT_DATA_PL3;

    gdt_table[0] = create_descriptor(
        gdt.null.base,
        gdt.null.limit,
        gdt.null.flags
    );

    gdt_table[1] = create_descriptor(
        gdt.kcode.base,
        gdt.kcode.limit,
        gdt.kcode.flags
    );

    gdt_table[2] = create_descriptor(
        gdt.kdata.base,
        gdt.kdata.limit,
        gdt.kdata.flags
    );

    gdt_table[3] = create_descriptor(
        gdt.kstack.base,
        gdt.kstack.limit,
        gdt.kstack.flags
    );

    gdt_table[4] = create_descriptor(
        gdt.ucode.base,
        gdt.ucode.limit,
        gdt.ucode.flags
    );

    gdt_table[5] = create_descriptor(
        gdt.udata.base,
        gdt.udata.limit,
        gdt.udata.flags
    );

    gdt_table[6] = create_descriptor(
        gdt.ustack.base,
        gdt.ustack.limit,
        gdt.ustack.flags
    );

    t_gdtr gdtr;

    gdtr.limit = sizeof(gdt_table) - 1;
    gdtr.base = (uint32_t)gdt_table;

    gdt_flush(&gdtr);

    return (&gdt);
}
