/* Copyright © 2019-2020 N. Van Bossuyt.                                      */
/* This code is licensed under the 3-Clause BSD License.                      */
/* See: LICENSE.md                                                            */

#include <libsystem/Logger.h>

#include "arch/x86/segmentation/Segmentation.h"
#include "arch/x86/x86.h"

x86::Tss tss;

x86::GdtEntry gdt_entries[GDT_ENTRY_COUNT] = {0};

x86::GdtDescriptor gdt_descriptor = {
    .size = sizeof(x86::GdtEntry) * GDT_ENTRY_COUNT,
    .offset = (u32)&gdt_entries[0],
};

void x86::segmentation_initialize()
{
    tss = x86::Tss();

    logger_info("Populating the GDT...");

    gdt_entries[0] = x86::GdtEntry::create(0, 0, 0, 0);
    gdt_entries[1] = x86::GdtEntry::create(0, 0xffffffff, GDT_PRESENT | GDT_READWRITE | GDT_EXECUTABLE, GDT_FLAGS);
    gdt_entries[2] = x86::GdtEntry::create(0, 0xffffffff, GDT_PRESENT | GDT_READWRITE, GDT_FLAGS);
    gdt_entries[3] = x86::GdtEntry::create(0, 0xffffffff, GDT_PRESENT | GDT_READWRITE | GDT_USER | GDT_EXECUTABLE, GDT_FLAGS);
    gdt_entries[4] = x86::GdtEntry::create(0, 0xffffffff, GDT_PRESENT | GDT_READWRITE | GDT_USER, GDT_FLAGS);
    gdt_entries[5] = x86::GdtEntry::create(((uintptr_t)&tss), sizeof(x86::Tss), GDT_PRESENT | GDT_EXECUTABLE | GDT_ACCESSED, TSS_FLAGS);

    logger_info("Loading the GDT...");
    x86::load_gdt((u32)&gdt_descriptor);

    logger_info("Memory segmentation loaded.");
}
