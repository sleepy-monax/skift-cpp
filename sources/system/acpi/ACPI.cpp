#include <libsystem/Logger.h>

#include "system/acpi/ACPI.h"
#include "system/acpi/Tables.h"

namespace system::acpi
{

void initialize(void *rsdp_addr)
{
    logger_info("Initializing ACPI sub-system...");

    RSDP *rsdp = reinterpret_cast<RSDP *>(rsdp_addr);

    logger_debug("The name is {#} OEM={#}", libruntime::String(rsdp->signature, 8), libruntime::String(rsdp->OEM_id, 6));

    RSDT *rsdt = reinterpret_cast<RSDT *>(rsdp->rsdt_address);

    for (size_t i = 0; i < rsdt->child_count(); i++)
    {
        SDTH *current = rsdt->child(i);

        logger_info("{#} - {#} - {#}",
                    libruntime::String(current->Signature, 4),
                    libruntime::String(current->OEMID, 6),
                    libruntime::String(current->OEMTableID, 8));
    }
}

} // namespace system::acpi
