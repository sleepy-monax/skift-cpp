#pragma once

#include <libruntime/Types.h>
#include <libruntime/Macros.h>

namespace arch
{

void stop() __noreturn;

size_t get_page_size();

} // namespace arch
