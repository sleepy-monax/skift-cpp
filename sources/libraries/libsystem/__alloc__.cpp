/*               Durand's Amazing Super Duper Memory functions.               */

#include <libsystem/__plugs__.h>
#include <libsystem/__alloc__.h>
#include <libsystem/Logger.h>
#include <libsystem/Assert.h>
#include <libc/string.h>

#define VERSION "1.1"
#define ALIGNMENT 16ul

#define ALIGN_TYPE char
#define ALIGN_INFO sizeof(ALIGN_TYPE) * 16

#define ALIGN(ptr)                                       \
    if (ALIGNMENT > 1)                                   \
    {                                                    \
        uintptr_t diff;                                  \
        ptr = (void *)((uintptr_t)ptr + ALIGN_INFO);     \
        diff = (uintptr_t)ptr & (ALIGNMENT - 1);         \
        if (diff != 0)                                   \
        {                                                \
            diff = ALIGNMENT - diff;                     \
            ptr = (void *)((uintptr_t)ptr + diff);       \
        }                                                \
        *((ALIGN_TYPE *)((uintptr_t)ptr - ALIGN_INFO)) = \
            diff + ALIGN_INFO;                           \
    }

#define UNALIGN(ptr)                                                     \
    if (ALIGNMENT > 1)                                                   \
    {                                                                    \
        uintptr_t diff = *((ALIGN_TYPE *)((uintptr_t)ptr - ALIGN_INFO)); \
        if (diff < (ALIGNMENT + ALIGN_INFO))                             \
        {                                                                \
            ptr = (void *)((uintptr_t)ptr - diff);                       \
        }                                                                \
    }

#define LIBALLOC_MAGIC 0xc001c0de
#define LIBALLOC_DEAD 0xdeaddead

struct AllocMinor;

struct AllocMajor
{
    unsigned int pages; ///< The number of pages in the block.
    unsigned int size;  ///< The number of pages*pagesize in the block.
    unsigned int usage; ///< The number of bytes used in the block.
    AllocMajor *prev;   ///< Linked list information.
    AllocMajor *next;   ///< Linked list information.
    AllocMinor *first;  ///< A pointer to the first allocated memory in the block.
};

/** This is a structure found at the beginning of all
 * sections in a major block which were allocated by a
 * malloc, calloc, realloc call.
 */
struct AllocMinor
{
    unsigned int magic;    ///< A magic number to idenfity correctness.
    unsigned int size;     ///< The size of the memory allocated. Could be 1 byte or more.
    unsigned int req_size; ///< The size of memory requested.
    AllocMinor *prev;      ///< Linked list information.
    AllocMinor *next;      ///< Linked list information.
    AllocMajor *block;     ///< The owning block. A pointer to the major structure.
};

static AllocMajor *l_memRoot = nullptr; ///< The root memory block acquired from the system.
static AllocMajor *l_bestBet = nullptr; ///< The major with the most free memory.

static unsigned int l_pageSize = 4096;     ///< The size of an individual page. Set up in liballoc_init.
static unsigned int l_pageCount = 16;      ///< The number of pages to request per chunk. Set up in liballoc_init.
static unsigned long long l_allocated = 0; ///< Running total of allocated memory.
static unsigned long long l_inuse = 0;     ///< Running total of used memory.

static long long l_warningCount = 0;     ///< Number of warnings encountered
static long long l_errorCount = 0;       ///< Number of actual errors
static long long l_possibleOverruns = 0; ///< Number of possible overruns

#if defined DEBUG || defined INFO
static void liballoc_dump()
{
#ifdef DEBUG
    AllocMajor *maj = l_memRoot;
    AllocMinor *min = nullptr;
#endif

    printf("liballoc: ------ Memory data ---------------\n");
    printf("liballoc: System memory allocated: %i bytes\n", l_allocated);
    printf("liballoc: Memory in used (malloc'ed): %i bytes\n", l_inuse);
    printf("liballoc: Warning count: %i\n", l_warningCount);
    printf("liballoc: Error count: %i\n", l_errorCount);
    printf("liballoc: Possible overruns: %i\n", l_possibleOverruns);

#ifdef DEBUG
    while (maj != nullptr)
    {
        printf("liballoc: %x: total = %i, used = %i\n",
               maj,
               maj->size,
               maj->usage);

        min = maj->first;
        while (min != nullptr)
        {
            printf("liballoc:    %x: %i bytes\n",
                   min,
                   min->size);
            min = min->next;
        }

        maj = maj->next;
    }
#endif

    FLUSH();
}
#endif

