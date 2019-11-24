#pragma once

#include <libruntime/Types.h>
#include <libruntime/Macros.h>

#include "system/memory/Region.h"

namespace arch
{

void stop() __noreturn;

size_t get_page_size();

system::memory::Region get_kernel_region();

} // namespace arch
