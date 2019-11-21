#pragma once

#include <libruntime/Types.h>
#include <libruntime/Macros.h>

namespace arch
{

void stop() __noreturn;

size_t get_page_size();

uintptr_t get_kernel_base_address();

size_t get_kernel_size();

} // namespace arch