static AllocMajor *allocate_new_page(unsigned int size)
{
    unsigned int st;
    AllocMajor *maj;

    // This is how much space is required.
    st = size + sizeof(AllocMajor);
    st += sizeof(AllocMinor);

    // Perfect amount of space?
    if ((st % l_pageSize) == 0)
        st = st / (l_pageSize);
    else
        st = st / (l_pageSize) + 1;
    // No, add the buffer.

    // Make sure it's >= the minimum size.
    if (st < l_pageCount)
        st = l_pageCount;

    auto result = __plugs__::memory_alloc(st);

    if (result.error() != libruntime::Error::SUCCEED)
    {
        l_warningCount += 1;
        logger_warn("__plugs__::memory_alloc({}) return nullptr", st);
        return nullptr; // uh oh, we ran out of memory.
    }
    else
    {
        maj = (AllocMajor *)result.value();
    }

    maj->prev = nullptr;
    maj->next = nullptr;
    maj->pages = st;
    maj->size = st * l_pageSize;
    maj->usage = sizeof(AllocMajor);
    maj->first = nullptr;

    l_allocated += maj->size;

#ifdef DEBUG
    printf("liballoc: Resource allocated %x of %i pages (%i bytes) for %i size.\n", maj, st, maj->size, size);

    printf("liballoc: Total memory usage = %i KB\n", (int)((l_allocated / (1024))));
    FLUSH();
#endif

    return maj;
}

