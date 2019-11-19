#pragma once

#ifndef __BUILD_TARGET__
#define __BUILD_TARGET__ "(null)"
#endif

#ifndef __BUILD_UNAME__
#define __BUILD_UNAME__ "(null)"
#endif

#define __packed __attribute__((packed))

#define __aligned(__align) __attribute__((aligned(__align)))

#define __unused(__arg) (void)(__arg)

#define __noreturn __attribute__((noreturn));

#define __str(s) #s
