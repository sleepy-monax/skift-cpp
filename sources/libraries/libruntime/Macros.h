#pragma once

/* Copyright © 2019-2020 N. Van Bossuyt.                                      */
/* This code is licensed under the 3-Clause BSD License.                      */
/* See: LICENSE.md                                                            */

#ifndef __BUILD_TARGET__
#define __BUILD_TARGET__ "unknown"
#endif

#ifndef __BUILD_GITREF__
#define __BUILD_GITREF__ "unknown/unknown"
#endif

#ifndef __BUILD_UNAME__
#define __BUILD_UNAME__ "unknown"
#endif

#define __packed __attribute__((packed))

#define __aligned(__align) __attribute__((aligned(__align)))

#define __unused(__arg) (void)(__arg)

#define __noreturn __attribute__((noreturn));

#define __str(s) #s