extern "C" void *__alloc__::malloc(size_t req_size)
{
    int startedBet = 0;
    unsigned long long bestSize = 0;
    void *p = nullptr;
    uintptr_t diff;
    AllocMajor *maj;
    AllocMinor *min;
    AllocMinor *new_min;
    unsigned long size = req_size;

    // For alignment, we adjust size so there's enough space to align.
    if (ALIGNMENT > 1)
    {
        size += ALIGNMENT + ALIGN_INFO;
    }

    // So, ideally, we really want an alignment of 0 or 1 in order
    // to save space.

    __plugs__::memory_lock();

    if (size == 0)
    {
        l_warningCount += 1;
        logger_warn("Alloc( 0 ) called from {#x}", __builtin_return_address(0));

        __plugs__::memory_unlock();

        return malloc(1);
    }

    if (l_memRoot == nullptr)
    {
        l_memRoot = allocate_new_page(size);

        if (l_memRoot == nullptr)
        {
            __plugs__::memory_unlock();
            logger_error("Initial memory block initialization failed", p);
            assert_not_reached();
        }
    }

    // Now we need to bounce through every major and find enough space....

    maj = l_memRoot;
    startedBet = 0;

    // Start at the best bet....
    if (l_bestBet != nullptr)
    {
        bestSize = l_bestBet->size - l_bestBet->usage;

        if (bestSize > (size + sizeof(AllocMinor)))
        {
            maj = l_bestBet;
            startedBet = 1;
        }
    }

    while (maj != nullptr)
    {
        diff = maj->size - maj->usage;

        if (bestSize < diff)
        {
            l_bestBet = maj;
            bestSize = diff;
        }

        // CASE 1:  There is not enough space in this major block.
        if (diff < (size + sizeof(AllocMinor)))
        {
            if (maj->next != nullptr)
            {
                maj = maj->next;
                continue;
            }

            if (startedBet == 1) // If we started at the best bet,
            {                    // let's start all over again.
                maj = l_memRoot;
                startedBet = 0;
                continue;
            }

            // Create a new major block next to this one and...
            maj->next = allocate_new_page(size); // next one will be okay.
            if (maj->next == nullptr)
                break; // no more memory.
            maj->next->prev = maj;
            maj = maj->next;

            // .. fall through to CASE 2 ..
        }

        // CASE 2: It's a brand new block.
        if (maj->first == nullptr)
        {
            maj->first = (AllocMinor *)((uintptr_t)maj + sizeof(AllocMajor));

            maj->first->magic = LIBALLOC_MAGIC;
            maj->first->prev = nullptr;
            maj->first->next = nullptr;
            maj->first->block = maj;
            maj->first->size = size;
            maj->first->req_size = req_size;
            maj->usage += size + sizeof(AllocMinor);

            l_inuse += size;

            p = (void *)((uintptr_t)(maj->first) + sizeof(AllocMinor));

            ALIGN(p);

            __plugs__::memory_unlock(); // release the lock
            return p;
        }

        // CASE 3: Block in use and enough space at the start of the block.
        diff = (uintptr_t)(maj->first);
        diff -= (uintptr_t)maj;
        diff -= sizeof(AllocMajor);

        if (diff >= (size + sizeof(AllocMinor)))
        {
            // Yes, space in front. Squeeze in.
            maj->first->prev = (AllocMinor *)((uintptr_t)maj + sizeof(AllocMajor));
            maj->first->prev->next = maj->first;
            maj->first = maj->first->prev;

            maj->first->magic = LIBALLOC_MAGIC;
            maj->first->prev = nullptr;
            maj->first->block = maj;
            maj->first->size = size;
            maj->first->req_size = req_size;
            maj->usage += size + sizeof(AllocMinor);

            l_inuse += size;

            p = (void *)((uintptr_t)(maj->first) + sizeof(AllocMinor));
            ALIGN(p);

            __plugs__::memory_unlock(); // release the lock
            return p;
        }

        // CASE 4: There is enough space in this block. But is it contiguous?
        min = maj->first;

        // Looping within the block now...
        while (min != nullptr)
        {
            // CASE 4.1: End of minors in a block. Space from last and end?
            if (min->next == nullptr)
            {
                // the rest of this block is free...  is it big enough?
                diff = (uintptr_t)(maj) + maj->size;
                diff -= (uintptr_t)min;
                diff -= sizeof(AllocMinor);
                diff -= min->size;
                // minus already existing usage..

                if (diff >= (size + sizeof(AllocMinor)))
                {
                    // yay....
                    min->next = (AllocMinor *)((uintptr_t)min + sizeof(AllocMinor) + min->size);
                    min->next->prev = min;
                    min = min->next;
                    min->next = nullptr;
                    min->magic = LIBALLOC_MAGIC;
                    min->block = maj;
                    min->size = size;
                    min->req_size = req_size;
                    maj->usage += size + sizeof(AllocMinor);

                    l_inuse += size;

                    p = (void *)((uintptr_t)min + sizeof(AllocMinor));
                    ALIGN(p);

                    __plugs__::memory_unlock(); // release the lock
                    return p;
                }
            }

            // CASE 4.2: Is there space between two minors?
            if (min->next != nullptr)
            {
                // is the difference between here and next big enough?
                diff = (uintptr_t)(min->next);
                diff -= (uintptr_t)min;
                diff -= sizeof(AllocMinor);
                diff -= min->size;
                // minus our existing usage.

                if (diff >= (size + sizeof(AllocMinor)))
                {
                    // yay......
                    new_min = (AllocMinor *)((uintptr_t)min + sizeof(AllocMinor) + min->size);

                    new_min->magic = LIBALLOC_MAGIC;
                    new_min->next = min->next;
                    new_min->prev = min;
                    new_min->size = size;
                    new_min->req_size = req_size;
                    new_min->block = maj;
                    min->next->prev = new_min;
                    min->next = new_min;
                    maj->usage += size + sizeof(AllocMinor);

                    l_inuse += size;

                    p = (void *)((uintptr_t)new_min + sizeof(AllocMinor));
                    ALIGN(p);

                    __plugs__::memory_unlock(); // release the lock
                    return p;
                }
            } // min->next != nullptr

            min = min->next;
        } // while min != nullptr ...

        // CASE 5: Block full! Ensure next block and loop.
        if (maj->next == nullptr)
        {
            if (startedBet == 1)
            {
                maj = l_memRoot;
                startedBet = 0;
                continue;
            }

            // we've run out. we need more...
            maj->next = allocate_new_page(size); // next one guaranteed to be okay
            if (maj->next == nullptr)
                break; //  uh oh,  no more memory.....
            maj->next->prev = maj;
        }

        maj = maj->next;
    } // while (maj != nullptr)

    __plugs__::memory_unlock(); // release the lock

    // Malloc should never return nullptr.
    logger_fatal("Out of memory!");
    assert_not_reached();
}

extern "C" void __alloc__::free(void *ptr)
{
    AllocMinor *min;
    AllocMajor *maj;

    if (ptr == nullptr)
    {
        l_warningCount += 1;
        logger_warn("free(nullptr) called from {x}", __builtin_return_address(0));
        return;
    }

    UNALIGN(ptr);

    __plugs__::memory_lock(); // lockit

    min = (AllocMinor *)((uintptr_t)ptr - sizeof(AllocMinor));

    if (min->magic != LIBALLOC_MAGIC)
    {
        l_errorCount += 1;

        // Check for overrun errors. For all bytes of LIBALLOC_MAGIC
        if (((min->magic & 0xFFFFFF) == (LIBALLOC_MAGIC & 0xFFFFFF)) ||
            ((min->magic & 0xFFFF) == (LIBALLOC_MAGIC & 0xFFFF)) ||
            ((min->magic & 0xFF) == (LIBALLOC_MAGIC & 0xFF)))
        {
            l_possibleOverruns += 1;
            logger_error("Possible 1-3 byte overrun for magic {x} != {x}", min->magic, LIBALLOC_MAGIC);
        }

        if (min->magic == LIBALLOC_DEAD)
        {
            logger_error("Multiple free() attempt on {x} from {x}", ptr, __builtin_return_address(0));
        }
        else
        {
            logger_error("Bad free({x}) called from {x}", ptr, __builtin_return_address(0));
        }

        // being lied to...
        __plugs__::memory_unlock(); // release the lock
        return;
    }

    maj = min->block;

    l_inuse -= min->size;

    maj->usage -= (min->size + sizeof(AllocMinor));
    min->magic = LIBALLOC_DEAD;

    if (min->next != nullptr)
        min->next->prev = min->prev;
    if (min->prev != nullptr)
        min->prev->next = min->next;

    if (min->prev == nullptr)
        maj->first = min->next;

    // We need to clean up after the majors now....

    if (maj->first == nullptr) // Block completely unused.
    {
        if (l_memRoot == maj)
            l_memRoot = maj->next;

        if (l_bestBet == maj)
            l_bestBet = nullptr;

        if (maj->prev != nullptr)
            maj->prev->next = maj->next;

        if (maj->next != nullptr)
            maj->next->prev = maj->prev;

        l_allocated -= maj->size;

        __plugs__::memory_free((uintptr_t)maj, maj->pages);
    }
    else
    {
        if (l_bestBet != nullptr)
        {
            int bestSize = l_bestBet->size - l_bestBet->usage;
            int majSize = maj->size - maj->usage;

            if (majSize > bestSize)
                l_bestBet = maj;
        }
    }

    __plugs__::memory_unlock(); // release the lock
}

extern "C" void *__alloc__::calloc(size_t nobj, size_t size)
{
    int real_size;
    void *p;

    real_size = nobj * size;

    p = malloc(real_size);

    libc::memset(p, 0, real_size);

    return p;
}

extern "C" void *__alloc__::realloc(void *p, size_t size)
{
    void *ptr;
    AllocMinor *min;
    unsigned int real_size;

    // Honour the case of size == 0 => free old and return nullptr
    if (size == 0)
    {
        free(p);
        return nullptr;
    }

    // In the case of a nullptr pointer, return a simple malloc.
    if (p == nullptr)
    {
        return malloc(size);
    }

    // Unalign the pointer if required.
    ptr = p;
    UNALIGN(ptr);

    __plugs__::memory_lock(); // lockit

    min = (AllocMinor *)((uintptr_t)ptr - sizeof(AllocMinor));

    // Ensure it is a valid structure.
    if (min->magic != LIBALLOC_MAGIC)
    {
        l_errorCount += 1;

        // FIXME: this code expecte the hoste to by little endian
        // Check for overrun errors. For all bytes of LIBALLOC_MAGIC
        if (((min->magic & 0xFFFFFF) == (LIBALLOC_MAGIC & 0xFFFFFF)) ||
            ((min->magic & 0xFFFF) == (LIBALLOC_MAGIC & 0xFFFF)) ||
            ((min->magic & 0xFF) == (LIBALLOC_MAGIC & 0xFF)))
        {
            l_possibleOverruns += 1;
            logger_error("Possible 1-3 byte overrun for magic {x} != {x}", min->magic, LIBALLOC_MAGIC);
        }
        else if (min->magic == LIBALLOC_DEAD)
        {
            logger_error("Multiple free() attempt on {x} from {x}.", ptr, __builtin_return_address(0));
        }
        else
        {
            logger_error("Bad free( {x} ) called from {x}.", ptr, __builtin_return_address(0));
        }

        __plugs__::memory_unlock();

        return nullptr;
    }

    // Definitely a memory block.

    real_size = min->req_size;

    if (real_size >= size)
    {
        min->req_size = size;
        __plugs__::memory_unlock();
        return p;
    }

    __plugs__::memory_unlock();

    // If we got here then we're reallocating to a block bigger than us.
    ptr = malloc(size); // We need to allocate new memory
    libc::memcpy(ptr, p, real_size);
    free(p);

    return ptr;
}
